// COPYRIGHT_BEGIN
// COPYRIGHT_END

#ifndef _LIBDWPTEST_H_
#define _LIBDWPTEST_H_

// Include system header files.
#include <stdio.h>
#include <memory>
#include <string>

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern Digital Workprint header files.
#include <mle/DwpItem.h>
#include <mle/DwpProperty.h>

// Declare external prototypes.

// Register the DWP test environment, which initializes the DWP library once
// for the whole test run. Call this from main() before running the tests.
// It also forces libdwptest.so to be loaded.
extern bool initDWPTest();

namespace dwptest {

// An owning pointer to a workprint hierarchy. The hierarchy is deleted when
// the pointer goes out of scope.
typedef std::unique_ptr<MleDwpItem> WorkprintPtr;

// Return the path of a test workprint. The workprints are found in the
// "workprints" directory of the test source tree, so the tests do not depend
// on the current directory.
std::string workprintPath(const char *name);

// Load a workprint from a file. Returns NULL if the file cannot be read.
WorkprintPtr loadFile(const std::string &filename);

// Parse a workprint from a string. The string must begin with the
// "#DWP 1.0 ascii" header. Returns NULL if the string cannot be parsed.
WorkprintPtr loadString(const std::string &text);

// Find the first Property with the given name. Returns NULL if there is none.
MleDwpProperty *findProperty(MleDwpItem *root, const char *name);

// Write a workprint hierarchy to a string.
std::string writeString(MleDwpItem *root);

} // namespace dwptest

// Base class for test fixtures that read the properties of the test
// workprint, "TestDWPProperties.dwp". The workprint is loaded once for each
// test suite and deleted when the suite finishes.
class TestWorkprintFixture : public ::testing::Test
{
  protected:
    static void SetUpTestSuite();
    static void TearDownTestSuite();

    // The workprint loaded for the test suite.
    static MleDwpItem *g_workprint;
};

#endif /* _LIBDWPTEST_H_ */
