/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpFile.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the File workprint item.
 *
 * @author Mark S. Millard
 * @date July 10, 2003
 */

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
#include <string.h>

// Inlclude Magic Lantern header files.
#include "mle/DwpFile.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"

#include "mle/mlAssert.h"
#include "mle/mlMalloc.h"


// Declare the mandatory source macro.
MLE_DWP_SOURCE(MleDwpFile,MleDwpItem);


MleDwpFile::MleDwpFile(void)
{
	// Initialize path.
	m_path = NULL;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}


MleDwpFile::~MleDwpFile()
{
    // Call the notification callbacks for this specific wp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all dwp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	// Free memory.
	if ( m_path )
		delete m_path;
}


int
MleDwpFile::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	// Read in the filename and use the name field for it.
	if ( in->readString(buffer) )
	{
		in->reportError("error reading file name");
		in->skip();
		return 1;
	}
	setName(buffer);

	// Open the file whose name is in buffer.
	char *errString = NULL;
	if ( in->openFile(buffer) )
	{
		errString = new char[strlen(buffer) + 128];
		sprintf(errString,"error opening included file %s",buffer);
	}
	else
	{
		// Set the path parameter.
		setPath(in->getPath());

		// Add the contents to this item as children.
		while ( read(in,this) )
			; // Do nothing loop.
	}
	
	// Close the file.
	in->closeFile();

	if ( errString )
	{
		in->reportError(errString);
		delete errString;
		return 1;
	}
	else
		return 0;
}


int
MleDwpFile::write(MleDwpOutput *out,int writeChildren)
{
	MLE_ASSERT(out);

	int rval = 0;	// Return value.

	// Write the opening paren and item type.
	out->writeString("( File");

	// Write the filename.
	out->writeString(getName());
	
	out->adjustIndent(1);

	if ( writeChildren )
	{
		// Create an output object for the new file.
		// Use the path if it exists, else use the filename.
		MleDwpOutput fileOut;
		if ( !fileOut.openFile(getPath() ? getPath() : getName()) )
		{
			MleDwpItem *child = getFirstChild();
			while ( child )
			{
				child->write(&fileOut,writeChildren);
				child = child->getNext();
			}
		}
		else
		{
			printf("couldn't open file %s for output.\n",
				getPath() ? getPath() : getName());
			rval = 1;
		}
	}
	
	// Write the closing paren.
	out->adjustIndent(-1);
	out->writeString(")");
	out->writeTerminal();

	return rval;
}


/*
 * This function sets the path to write to.  The path is not something
 * that gets written out to the workprint.  It is an interpretation
 * of the filename (accessed with {get,set}Name()), which is written
 * to the workprint.  The path provides a more complete file
 * specification which can be left deliberately vague in the filename.
 */
void
MleDwpFile::setPath(const char *p)
{
	if ( m_path )
		delete m_path;
	
	m_path = p ? strcpy(new char[strlen(p) + 1],p) : NULL;

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}


/*
 * This function retrieves the path.
 */
const char *
MleDwpFile::getPath(void) const
{
	return m_path;
}


void *
MleDwpFile::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpFile::operator delete(void *p)
{
	mlFree(p);
}
