/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QoreZSock.cpp defines the QoreZSock C++ class */
/*
    Qore Programming Language

    Copyright (C) 2017 - 2018 Qore Technologies, s.r.o.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "zmq-module.h"

#include "QC_ZSocket.h"

#include <regex>
#include <string>
#include <vector>

#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <string.h>

static std::regex url_port_regex("^tcp://.*:(d+|\\*)$", std::regex_constants::ECMAScript | std::regex_constants::icase | std::regex_constants::optimize);

int QoreZSock::poll(short events, int timeout_ms, const char* meth, ExceptionSink *xsink) {
    zmq_pollitem_t p = { sock, 0, events, 0 };
    int rc;
    while (true) {
        rc = zmq_poll(&p, 1, timeout_ms);
        if (rc == -1 && errno == EINTR)
            continue;
        break;
    }
    if (rc > 0)
        return 0;
    if (!rc)
        xsink->raiseException("ZSOCKET-TIMEOUT", "timeout waiting %d ms in %s() for data%s on the socket", timeout_ms, meth, events & ZMQ_POLLOUT ? " to be sent" : "");
    else
        zmq_error(xsink, "ZSOCKET-TIMEOUT", "error in zmq_poll() in %s()", meth);
    return -1;
}

// like czmq's zsock_attach()
int QoreZSock::attach(ExceptionSink *xsink, const char* endpoints, bool do_bind) {
    assert(endpoints);
    // get a vector of all endpoints with regex_token_iterator
    std::regex re(",");
    // passing -1 as the submatch index parameter performs splitting
    std::cregex_token_iterator first{endpoints, endpoints + strlen(endpoints), re, -1}, last;
    std::vector<std::string> vec = {first, last};

    // iterate all endpoints
    for (auto& str : vec) {
        if (str[0] == '@') {
            if (bind(xsink, &str.c_str()[1]) == -1)
                return -1;
        }
        else if (str[0] == '>') {
            if (connect(xsink, &str.c_str()[1]))
                return -1;
        }
        else if (do_bind) {
            if (bind(xsink, str.c_str()) == -1)
                return -1;
        }
        else if (connect(xsink, str.c_str()))
            return -1;
    }

    return 0;
}

int QoreZSock::bind(ExceptionSink *xsink, const char* endpoint, const char* err) {
    std::cmatch match;
    if (regex_search(endpoint, match, url_port_regex)) {
        assert(match.ready());
        assert(match.size() == 2);
        if (!zmq_bind(sock, endpoint)) {
            // get port specification
            std::string str = match[1];
            int port = atoi(str.c_str());
            if (!port) {
                // get actual port bound
                char le[1024];
                size_t size = sizeof(le);
                if (!getSocketOption(ZMQ_LAST_ENDPOINT, &le, &size)) {
                    const char* p = strrchr(le, ':');
                    if (p)
                        port = atoi(p + 1);
                }
            }
            return port;
        }
    }
    else if (!zmq_bind(sock, endpoint)) {
        // NOTE: zmq_bind() is not affected by EINTR
        return 0;
    }

    zmq_error(xsink, err, "failed to bind to \"%s\"", endpoint);
    return -1;
}

int QoreZSock::connect(ExceptionSink *xsink, const char* endpoint, const char* err) {
    // NOTE: zmq_connect() is not affected by EINTR
    int rc = zmq_connect(sock, endpoint);
    if (rc)
        zmq_error(xsink, err, "failed to connect to \"%s\"", endpoint);
    return rc;
}
