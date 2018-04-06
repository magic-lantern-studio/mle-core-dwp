// COPYRIGHT_BEGIN
//
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

// Include Digital Workprint header files.
#include "mle/Dwp.h"
#include "mle/DwpItem.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"

//#include "libDWP.h"

extern void mleDwpInit(void);

void printUsage(char *name)
{
	printf("Usage: %s %s", name, "<dwp_file>\n");
}

int main(int argc, char **argv)
{
	int status = FALSE;

	// Parse the input arguments.
	if ( argc < 2 )
	{
		printUsage(argv[0]);
		return -1;
	}

	// Initialize the Digital Workprint API.
	mleDwpInit();

	// Create a new DWP Input item.
	MleDwpInput *in = new MleDwpInput;

	// Configure input item to read Digital Workprint from file.
	status = in->openFile(argv[1]);
	if (status)
	{
		printf("Error: Unable to open Digital Workprint %s.\n", argv[1]);
		return -1;
	}

    // Read the Digital Workprint.
    MleDwpItem *item = MleDwpItem::readAll(in);

    // Delete the DWP Input item.
    delete in;

    // Create a new DWP Output item.
    MleDwpOutput *out = new MleDwpOutput;

    // Set output target to stdout.
    out->setFilePointer(stdout);

    // Write the Digital Workprint.
    status = item->write(out);
    if (status) printf("Error: An error occurred while writing Digital Workprint.\n");

    // Delete the DWP Output item.
    delete out;

    // Delete the Digital Workprint.
    delete item;

    return 0;
}
