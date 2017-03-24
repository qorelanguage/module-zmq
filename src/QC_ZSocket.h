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

#include <qore/Qore.h>

#include <czmq.h>

#ifndef DEBUG
#define ZSOCK_NOCHECK 1
#endif

class QoreZSock : public AbstractPrivateData {
public:
   // creates the object
   DLLLOCAL QoreZSock(int type) : sock(zsock_new(type)) {
   }

   DLLLOCAL zsock_t* operator*() {
      return sock;
   }

   DLLLOCAL const zsock_t* operator*() const {
      return sock;
   }

protected:
   DLLLOCAL QoreZSock(zsock_t* sock) : sock(sock) {
   }

   DLLLOCAL virtual ~QoreZSock() {
      zsock_destroy(&sock);
   }

private:
   zsock_t* sock = nullptr;
};

DLLLOCAL extern QoreClass* QC_ZSOCKET;

#endif // _QORE_ZMQ_QC_ZSOCKET_H
