/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QC_ZSocketSub.h defines the c++ implementation of the ZSocketSub class */
/*
  QC_ZSocketSub.h

  Qore Programming Language

  Copyright (C) 2017 - 2018 Qore Technologies, s.r.o.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as sublished by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _QORE_ZMQ_QC_ZSOCKETSUB_H

#define _QORE_ZMQ_QC_ZSOCKETSUB_H

#include "QC_ZSocket.h"

class QoreSubZSock : public QoreZSockConnect {
public:
   // creates the object
   DLLLOCAL QoreSubZSock(QoreZContext& ctx, const char* endpoint, const QoreStringNode* subs, ExceptionSink* xsink) : QoreZSockConnect(ctx, ZMQ_SUB, endpoint, xsink) {
      if (!*xsink && subs)
         subscribe(xsink, subs);
   }

   DLLLOCAL virtual int getType() const {
      return ZMQ_SUB;
   }

   DLLLOCAL virtual const char* getTypeName() const {
      return "SUB";
   }

   DLLLOCAL int subscribeIntern(ExceptionSink* xsink, const QoreString* subs_utf8) {
      assert(subs_utf8->getEncoding() == QCS_UTF8);

      while (true) {
         int rc = zmq_setsockopt(**this, ZMQ_SUBSCRIBE, subs_utf8 ? subs_utf8->c_str() : nullptr, subs_utf8 ? subs_utf8->size() : 0);
         if (rc) {
            if (errno == EINTR)
               continue;
            zmq_error(xsink, "ZSOCKET-SUBSCRIBE-ERROR", "error adding subscription \"%s\"", subs_utf8->c_str());
            return -1;
         }
         break;
      }

      return 0;
   }

   DLLLOCAL int subscribe(ExceptionSink* xsink, const QoreString* subs) {
      TempEncodingHelper subs_utf8;

      if (subs && !subs_utf8.set(subs, QCS_UTF8, xsink))
         return -1;

      return subscribeIntern(xsink, *subs_utf8);
   }
};

#endif // _QORE_ZMQ_QC_ZSOCKETSUB_H
