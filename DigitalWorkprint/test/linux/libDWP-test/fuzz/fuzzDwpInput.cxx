/**
 * @file fuzzDwpInput.cxx
 *
 * A fuzz target for the Digital Workprint reader. It is built with libFuzzer
 * (clang -fsanitize=fuzzer) or, without it, with standaloneDriver.cxx, which
 * replays files and can mutate them at random.
 *
 * Each input is read as a workprint from memory. What was read is then
 * written out, copied and checked, since those walk the same tree and a
 * reader that builds an odd tree can break them. Any crash, hang or memory
 * error is a defect.
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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <mle/Dwp.h>
#include <mle/DwpChecker.h>
#include <mle/DwpInclude.h>
#include <mle/DwpInput.h>
#include <mle/DwpItem.h>
#include <mle/DwpOutput.h>
#include <mle/mlMalloc.h>

extern void mleDwpInit(void);

extern "C" int LLVMFuzzerInitialize(int *, char ***)
{
	mleDwpInit();

	// Writing an Include writes its file, which would overwrite any file
	// that an input names.
	MleDwpInclude::setClassWriteChildren(MleDwpInclude::WRITE_NONE);
	return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	// The input is text, so it ends at the first NUL byte.
	std::string text((const char *)data, size);

	// An Include reads the file that the input names, which is not what is
	// being tested and can block (a named pipe) or be very large.
	if (text.find("Include") != std::string::npos)
		return 0;

	MleDwpInput in;
	if (in.setBuffer(text.c_str()))
		return 0;
	MleDwpItem *root = MleDwpItem::readAll(&in);
	if (! root)
		return 0;

	// Write the tree out.
	MleDwpOutput out;
	root->write(&out);
	mlFree(out.getBuffer());

	// Copy it, which writes it out and reads it back.
	MleDwpItem *copy = root->copy(1);
	delete copy;

	// Check it.
	MleDwpChecker checker(root);
	checker.check();

	delete root;
	return 0;
}
