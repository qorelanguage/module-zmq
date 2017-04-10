# - Try to find ZMQ
# Once done this will define
# ZMQ_FOUND - System has ZMQ
# ZMQ_INCLUDE_DIRS - The ZMQ include directories
# ZMQ_LIBRARIES - The libraries needed to use ZMQ
# ZMQ_DEFINITIONS - Compiler switches required for using ZMQ

find_path(ZMQ_INCLUDE_DIRS zmq.h HINTS $ENV{ZMQ_DIR}/include)
if (ZMQ_INCLUDE_DIRS)
  message(STATUS "found ZeroMQ includes in ${ZMQ_INCLUDE_DIRS}")
  file(STRINGS ${ZMQ_INCLUDE_DIRS}/zmq.h ZMQ_VERSION_MAJOR_RAW REGEX "^#define ZMQ_VERSION_MAJOR")
  string(REGEX REPLACE "^#define ZMQ_VERSION_MAJOR " "" ZMQ_VERSION_MAJOR ${ZMQ_VERSION_MAJOR_RAW})
  file(STRINGS ${ZMQ_INCLUDE_DIRS}/zmq.h ZMQ_VERSION_MINOR_RAW REGEX "^#define ZMQ_VERSION_MINOR")
  string(REGEX REPLACE "^#define ZMQ_VERSION_MINOR " "" ZMQ_VERSION_MINOR ${ZMQ_VERSION_MINOR_RAW})
  file(STRINGS ${ZMQ_INCLUDE_DIRS}/zmq.h ZMQ_VERSION_PATCH_RAW REGEX "^#define ZMQ_VERSION_PATCH")
  string(REGEX REPLACE "^#define ZMQ_VERSION_PATCH " "" ZMQ_VERSION_PATCH ${ZMQ_VERSION_PATCH_RAW})
  if (${ZMQ_VERSION_MAJOR} LESS 4)
    message(FATAL_ERROR "ZeroMQ (major) version ${ZMQ_VERSION_MAJOR}.${ZMQ_VERSION_MINOR}.${ZMQ_VERSION_PATCH} is less than 4.2.2")
  elseif (${ZMQ_VERSION_MAJOR} EQUAL 4)
    if (${ZMQ_VERSION_MINOR} LESS 2)
      message(FATAL_ERROR "ZeroMQ (minor) version ${ZMQ_VERSION_MAJOR}.${ZMQ_VERSION_MINOR}.${ZMQ_VERSION_PATCH} is less than the minimum 4.2.2")
    elseif(${ZMQ_VERSION_MINOR} EQUAL 2)
      if (${ZMQ_VERSION_PATCH} LESS 2)
        message(FATAL_ERROR "ZeroMQ (patch) version ${ZMQ_VERSION_MAJOR}.${ZMQ_VERSION_MINOR}.${ZMQ_VERSION_PATCH} is less than the minimum 4.2.2")
      endif()
    endif()
  endif()
else()
  message(FATAL_ERROR "ZeroMQ includes not found (set ZMQ_DIR to the ZeroMQ installation location and try again)")
endif(ZMQ_INCLUDE_DIRS)

find_library(ZMQ_LIBRARY NAMES zmq HINTS $ENV{ZMQ_DIR}/lib)

set(ZMQ_LIBRARIES ${ZMQ_LIBRARY})

include ( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set ZMQ_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args ( ZMQ DEFAULT_MSG ZMQ_LIBRARY ZMQ_INCLUDE_DIRS )
