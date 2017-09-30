/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QC_ZSocket.h defines the c++ implementation of the Qore ZSocket class */
/*
  QC_ZSocket.h

  Qore Programming Language

  Copyright (C) 2017 Qore Technologies, s.r.o.

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

#ifndef _QORE_ZMQ_QC_ZSOCKET_H

#define _QORE_ZMQ_QC_ZSOCKET_H

#include "zmq-module.h"

#include "QC_ZContext.h"

#include <czmq.h>

#include <string>

#ifndef DEBUG
#define ZSOCK_NOCHECK 1
#endif

// default timeout value: 2 minutes
#define ZSOCK_TIMEOUT_MS 120000

class QoreZSock : public AbstractZmqThreadLocalData {
public:
    // creates the object
    DLLLOCAL QoreZSock(QoreZContext& ctx, int type, ExceptionSink* xsink) : sock(zmq_socket(*ctx, type)) {
        if (!sock) {
            zmq_error(xsink, "ZSOCKET-CONSTRUCTOR-ERROR", "error creating socket");
            return;
        }

        setTimeouts();
    }

    DLLLOCAL void* operator*() {
        return sock;
    }

    DLLLOCAL const void* operator*() const {
        return sock;
    }

    // returns -1 for error (exception raised), 0 for OK
    DLLLOCAL int poll(short events, int timeout_ms, const char* meth, ExceptionSink *xsink);

    // like zsock_attach() from the czmq; returns -1 for error (exception raised), 0 for OK
    DLLLOCAL int attach(ExceptionSink *xsink, const char* endpoints, bool do_bind);

    // returns -1 for error (exception raised), >= 0 for OK, > 0 = the port bound
    DLLLOCAL int bind(ExceptionSink *xsink, const char* endpoint, const char* err = "ZSOCKET-BIND-ERROR");

    // returns -1 for error (exception raised), 0 for OK
    DLLLOCAL int connect(ExceptionSink *xsink, const char* endpoint, const char* err = "ZSOCKET-CONNECT-ERROR");

    //! the error string for exceptions
    DLLLOCAL virtual const char* getErrorString() const {
        return "ZSOCKET-THREAD-ERROR";
    }

    //! returns the socket type code
    virtual int getType() const = 0;

    //! returns the socket type name
    virtual const char* getTypeName() const = 0;

protected:
    int tid = gettid();

    DLLLOCAL virtual ~QoreZSock() {
        zmq_close(sock);
    }

    DLLLOCAL void setTimeouts() {
        // set default timeout values
        int v = ZSOCK_TIMEOUT_MS;
        zmq_setsockopt(sock, ZMQ_SNDTIMEO, &v, sizeof v);
        v = ZSOCK_TIMEOUT_MS;
        zmq_setsockopt(sock, ZMQ_RCVTIMEO, &v, sizeof v);
#ifdef ZMQ_CONNECT_TIMEOUT
        v = ZSOCK_TIMEOUT_MS;
        zmq_setsockopt(sock, ZMQ_CONNECT_TIMEOUT, &v, sizeof v);
#endif
    }

    void* sock = nullptr;
};

class QoreZSockBind : public QoreZSock {
public:
    // creates the object
    DLLLOCAL QoreZSockBind(QoreZContext& ctx, int type, const char* endpoint, ExceptionSink* xsink) : QoreZSock(ctx, type, xsink) {
        if (sock && endpoint && endpoint[0])
            attach(xsink, endpoint, true);
    }
};

class QoreZSockConnect : public QoreZSock {
public:
    // creates the object
    DLLLOCAL QoreZSockConnect(QoreZContext& ctx, int type, const char* endpoint, ExceptionSink* xsink) : QoreZSock(ctx, type, xsink) {
        if (sock && endpoint && endpoint[0])
            attach(xsink, endpoint, false);
    }
};

DLLLOCAL extern QoreClass* QC_ZSOCKET;

#endif // _QORE_ZMQ_QC_ZSOCKET_H
