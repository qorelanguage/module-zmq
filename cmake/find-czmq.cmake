# - Try to find CZMQ
# Once done this will define
# CZMQ_FOUND - System has CZMQ
# CZMQ_INCLUDE_DIRS - The CZMQ include directories
# CZMQ_LIBRARIES - The libraries needed to use CZMQ
# CZMQ_DEFINITIONS - Compiler switches required for using CZMQ

include(CheckFunctionExists)
include(CheckCXXSourceCompiles)

find_path(CZMQ_INCLUDE_DIRS czmq.h HINTS $ENV{ZMQ_DIR}/include)
if (CZMQ_INCLUDE_DIRS)
  message(STATUS "found czmq includes in ${CZMQ_INCLUDE_DIRS}")
  file(STRINGS ${CZMQ_INCLUDE_DIRS}/czmq_library.h CZMQ_VERSION_MAJOR_RAW REGEX "^#define CZMQ_VERSION_MAJOR")
  string(REGEX REPLACE "^#define CZMQ_VERSION_MAJOR " "" CZMQ_VERSION_MAJOR ${CZMQ_VERSION_MAJOR_RAW})
  file(STRINGS ${CZMQ_INCLUDE_DIRS}/czmq_library.h CZMQ_VERSION_MINOR_RAW REGEX "^#define CZMQ_VERSION_MINOR")
  string(REGEX REPLACE "^#define CZMQ_VERSION_MINOR " "" CZMQ_VERSION_MINOR ${CZMQ_VERSION_MINOR_RAW})
  file(STRINGS ${CZMQ_INCLUDE_DIRS}/czmq_library.h CZMQ_VERSION_PATCH_RAW REGEX "^#define CZMQ_VERSION_PATCH")
  string(REGEX REPLACE "^#define CZMQ_VERSION_PATCH " "" CZMQ_VERSION_PATCH ${CZMQ_VERSION_PATCH_RAW})
  if (${CZMQ_VERSION_MAJOR} LESS 4)
    message(FATAL_ERROR "czmq (major) version ${CZMQ_VERSION_MAJOR}.${CZMQ_VERSION_MINOR}.${CZMQ_VERSION_PATCH} is less than 4.0.2")
  elseif (${CZMQ_VERSION_MAJOR} EQUAL 4)
    if (${CZMQ_VERSION_MINOR} LESS 0)
      message(FATAL_ERROR "czmq (minor) version ${CZMQ_VERSION_MAJOR}.${CZMQ_VERSION_MINOR}.${CZMQ_VERSION_PATCH} is less than the minimum 4.0.2")
    elseif(${CZMQ_VERSION_MINOR} EQUAL 0)
      if (${CZMQ_VERSION_PATCH} LESS 2)
        message(FATAL_ERROR "czmq (patch) version ${CZMQ_VERSION_MAJOR}.${CZMQ_VERSION_MINOR}.${CZMQ_VERSION_PATCH} is less than the minimum 4.0.2")
      endif()
    endif()
  endif()
else()
  message(FATAL_ERROR "czmqi ncludes not found (set ZMQ_DIR to the ZeroMQ/czmq installation location and try again)")
endif(CZMQ_INCLUDE_DIRS)
find_library(CZMQ_LIBRARY NAMES czmq HINTS $ENV{ZMQ_DIR}/lib)

set(CZMQ_LIBRARIES ${CZMQ_LIBRARY})

set(CMAKE_REQUIRED_INCLUDES ${CZMQ_INCLUDE_DIRS})
set(CMAKE_REQUIRED_LIBRARIES ${CZMQ_LIBRARY})
check_function_exists(zframe_meta HAVE_ZFRAME_META)
if(HAVE_ZFRAME_META)
  add_definitions(-DHAVE_ZFRAME_META)
endif(HAVE_ZFRAME_META)

# check signature of zmsg_encode()
check_cxx_source_compiles("
#include <zmq.h>
#include <czmq.h>
#include <zmsg.h>
#include <zframe.h>
int main() {
    zframe_t* f = zmsg_encode((zmsg_t*)0);
}
" HAVE_ZMSG_ENCODE_TO_ZFRAME)

if(HAVE_ZMSG_ENCODE_TO_ZFRAME)
  add_definitions(-DHAVE_ZMSG_ENCODE_TO_ZFRAME)
else()
  check_cxx_source_compiles("
#include <czmq.h>
#include <zmsg.h>
int main() {
    byte* p;
    size_t i = zmsg_encode((zmsg_t*)0, &p);
}
" HAVE_ZMSG_ENCODE_TO_BUFFER)
  if(HAVE_ZMSG_ENCODE_TO_BUFFER)
    add_definitions(-DHAVE_ZMSG_ENCODE_TO_BUFFER)
  endif(HAVE_ZMSG_ENCODE_TO_BUFFER)
endif(HAVE_ZMSG_ENCODE_TO_ZFRAME)

include ( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set CZMQ_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args ( CZMQ DEFAULT_MSG CZMQ_LIBRARY CZMQ_INCLUDE_DIRS )
