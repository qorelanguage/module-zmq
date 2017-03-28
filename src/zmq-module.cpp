/* indent-tabs-mode: nil -*- */
/*
  Qore zmq module

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

#include "zmq-module.h"

static QoreStringNode* zmq_module_init();
static void zmq_module_ns_init(QoreNamespace* rns, QoreNamespace* qns);
static void zmq_module_delete();

DLLLOCAL void preinitZFrameClass();
DLLLOCAL void preinitSimpleZMsgClass();

DLLLOCAL QoreClass* initZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initPubZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initSubZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initReqZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initRepZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initDealerZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initRouterZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initPushZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initPullZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initXPubZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initXSubZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initPairZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initStreamZSocketClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initZFrameClass(QoreNamespace& ns);
DLLLOCAL QoreClass* initSimpleZMsgClass(QoreNamespace& ns);

// qore module symbols
DLLEXPORT char qore_module_name[] = "zmq";
DLLEXPORT char qore_module_version[] = PACKAGE_VERSION;
DLLEXPORT char qore_module_description[] = "zmq module";
DLLEXPORT char qore_module_author[] = "David Nichols";
DLLEXPORT char qore_module_url[] = "http://qore.org";
DLLEXPORT int qore_module_api_major = QORE_MODULE_API_MAJOR;
DLLEXPORT int qore_module_api_minor = QORE_MODULE_API_MINOR;
DLLEXPORT qore_module_init_t qore_module_init = zmq_module_init;
DLLEXPORT qore_module_ns_init_t qore_module_ns_init = zmq_module_ns_init;
DLLEXPORT qore_module_delete_t qore_module_delete = zmq_module_delete;
DLLEXPORT qore_license_t qore_module_license = QL_MIT;
DLLEXPORT char qore_module_license_str[] = "MIT";

DLLLOCAL void init_zmq_functions(QoreNamespace& ns);
DLLLOCAL void init_zmq_constants(QoreNamespace& ns);

QoreNamespace zmqns("ZMQ");

static QoreStringNode* zmq_module_init() {
   preinitZFrameClass();
   preinitSimpleZMsgClass();

   zmqns.addSystemClass(initZFrameClass(zmqns));
   zmqns.addSystemClass(initSimpleZMsgClass(zmqns));

   zmqns.addSystemClass(initZSocketClass(zmqns));
   zmqns.addSystemClass(initPubZSocketClass(zmqns));
   zmqns.addSystemClass(initSubZSocketClass(zmqns));
   zmqns.addSystemClass(initReqZSocketClass(zmqns));
   zmqns.addSystemClass(initRepZSocketClass(zmqns));
   zmqns.addSystemClass(initDealerZSocketClass(zmqns));
   zmqns.addSystemClass(initRouterZSocketClass(zmqns));
   zmqns.addSystemClass(initPushZSocketClass(zmqns));
   zmqns.addSystemClass(initPullZSocketClass(zmqns));
   zmqns.addSystemClass(initXPubZSocketClass(zmqns));
   zmqns.addSystemClass(initXSubZSocketClass(zmqns));
   zmqns.addSystemClass(initPairZSocketClass(zmqns));
   zmqns.addSystemClass(initStreamZSocketClass(zmqns));

   init_zmq_constants(zmqns);

   return 0;
}

static void zmq_module_ns_init(QoreNamespace* rns, QoreNamespace* qns) {
   qns->addNamespace(zmqns.copy());
}

static void zmq_module_delete() {
}

// module library functions
void zmq_error(ExceptionSink* xsink, const char* err, const char* desc_fmt, ...) {
   va_list args;

   QoreString desc;

   while (true) {
      va_start(args, desc_fmt);
      int rc = desc.vsprintf(desc_fmt, args);
      va_end(args);
      if (!rc)
         break;
   }

   desc.concat(": ");
   desc.concat(zmq_strerror(errno));

   xsink->raiseExceptionArg(err, new QoreBigIntNode(errno), desc.c_str());
}
