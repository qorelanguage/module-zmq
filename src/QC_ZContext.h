/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QC_ZContext.h defines the c++ implementation of the ZContext class */
/*
  QC_ZContext.h

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

#ifndef _QORE_ZMQ_QC_ZCONTEXT_H

#define _QORE_ZMQ_QC_ZCONTEXT_H

#include "zmq-module.h"

class QoreZContext : public AbstractPrivateData {
public:
   // creates the object
   DLLLOCAL QoreZContext() : ctx(zmq_ctx_new()) {
   }

   DLLLOCAL void* operator*() {
      return ctx;
   }

   DLLLOCAL const void* operator*() const {
      return ctx;
   }

protected:
   DLLLOCAL virtual ~QoreZContext() {
      while (true) {
         int rc = zmq_ctx_term(ctx);
         if (rc && errno == EINTR)
            continue;
         break;
      }
      zmq_ctx_destroy(ctx);
   }

private:
   void* ctx;
};

DLLLOCAL extern QoreClass* QC_ZCONTEXT;
DLLLOCAL extern qore_classid_t CID_ZCONTEXT;

#endif // _QORE_ZMQ_QC_ZCONTEXT_H
