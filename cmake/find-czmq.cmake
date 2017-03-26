# - Try to find CZMQ
# Once done this will define
# CZMQ_FOUND - System has CZMQ
# CZMQ_INCLUDE_DIRS - The CZMQ include directories
# CZMQ_LIBRARIES - The libraries needed to use CZMQ
# CZMQ_DEFINITIONS - Compiler switches required for using CZMQ

find_path(CZMQ_INCLUDE_DIRS czmq.h HINTS $ENV{ZMQ_DIR}/include)
find_library(CZMQ_LIBRARY NAMES czmq HINTS $ENV{ZMQ_DIR}/lib)

set(CZMQ_LIBRARIES ${CZMQ_LIBRARY})

include ( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set CZMQ_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args ( CZMQ DEFAULT_MSG CZMQ_LIBRARY CZMQ_INCLUDE_DIRS )
