# FindGMock.cmake
# - Try to find GMock
#
# The following variables are optionally searched for defaults
#  GMOCK_ROOT_DIR:  Base directory where all GMock components are found
#
# Once done this will define
#  GMOCK_FOUND - System has GMock
#  GMOCK_INCLUDE_DIRS - The GMock include directories
#  GMOCK_LIBRARIES - The libraries needed to use GMock

set(HOME_LOCAL_DIR "$ENV{HOME}/local")
find_path(GMOCK_INCLUDE_DIR "gmock/gmock.h"
  PATH_SUFFIXES include
  HINTS ${HOME_LOCAL_DIR}
)

find_library(GMOCK_LIBRARY NAMES "gmock"
  PATH_SUFFIXES lib lib64
  HINTS ${HOME_LOCAL_DIR}
)
include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GMOCK_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(GMock FOUND_VAR GMOCK_FOUND
  REQUIRED_VARS GMOCK_LIBRARY
  GMOCK_INCLUDE_DIR)

if(GMOCK_FOUND)
  set(GMOCK_LIBRARIES ${GMOCK_LIBRARY})
  set(GMOCK_INCLUDE_DIRS ${GMOCK_INCLUDE_DIR})
endif()

mark_as_advanced(GMOCK_INCLUDE_DIR GMOCK_LIBRARY)
unset(HOME_LOCAL_DIR)
