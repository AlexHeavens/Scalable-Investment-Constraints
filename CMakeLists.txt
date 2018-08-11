cmake_minimum_required(VERSION 3.6)
project(ScalableInvestmentConstraints)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "release")
endif()
message("Build type is ${CMAKE_BUILD_TYPE}.")

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BIN_DIR_SUFFIX "_debug")
endif()

# Custom Modules
## Necessary for finding Google Benchmark
set(PROJECT_CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/.cmake/Modules/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_CMAKE_MODULE_PATH})

# Project
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin${BIN_DIR_SUFFIX}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")
set(BENCHMARK_DIR "${PROJECT_SOURCE_DIR}/benchmark")
set(DOC_DIR "${PROJECT_SOURCE_DIR}/doc")
set(DOXYGEN_CONFIG "${DOC_DIR}/doxygen-config")
set(USE_CASE_DIR "${PROJECT_SOURCE_DIR}/use_case")
set(USE_CASE_BIN_DIR "${BIN_DIR}/use_case")

# Build Properties
set(PROJECT_EXE "project_prog${BIN_SUFFIX}")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(TEST_EXE "tests${BIN_SUFFIX}")
set(BENCHMARK_EXE "benchmarks${BIN_SUFFIX}")

# Compile Flags
set(COMMON_FLAGS "-Werror -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-mno-omit-leaf-frame-pointer -fno-omit-frame-pointer -ggdb -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_DEBUG}")

# Required Build Programs
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if (NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else ()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CLANG_TIDY_IGN "-fuchsia-default-arguments")
    set(CLANG_TIDY_PROPERTIES "${CLANG_TIDY_EXE}"
            "-checks=*,${CLANG_TIDY_IGN}"
            "-warnings-as-errors=*"
            "-format-style=google"
            "-header-filter=\"${SRC_DIR}\""
            "-quiet"
            )

    # gtest breaks Google's own clang-tidy warnings.
    set(CLANG_TIDY_TEST_IGN
        "${CLANG_TIDY_IGN},-hicpp-special-member-functions,\
        -cppcoreguidelines-special-member-functions,\
        -hicpp-use-equals-default,-cert-err58-cpp,\
        -modernize-use-equals-default,-modernize-use-equals-delete,\
        -hicpp-use-equals-delete,-cppcoreguidelines-owning-memory,\
        -cppcoreguidelines-pro-type-vararg,-hicpp-vararg,\
        -clang-analyzer-cplusplus.NewDelete,\
        -clang-analyzer-cplusplus.NewDeleteLeaks,\
        -clang-analyzer-optin.cplusplus.VirtualCall"
    )
    set(CLANG_TIDY_TEST_PROPERTIES "${CLANG_TIDY_EXE}"
            "-checks=*,${CLANG_TIDY_TEST_IGN}"
            "-warnings-as-errors=*"
            "-format-style=google"
            "-header-filter=\"${SRC_DIR}\""
            "-quiet"
            )
endif ()

find_package(Doxygen 1.8.5)
if (DOXYGEN_FOUND)
    option(BUILD_DOC "Build documentation" ON)
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )
endif ()

# Source Files
set(MAIN_SRC "${SRC_DIR}/Main.cc")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cc" "${SRC_DIR}/*.c")
list(REMOVE_ITEM SOURCES "${MAIN_SRC}")

set(TEST_MAIN_SRC "${TEST_DIR}/AllTests.cc")
file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/*.cc" "${TEST_DIR}/*.c")
list(REMOVE_ITEM TEST_SOURCES "${TEST_MAIN_SRC}")

file(GLOB_RECURSE BENCHMARK_SOURCES "${BENCHMARK_DIR}/*.cc" "${BENCHMARK_DIR}/*.c")

# Required Libraries
include(FindGTest)
find_package(GTest)

include(FindGMock)
find_package(GMock)

include(FindBenchmark)
find_package(Benchmark)

# Build
add_executable("${PROJECT_EXE}" ${SOURCES} ${MAIN_SRC})
target_include_directories("${PROJECT_EXE}" PUBLIC "${SRC_DIR}")

if (GTEST_FOUND AND GMOCK_FOUND)
    set(GEN_TEST_EXE True)

    add_executable("${TEST_EXE}" ${SOURCES} ${TEST_SOURCES} ${TEST_MAIN_SRC})
    target_include_directories("${TEST_EXE}" PUBLIC ${SRC_DIR} ${TEST_DIR} ${GTEST_INCLUDE_DIRS})
    target_link_libraries("${TEST_EXE}" ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} pthread)

    enable_testing()
    add_test(NAME AllTests COMMAND "${TEST_EXE}")
else ()
    set(GEN_TEST_EXE False)
    message("GTest or GMock not found")
endif ()

if (BENCHMARK_FOUND)
    add_executable("${BENCHMARK_EXE}" ${SOURCES} ${BENCHMARK_SOURCES})
    target_include_directories("${BENCHMARK_EXE}" PUBLIC "${SRC_DIR}" "${BENCHMARK_INCLUDE_DIRS}"  "${USE_CASE_DIR}")
    target_link_libraries("${BENCHMARK_EXE}" ${BENCHMARK_MAIN_LIBRARIES} pthread)
else ()
    message("Benchmark not found")
endif ()

# Use Cases
set(USE_CASES "All" "FilterAssets" "EvaluatePortfolios" "EvaluateRestrictionResults" "OutputRestrictionResults")

foreach(USE_CASE ${USE_CASES})
    set(USE_CASE_BIN "${USE_CASE}")
    set(USE_CASE_MAIN "${USE_CASE_DIR}/${USE_CASE}.cc")

    add_executable("${USE_CASE_BIN}" ${USE_CASE_MAIN} ${SOURCES})
    target_include_directories("${USE_CASE_BIN}" PUBLIC "${SRC_DIR}" "${USE_CASE_DIR}")
    target_link_libraries(${USE_CASE_BIN} pthread)
    set_target_properties(${USE_CASE_BIN}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${USE_CASE_BIN_DIR}
    )
endforeach(USE_CASE)

if (CLANG_TIDY_EXE)
    set_target_properties(
            "${PROJECT_EXE}" PROPERTIES CXX_CLANG_TIDY
            "${CLANG_TIDY_PROPERTIES}"
    )

    if (GEN_TEST_EXE)
        set_target_properties(
                "${TEST_EXE}" PROPERTIES CXX_CLANG_TIDY
                "${CLANG_TIDY_TEST_PROPERTIES}"
        )
    endif ()
endif (CLANG_TIDY_EXE)
