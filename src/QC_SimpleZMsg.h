/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QC_SimpleZMsg.h defines the c++ implementation of the Qore SimpleZMsg class */
/*
  QC_SimpleZMsg.h

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

#ifndef _QORE_ZMQ_QC_SIMPLEZMSG_H

#define _QORE_ZMQ_QC_SIMPLEZMSG_H

#include <qore/Qore.h>

#include <czmq.h>

class QoreSimpleZMsg : public AbstractPrivateData {
public:
   // creates an empty msg
   DLLLOCAL QoreSimpleZMsg() : msg(zmsg_new()) {
   }

   // copies the msg
   DLLLOCAL QoreSimpleZMsg(const QoreSimpleZMsg& old) : msg(zmsg_dup(old.msg)) {
   }

   DLLLOCAL zmsg_t* operator*() {
      return msg;
   }

   DLLLOCAL const zmsg_t* operator*() const {
      return msg;
   }

   DLLLOCAL int addFrames(const QoreListNode* l, ExceptionSink* xsink);
   DLLLOCAL int addFrames(const QoreValueList* l, ExceptionSink* xsink);

protected:
   DLLLOCAL QoreSimpleZMsg(zmsg_t* msg) : msg(msg) {
   }

   DLLLOCAL virtual ~QoreSimpleZMsg() {
      zmsg_destroy(&msg);
   }

   DLLLOCAL int addFrame(const QoreValue v, int i, int max, ExceptionSink* xsink);

private:
   zmsg_t* msg = nullptr;
};

DLLLOCAL extern QoreClass* QC_SIMPLEZMSG;

#endif // _QORE_ZMQ_QC_SIMPLEZMSG_H
