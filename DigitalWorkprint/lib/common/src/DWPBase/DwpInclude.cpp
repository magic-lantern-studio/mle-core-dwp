/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpInclude.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the file inclusion used by the Magic Lantern Digital
 * Workprint Library API.
 *
 * @author Mark S. Millard
 * @date July 14, 2003
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

// Include Magic Lantern header files.
#include "mle/DwpInclude.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"

#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"


MLE_DWP_SOURCE(MleDwpInclude,MleDwpItem);


// This static class variable globally enables writing out children
// into the file specified (instead of just writing out the Include
// directive into the current file.
MleDwpInclude::Enabling MleDwpInclude::g_globalWriteEnable = MleDwpInclude::DEFAULT;


MleDwpInclude::MleDwpInclude(void)
{
	// Initialize path.
	m_path = NULL;

	m_localWriteEnable = DEFAULT;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}


MleDwpInclude::~MleDwpInclude()
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	// Free memory.
	if ( m_path )
		delete m_path;
}


int
MleDwpInclude::readContents(MleDwpInput *in)
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
			;

		// Close the file.
		in->closeFile();
	}
	
	if ( errString )
	{
		in->reportError(errString);
		delete errString;
		return 1;
	}
	else
		return 0;
}


// Populate the subtree under us with the current data from
// the file. Current children are deleted if they exist, and subtree is
// populated. This allows updating of included files, as well as populating
// an include subtree after constructing the include node.
int
MleDwpInclude::populate()
{
    // If a subtree exists, delete it.
    MleDwpItem *child;
    while ((child = getFirstChild()) != NULL)
    {
	    removeChild(child);
	    delete child;
    }
    
    char *errString = NULL;
    MleDwpInput in;
    if ( in.openFile(getName()) )
    {
	    errString = new char[strlen(getName()) + 128];
	    sprintf(errString,"error opening included file %s", getName());
    }
    else
    {
	    // Add the contents to this item as children.
	    while ( read(&in,this) )
	        ;
    }

    if ( errString )
    {
	    // XXX should 'in' be a ptr passed by caller, to get his error
	    // report back?
	    in.reportError(errString);
	    delete errString;
	    return 1;
    }

    return 0;
}


int
MleDwpInclude::write(MleDwpOutput *out,int writeChildren)
{
	MLE_ASSERT(out);

	int rval = 0;	// Return value.

// This macro returns the global write flag unless it is DEFAULT, in which
// case it returns the local write flag
#define WRITEFLAG \
	(m_localWriteEnable == DEFAULT ? g_globalWriteEnable : m_localWriteEnable)

	// Write the opening paren and item type.
	if ( WRITEFLAG == WRITE_CURRENT )
	{
		rval |= out->writeString("( Container");
		rval |= out->writeTerminal();
	}
	else
	{
		rval |= out->writeString("( Include");

		// Write the filename.
		rval |= out->writeString(getName());
	}
	
	out->adjustIndent(1);

	if ( writeChildren )
	{
	    switch ( WRITEFLAG )
	    {
		case DEFAULT:
		case WRITE_FILE:
		{
		    // Create an output object for the new file
		    // use the path if it exists, else use the filename.
		    MleDwpOutput fileOut;
		    if ( !fileOut.openFile(getPath() ? getPath() : getName()) )
		    {
			    MleDwpItem *child = getFirstChild();
			    while ( child )
				{
				    rval |= child->write(&fileOut,writeChildren);
				    child = child->getNext();
				}
		    }
		    else
		    {
			    printf("couldn't open file %s for output.\n",
				    getPath() ? getPath() : getName());
			 rval |= 1;
		    }
		}
		    break;
		case WRITE_CURRENT:
		{
		    // just write the children
		    MleDwpItem *child = getFirstChild();
		    while ( child )
		    {
		    	rval |= child->write(out,writeChildren);
		    	child = child->getNext();
		    }
		}
		    break;
		case WRITE_NONE:
		    // don't write
		    break;
	    }
	}
	
	// Write the closing paren.
	out->adjustIndent(-1);
	rval |= out->writeString(")");
	rval |= out->writeTerminal();

	return rval;
}


//	This function sets the path to write to.  The path is not something
//	that gets written out to the workprint.  It is an interpretation
//	of the filename (accessed with {get,set}Name()), which is written
//	to the workprint.  The path provides a more complete file
//	specification which can be left deliberately vague in the filename.
void
MleDwpInclude::setPath(const char *p)
{
	if ( m_path )
		delete m_path;
	
	m_path = p ? strcpy(new char[strlen(p) + 1],p) : NULL;

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}


// This function retrieves the path.
const char *
MleDwpInclude::getPath(void) const
{
	return m_path;
}


MleDwpItem *
MleDwpInclude::copy(int copyChildren,MleDwpItem *parent)
{
	// Create a new Include item.
	MleDwpInclude *rval = new MleDwpInclude;

	// Attach to parent.
	if ( parent )
		parent->insertChild(rval);
	
	// Set the file.
	rval->setName(getName());

	// Set the path.
	rval->setPath(getPath());

	// Set instance writing.
	rval->setInstanceWriteChildren(getInstanceWriteChildren());

	// Copy the children if requested.
	if ( copyChildren )
	{
		MleDwpItem *child = getFirstChild();
		while ( child )
		{
			child->copy(copyChildren,rval);
			child = child->getNext();
		}
	}

	// Copy the discriminator if item not being added to a parent.
	// This will also set discrim on all children.
	if (!parent)
	    rval->setDiscriminator(this->getDiscriminator());

	// Call the modification notification callbacks for this dwp item.
	rval->notify(MleDwpChangeNotification);

	return rval;
}


void
MleDwpInclude::setInstanceWriteChildren(MleDwpInclude::Enabling flag)
{
	m_localWriteEnable = flag;
}


MleDwpInclude::Enabling
MleDwpInclude::getInstanceWriteChildren(void) const
{
	return m_localWriteEnable;
}


void
MleDwpInclude::setClassWriteChildren(MleDwpInclude::Enabling flag)
{
	g_globalWriteEnable = flag;
}


MleDwpInclude::Enabling
MleDwpInclude::getClassWriteChildren(void)
{
	return g_globalWriteEnable;
}

void *
MleDwpInclude::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpInclude::operator delete(void *p)
{
	mlFree(p);
}
