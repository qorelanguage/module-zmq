/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QC_ZSocketPull.h defines the c++ implementation of the ZSocketPull class */
/*
  QC_ZSocketPull.h

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

#ifndef _QORE_ZMQ_QC_ZSOCKETPULL_H

#define _QORE_ZMQ_QC_ZSOCKETPULL_H

#include "QC_ZSocket.h"

class QorePullZSock : public QoreZSockBind {
public:
   // creates the object
   DLLLOCAL QorePullZSock(QoreZContext& ctx, const char* endpoint, ExceptionSink* xsink) : QoreZSockBind(ctx, ZMQ_PULL, endpoint, xsink) {
   }

   DLLLOCAL virtual int getType() const {
      return ZMQ_PULL;
   }

   DLLLOCAL virtual const char* getTypeName() const {
      return "PULL";
   }
};

#endif // _QORE_ZMQ_QC_ZSOCKETPULL_H
