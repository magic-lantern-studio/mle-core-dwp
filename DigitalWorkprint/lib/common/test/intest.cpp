/** @defgroup MleDWPTest Magic Lantern Digital Workprint Library API - Test */

/**
 * @file intest.cpp
 * @ingroup MleDWPTest
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
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
#include "mle/DwpActor.h"

extern void mleDwpInit(void);


int main(int argc,char **argv)
{
	// Parse the input arguments.
	if ( argc < 2 ) return 0;

	// Initialize the Digital Workprint API.
	mleDwpInit();

	// Create a new DWP Input item.
	MleDwpInput *in = new MleDwpInput;

// Test input reading from a buffer or (default) directly from the file.
#define XINPUTBUF
#ifdef INPUTBUF
{
	char inbuf[65536];

	int n = 0;
	int c;
	FILE *fp = fopen(argv[1],"r");
	while ( (c = getc(fp)) != EOF )
		inbuf[n++] = c;
	fclose(fp);

	printf("%d characters read from %s.\n",n,argv[1]);
	in->setBuffer(inbuf);
}
#else
	in->openFile(argv[1]);
#endif

	// Read the Digital Workprint.
	MleDwpItem *item = MleDwpItem::readAll(in);
	
	// Delete the DWP Input item.
	delete in;

	// Create a new DWP Output item.
	MleDwpOutput *out = new MleDwpOutput;

// Test writing to a memory buffer.
#define XOUTPUTBUF
#ifdef OUTPUTBUF
	out->setBuffer(NULL,0);
#else
	out->setFilePointer(stdout);
#endif

	int status = 0;

	// Write the Digital Workprint.
	status = item->write(out);

	if ( status )
		printf("Error writing workprint.\n");

#ifdef OUTPUTBUF
	char *outbuf;
	int outsize;
	out->getBuffer(&outbuf,&outsize);

	printf("%d bytes written.\n",outsize);
	printf("%s",outbuf);
#endif

	// Delete the DWP Output item.
	delete out;

	// Remove the digital Workprint.
	delete item;

	return 0;
}

