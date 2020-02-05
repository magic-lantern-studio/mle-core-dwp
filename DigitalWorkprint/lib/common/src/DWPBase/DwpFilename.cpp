/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpFilename.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the filename object used by the Magic Lantern Digital
 * Workprint Library API.
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include <mle/DwpFilename.h>


MleDwpFilename::MleDwpFilename(void)
{
	// Initialize member variables.
	m_name = NULL;
	m_path = NULL;
}


MleDwpFilename::~MleDwpFilename(void)
{
	// Deallocate memory.
	if ( m_name ) delete m_name;
	if ( m_path ) delete m_path;
}


// This function sets the file name.  The file name may be a relative or
// and absolute file specifier.
void
MleDwpFilename::setName(const char *n)
{
	// Free storage if it already exists.
	if ( m_name )
		delete m_name;
	
	// Allocate and copy the string.
	m_name = n ? strcpy(new char[strlen(n) + 1],n) : NULL;
}


// This function sets the file path.
void
MleDwpFilename::setPath(const char *p)
{
	// Free storage if it already exists.
	if ( m_path )
		delete m_path;
	
	// Allocate and copy the string.
	m_path = p ? strcpy(new char[strlen(p) + 1],p) : NULL;
}

void *
MleDwpFilename::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpFilename::operator delete(void *p)
{
	mlFree(p);
}
