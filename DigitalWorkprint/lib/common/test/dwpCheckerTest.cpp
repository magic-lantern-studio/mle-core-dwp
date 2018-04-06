/** @defgroup MleDWPTest Magic Lantern Digital Workprint Library API - Test */

/**
 * @file dwpCheckerTest.cpp
 * @ingroup MleDWPTest
 *
 * This program reads a digital workprint and runs the MleDwpChecker on it to 
 * test for workprint validity.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlErrno.h"

// Inlclude Digital Wrokprint header files.
#include "mle/Dwp.h"
#include "mle/DwpItem.h"
#include "mle/DwpInput.h"
#include "mle/DwpChecker.h"

extern void mleDwpInit(void);


int main(int argc,char **argv)
{
	// Verify the minimal number of arguments to run.
    if ( argc < 2 ) 
    {
		printf("Usage is 'DWPChecker <dwpfile>'\n");
		return 1;
    }

	// Initialize the DWP API.
    mleDwpInit();

    for (int i = 1; i < argc; i++)
    {
		// Open the DWP.
		MleDwpInput in;
		if (in.openFile(argv[i]))
		{
			printf("dwpCheckerTest: can't open file '%s'\n", argv[i]);
			return 1;
		}
    
		// Read the DWP.
		MleDwpItem *root = new MleDwpItem;
		while ( MleDwpItem::read(&in,root) )
			;
		
		// Check the semantics of the DWP.
		MleDwpChecker checker(root);
		MlErr err = checker.check();
		if (err)
		{
			printf("DwpChecker: file '%s': %s\n",
			argv[i], checker.getErrorMsg());
			return err;
		}
    }

    return 0;
}
