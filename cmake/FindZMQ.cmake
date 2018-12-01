# - Try to find ZMQ
# Once done this will define
# ZMQ_FOUND - System has ZMQ
# ZMQ_INCLUDE_DIR - The ZMQ include directories
# ZMQ_INCLUDE_DIRS - The ZMQ include directories
# ZMQ_LIBRARY - The libraries needed to use ZMQ
# ZMQ_LIBRARIES - The libraries needed to use ZMQ
# ZMQ_DEFINITIONS - Compiler switches required for using ZMQ

find_path(ZMQ_INCLUDE_DIR zmq.h HINTS $ENV{ZMQ_DIR}/include)
if (ZMQ_INCLUDE_DIR)
    message(STATUS "Found ZMQ includes: ${ZMQ_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "ZMQ includes not found (set ZMQ_DIR to the ZMQ installation location and try again)")
endif(ZMQ_INCLUDE_DIR)

find_library(ZMQ_LIBRARY NAMES zmq HINTS $ENV{ZMQ_DIR}/lib)

set(ZMQ_INCLUDE_DIRS ${ZMQ_INCLUDE_DIR})
set(ZMQ_LIBRARIES ${ZMQ_LIBRARY})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ZMQ_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(ZMQ DEFAULT_MSG ZMQ_LIBRARY ZMQ_INCLUDE_DIR)
