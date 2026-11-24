# COPYRIGHT_BEGIN
#
# The MIT License (MIT)
#
# Copyright (c) 2025 Wizzer Works
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#  For information concerning this header file, contact Mark S. Millard,
#  of Wizzer Works at msm@wizzerworks.com.
#
#  More information concerning Wizzer Works may be found at
#
#      http://www.wizzerworks.com
#
# COPYRIGHT_END


# Stop if cmake version is below 3.10
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project name and enable C++ support
project(libDWP-test VERSION 1.0.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for Magic Lantern environment variables
if (DEFINED ENV{MLE_ROOT})
  message(STATUS "MLE_ROOT=" $ENV{MLE_ROOT})
else()
  message(STATUS "MLE_ROOT NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern")
  message(STATUS "Using " $ENV{MLE_ROOT})
endif()

# Fail early if one of the required packages cannot be found. The tests are
# run against libdwp, libmlmath and libmlutil, which must already be installed
# in $MLE_ROOT.
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
foreach(lib dwp mlmath mlutil)
  string(TOUPPER ${lib} upper)
  find_library(${upper}_LIBRARY
    NAMES ${lib}d ${lib}
    HINTS $ENV{MLE_ROOT}/lib
    NO_DEFAULT_PATH)
  if (NOT ${upper}_LIBRARY)
    message(FATAL_ERROR
      "Could not find lib${lib} in $ENV{MLE_ROOT}/lib. "
      "Install mle-core-util, mle-core-math and mle-core-dwp into MLE_ROOT first.")
  endif()
  message(STATUS "Using ${${upper}_LIBRARY}")
endforeach()

# Specify the compiler macro definitions. MLE_DEBUG must match the libdwp
# build because it changes the layout of some of its classes.
add_compile_definitions(
  ML_MATH_DEBUG=0
  ML_FIXED_POINT=0
  MLE_NOT_UTIL_DLL
  MLE_NOT_MATH_DLL
  MLE_NOT_DWP_DLL
  MLE_REHEARSAL
  $<$<CONFIG:Debug>:MLE_DEBUG>)

# Specify the include directories for the header files
include_directories(
  include
  $ENV{MLE_ROOT}/include)

# The unit test library, libdwptest. Like the autotools build, it is not
# linked against Google Test or the Magic Lantern libraries; the symbols are
# resolved by exampleProgram, which loads it.
add_library(dwptest SHARED
  libdwptest/libdwptest.cxx
  libdwptest/testString.cxx
  libdwptest/testInt.cxx
  libdwptest/testFloat.cxx
  libdwptest/testMlVector2.cxx
  libdwptest/testMlVector3.cxx
  libdwptest/testMlVector4.cxx
  libdwptest/testFloatArray.cxx
  libdwptest/testWorkprint.cxx
  libdwptest/testDatatypes.cxx
  libdwptest/testItems.cxx
  libdwptest/testFuzzFindings.cxx
  libdwptest/testBase.cxx
  libdwptest/testRoundTrip.cxx
  libdwptest/testChecker.cxx)
target_include_directories(dwptest PRIVATE
  $<TARGET_PROPERTY:GTest::gtest,INTERFACE_INCLUDE_DIRECTORIES>)

# The tests find the test workprints by absolute path.
target_compile_definitions(dwptest PRIVATE
  DWPTEST_WORKPRINT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/workprints")

# The DwpChecker command line tool is tested if it is installed in MLE_ROOT.
find_program(DWPCHECKER_TOOL
  NAMES DwpChecker DwpCheckerd
  HINTS $ENV{MLE_ROOT}/bin
  NO_DEFAULT_PATH)
if (DWPCHECKER_TOOL)
  message(STATUS "Using ${DWPCHECKER_TOOL}")
  target_compile_definitions(dwptest PRIVATE
    DWPTEST_CHECKER_TOOL="${DWPCHECKER_TOOL}")
else()
  message(STATUS "DwpChecker not found in $ENV{MLE_ROOT}/bin; its tests are not built")
endif()

# The unit test runner.
add_executable(exampleProgram exampleProgram/exampleProgram.cxx)
target_link_libraries(exampleProgram
  dwptest
  ${DWP_LIBRARY}
  ${MLMATH_LIBRARY}
  ${MLUTIL_LIBRARY}
  GTest::gtest
  ${CMAKE_DL_LIBS}
  Threads::Threads)

# The fuzz target for the workprint reader. With -DDWPTEST_LIBFUZZER=ON and
# clang it is a libFuzzer fuzzer; otherwise it is built with a driver that
# replays the files it is given (and can mutate them), and ctest replays the
# workprints in this directory through it.
option(DWPTEST_LIBFUZZER "Build fuzzDwpInput with libFuzzer (needs clang)" OFF)
add_executable(fuzzDwpInput fuzz/fuzzDwpInput.cxx)
target_link_libraries(fuzzDwpInput
  ${DWP_LIBRARY}
  ${MLMATH_LIBRARY}
  ${MLUTIL_LIBRARY}
  ${CMAKE_DL_LIBS})
if (DWPTEST_LIBFUZZER)
  if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "DWPTEST_LIBFUZZER needs clang (libFuzzer)")
  endif()
  target_compile_options(fuzzDwpInput PRIVATE -fsanitize=fuzzer,address,undefined)
  target_link_options(fuzzDwpInput PRIVATE -fsanitize=fuzzer,address,undefined)
else()
  target_sources(fuzzDwpInput PRIVATE fuzz/standaloneDriver.cxx)
  file(GLOB_RECURSE FUZZ_SEEDS
    ${CMAKE_CURRENT_SOURCE_DIR}/workprints/*.dwp
    ${CMAKE_CURRENT_SOURCE_DIR}/workprints/*.wpf)
  list(SORT FUZZ_SEEDS)
  enable_testing()
  add_test(NAME fuzz-replay COMMAND fuzzDwpInput ${FUZZ_SEEDS})
  add_test(NAME fuzz-mutate COMMAND fuzzDwpInput -mutate=20000 -seed=1 ${FUZZ_SEEDS})
  set_tests_properties(fuzz-replay fuzz-mutate PROPERTIES ENVIRONMENT
    "LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/fuzz/lsan.supp")
endif()

# Register each test with ctest, so a failure is reported for the test that
# failed and tests can be selected with "ctest -R".
include(GoogleTest)
enable_testing()
gtest_discover_tests(exampleProgram
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DISCOVERY_TIMEOUT 30)
