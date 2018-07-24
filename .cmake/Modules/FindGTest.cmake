# FindGTest.cmake
# - Try to find GTest
#
# The following variables are optionally searched for defaults
#  GTEST_ROOT_DIR:  Base directory where all GTest components are found
#
# Once done this will define
#  GTEST_FOUND - System has GTest
#  GTEST_INCLUDE_DIRS - The GTest include directories
#  GTEST_LIBRARIES - The libraries needed to use GTest

set(HOME_LOCAL_DIR "$ENV{HOME}/local")
find_path(GTEST_INCLUDE_DIR "gtest/gtest.h"
  PATH_SUFFIXES include
  HINTS ${HOME_LOCAL_DIR}
)

find_library(GTEST_LIBRARY NAMES "gtest"
  PATH_SUFFIXES lib lib64
  HINTS ${HOME_LOCAL_DIR}
)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GTEST_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(GTest FOUND_VAR GTEST_FOUND
  REQUIRED_VARS GTEST_LIBRARY
  GTEST_INCLUDE_DIR)

if(GTEST_FOUND)
  set(GTEST_LIBRARIES ${GTEST_LIBRARY})
  set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
endif()

mark_as_advanced(GTEST_INCLUDE_DIR GTEST_LIBRARY)
