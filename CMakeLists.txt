cmake_minimum_required(VERSION 2.8.3)

project(qore-zmq-module)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 0)

set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

find_package(Qore 0.8.13 REQUIRED)

include("${CMAKE_SOURCE_DIR}/cmake/find-zmq.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/find-czmq.cmake")

include_directories(${CZMQ_INCLUDE_DIRS})
include_directories(${ZMQ_INCLUDE_DIRS})

# Check for C++11.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "release")
endif (NOT CMAKE_BUILD_TYPE)
string(TOLOWER ${CMAKE_BUILD_TYPE} QORE_BUILD_TYPE_LWR)
if (${QORE_BUILD_TYPE_LWR} MATCHES "debug")
    add_definitions(-DDEBUG)
else ()
    add_definitions(-DNDEBUG)
endif ()

set(QPP_SRC
    src/QC_ZContext.qpp
    src/QC_ZSocket.qpp
    src/QC_ZSocketPub.qpp
    src/QC_ZSocketSub.qpp
    src/QC_ZSocketReq.qpp
    src/QC_ZSocketRep.qpp
    src/QC_ZSocketDealer.qpp
    src/QC_ZSocketRouter.qpp
    src/QC_ZSocketPush.qpp
    src/QC_ZSocketPull.qpp
    src/QC_ZSocketXPub.qpp
    src/QC_ZSocketXSub.qpp
    src/QC_ZSocketPair.qpp
    src/QC_ZSocketStream.qpp
    #src/QC_ZSocketRadio.qpp
    #src/QC_ZSocketDish.qpp
    src/QC_ZFrame.qpp
    src/QC_ZMsg.qpp
    src/qc_zmq.qpp
    src/ql_zmq.qpp
)

set(CPP_SRC
    src/zmq-module.cpp
    src/QoreZSock.cpp
)

qore_wrap_qpp_value(QPP_SOURCES ${QPP_SRC})

set(module_name "zmq")

set(QORE_DOX_TMPL_SRC
  docs/mainpage.dox.tmpl
)

add_library(${module_name} SHARED ${CPP_SRC} ${QPP_SOURCES})
include_directories(${ZMQ_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)
target_include_directories(${module_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)

target_link_libraries(${module_name} ${ZMQ_LIBRARIES} ${CZMQ_LIBRARIES})

qore_binary_module(${module_name} ${PROJECT_VERSION})

qore_dist(${PROJECT_VERSION})

qore_config_info()

FIND_PACKAGE(Doxygen)
if (DOXYGEN_FOUND)
  qore_wrap_dox(QORE_DOX_SRC ${QORE_DOX_TMPL_SRC})
  add_custom_target(QORE_MOD_DOX_FILES DEPENDS ${QORE_DOX_SRC})
  add_dependencies(docs QORE_MOD_DOX_FILES)
endif()
