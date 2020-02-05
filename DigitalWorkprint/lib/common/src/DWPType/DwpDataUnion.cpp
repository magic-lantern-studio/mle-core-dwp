/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpDataUnion.cpp
 * @ingroup MleDWPType
 *
 * This file implements the data type union used by the Magic Lantern Digital
 * Workprint Library API.
 *
 * @author Mark S. Millard
 * @date February 6, 2004
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Inlclude Digital Workprint header files.
#include "mle/DwpDataUnion.h"
#include "mle/DwpDatatype.h"


MleDwpDataUnion::MleDwpDataUnion(void)
{
	// Initialize variables.
	m_u.v = NULL;

	m_datatype = NULL;
}

MleDwpDataUnion::~MleDwpDataUnion()
{
	if ( m_datatype )
		m_datatype->release(this);
}

void
MleDwpDataUnion::setDatatype(const MleDwpDatatype *d)
{
	// If the new datatype is the same as the old, do nothing.
	if ( d == m_datatype )
		return;

	// Release the old data.
	if ( m_datatype )
		m_datatype->release(this);

	// Make sure no pointers are left around.
	m_u.v = NULL;

	// Set the new datatype.
	m_datatype = d;
}

void *
MleDwpDataUnion::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpDataUnion::operator delete(void *p)
{
	mlFree(p);
}
