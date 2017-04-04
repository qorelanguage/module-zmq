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

#include <czmq.h>

#ifndef DEBUG
#define ZSOCK_NOCHECK 1
#endif

// default timeout value: 2 minutes
#define ZSOCK_TIMEOUT_MS 120000

class QoreZSock : public AbstractPrivateData {
   friend class QoreZSockLockHelper;
public:
   // creates the object
   DLLLOCAL QoreZSock(int type) : sock(zsock_new(type)) {
      int v = ZSOCK_TIMEOUT_MS;
      zmq_setsockopt(sock, ZMQ_SNDTIMEO, &v, sizeof v);
      v = ZSOCK_TIMEOUT_MS;
      zmq_setsockopt(sock, ZMQ_RCVTIMEO, &v, sizeof v);
#ifdef ZMQ_CONNECT_TIMEOUT
      v = ZSOCK_TIMEOUT_MS;
      zmq_setsockopt(sock, ZMQ_CONNECT_TIMEOUT, &v, sizeof v);
#endif
   }

   DLLLOCAL zsock_t* operator*() {
      return sock;
   }

   DLLLOCAL const zsock_t* operator*() const {
      return sock;
   }

   // returns -1 for error (exception raised), 0 for OK
   DLLLOCAL int poll(short events, int timeout_ms, const char* meth, ExceptionSink *xsink);

protected:
   QoreThreadLock lck;

   DLLLOCAL QoreZSock(zsock_t* sock, ExceptionSink* xsink) : sock(sock) {
      if (!sock)
         zmq_error(xsink, "ZSOCKET-CONSTRUCTOR-ERROR", "error creating socket");
   }

   DLLLOCAL virtual ~QoreZSock() {
      zsock_destroy(&sock);
   }

private:
   zsock_t* sock = nullptr;
};

class QoreZSockLockHelper : public AutoLocker {
public:
   DLLLOCAL QoreZSockLockHelper(QoreZSock& s) : AutoLocker(s.lck) {
   }

   DLLLOCAL QoreZSockLockHelper(QoreZSock* s) : AutoLocker(s->lck) {
   }
};

DLLLOCAL extern QoreClass* QC_ZSOCKET;

#endif // _QORE_ZMQ_QC_ZSOCKET_H
