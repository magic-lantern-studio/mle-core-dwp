/**
 * @file testFuzzFindings.cxx
 *
 * Tests for the defects that the fuzz target (fuzz/fuzzDwpInput.cxx) found.
 * Each is a workprint that used to crash the reader or the writer.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2025 Wizzer Works
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

#include <string>

#include "libdwptest.h"

#include <mle/DwpInput.h>

using namespace dwptest;

// A File item that names a file that cannot be opened used to close the
// input twice, which asserted.
TEST(FuzzFindingTest, FileItemThatCannotBeOpenedIsAnError) {
	MleDwpInput in;
	ASSERT_EQ(0, in.setBuffer("#DWP 1.0 ascii\n(File nosuchfile.wpf)\n"));
	std::unique_ptr<MleDwpItem> root(MleDwpItem::readAll(&in));
	EXPECT_GT(in.getErrorCount(), 0);
}

// A number that is cut short by a bad byte at the start of the input
// used to put back a byte that was never counted, and index the line
// buffer with -2.
TEST(FuzzFindingTest, BadNumberAtTheStartOfAValueIsAnError) {
	MleDwpInput in;
	ASSERT_EQ(0, in.setBuffer("#DWP 1.0 ascii\n(PropertyDef p MlVector2)\n"
		"(Property p MlVector2 ( -x 1 ))\n"));
	std::unique_ptr<MleDwpItem> root(MleDwpItem::readAll(&in));
	EXPECT_GT(in.getErrorCount(), 0);
}

// A String property whose value could not be read used to keep the
// uninitialized buffer, and writing it read past the end.
TEST(FuzzFindingTest, StringThatCouldNotBeReadWritesAsNothingBad) {
	MleDwpInput in;
	ASSERT_EQ(0, in.setBuffer("#DWP 1.0 ascii\n(Property p string \"unterminated"));
	std::unique_ptr<MleDwpItem> root(MleDwpItem::readAll(&in));
	if (root)
		writeString(root.get());
	EXPECT_GT(in.getErrorCount(), 0);
}

// An Array property that was never read used to be written through a
// NULL pointer.
TEST(FuzzFindingTest, ArrayPropertyWithoutAValueCanBeWritten) {
	MleDwpInput in;
	ASSERT_EQ(0, in.setBuffer("#DWP 1.0 ascii\n(Property p FloatArray [1,2 \n"));
	std::unique_ptr<MleDwpItem> root(MleDwpItem::readAll(&in));
	if (root) {
		writeString(root.get());
		std::unique_ptr<MleDwpItem> copy(root->copy(1));
	}
	SUCCEED();
}

// A byte above 127 in a buffer used to be read as a negative number, and
// so as the end of the input.
TEST(FuzzFindingTest, HighByteInABufferIsNotTheEndOfTheInput) {
	MleDwpInput in;
	ASSERT_EQ(0, in.setBuffer("#DWP 1.0 ascii\n(Property p string \"a\xff" "b\")\n"));
	std::unique_ptr<MleDwpItem> root(MleDwpItem::readAll(&in));
	ASSERT_TRUE(root.get() != NULL);
	EXPECT_EQ(0, in.getErrorCount());
	std::string out = writeString(root.get());
	EXPECT_NE(std::string::npos, out.find("a\xff" "b"));
}
