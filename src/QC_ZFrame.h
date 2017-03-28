/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QC_ZFrame.h defines the c++ implementation of the Qore ZFrame class */
/*
  QC_ZFrame.h

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

#ifndef _QORE_ZMQ_QC_ZFRAME_H

#define _QORE_ZMQ_QC_ZFRAME_H

#include <qore/Qore.h>

#include <czmq.h>

class QoreZFrame : public AbstractPrivateData {
public:
   // creates an empty frame
   DLLLOCAL QoreZFrame() : frame(zframe_new_empty()) {
   }

   // creates a frame from the data supplied
   DLLLOCAL QoreZFrame(const void* data, size_t size) : frame(zframe_new(data, size)) {
   }

   //! creates a frame from a zframe_t ptr
   DLLLOCAL QoreZFrame(zframe_t* frame) : frame(frame) {
   }

   // copies the frame
   DLLLOCAL QoreZFrame(const QoreZFrame& old) : frame(zframe_dup(old.frame)) {
   }

   DLLLOCAL zframe_t* operator*() {
      return frame;
   }

   DLLLOCAL const zframe_t* operator*() const {
      return frame;
   }

   DLLLOCAL zframe_t** getPtr() {
      return &frame;
   }

protected:
   DLLLOCAL virtual ~QoreZFrame() {
      zframe_destroy(&frame);
   }

private:
   zframe_t* frame = nullptr;
};

DLLLOCAL extern QoreClass* QC_ZFRAME;
DLLLOCAL extern qore_classid_t CID_ZFRAME;

#endif // _QORE_ZMQ_QC_ZFRAME_H
