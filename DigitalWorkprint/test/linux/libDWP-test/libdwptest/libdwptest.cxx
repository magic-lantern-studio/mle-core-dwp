// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2016 - 2018 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include DWP test header files.
#include "libdwptest.h"

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpFinder.h>
#include <mle/DwpInput.h>
#include <mle/DwpOutput.h>

// Include Google Test header files.
#include "gtest/gtest.h"

// The directory containing the test workprints. The build normally defines
// this as the absolute path of the "workprints" source directory.
#ifndef DWPTEST_WORKPRINT_DIR
#define DWPTEST_WORKPRINT_DIR "workprints"
#endif

// Initializes the DWP library once, before any test runs. mleDwpInit() must
// not be called more than once.
class DwpEnvironment : public ::testing::Environment
{
  public:
    void SetUp() override {
        mleDwpInit();
    }
};

bool initDWPTest() {
    static bool registered = false;
    if (! registered) {
        // Google Test takes ownership of the environment.
        ::testing::AddGlobalTestEnvironment(new DwpEnvironment);
        registered = true;
    }
    return true;
}

namespace dwptest {

std::string workprintPath(const char *name) {
    return std::string(DWPTEST_WORKPRINT_DIR) + "/" + name;
}

WorkprintPtr loadFile(const std::string &filename) {
    return WorkprintPtr(MleDwpItem::readAll(filename.c_str()));
}

WorkprintPtr loadString(const std::string &text) {
    MleDwpInput in;

    // setBuffer() returns zero on success. The input uses the string in place,
    // so the string must stay alive until the parse is finished.
    if (in.setBuffer(text.c_str()))
        return WorkprintPtr();
    return WorkprintPtr(MleDwpItem::readAll(&in));
}

MleDwpProperty *findProperty(MleDwpItem *root, const char *name) {
    MleDwpFinder finder;
    finder.setType(MleDwpProperty::typeId);
    finder.setName(name);

    MleDwpItem *item = finder.find(root);
    if (item == NULL || ! item->isa(MleDwpProperty::typeId))
        return NULL;
    return static_cast<MleDwpProperty *>(item);
}

std::string writeString(MleDwpItem *root) {
    // The output writes into a memory buffer owned by the output object, so
    // copy it before the object goes away.
    MleDwpOutput out;
    root->write(&out);
    return std::string(out.getBuffer());
}

} // namespace dwptest

MleDwpItem *TestWorkprintFixture::g_workprint = NULL;

void TestWorkprintFixture::SetUpTestSuite() {
    g_workprint = dwptest::loadFile(
        dwptest::workprintPath("TestDWPProperties.dwp")).release();
}

void TestWorkprintFixture::TearDownTestSuite() {
    delete g_workprint;
    g_workprint = NULL;
}
