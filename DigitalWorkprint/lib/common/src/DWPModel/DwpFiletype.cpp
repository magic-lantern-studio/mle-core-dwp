/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpFiletype.cpp
 * @ingroup MleDWPModel
 *
 * This file defines the File workprint item.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2025 Wizzer Works
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
#include <stdlib.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include <mle/DwpFiletype.h>
#include <mle/DwpInput.h>
#include <mle/DwpOutput.h>
#include <mle/DwpFinder.h>

// ***** MleDwpFiletype WPITEM SOURCE *****

MLE_DWP_SOURCE(MleDwpFiletype, MleDwpItem);


MleDwpFiletype::MleDwpFiletype(void)
     : m_filetype(NULL)
{
    // Call the creation notification callbacks for this type of wp item.
    notify(MleDwpCreateNotification);
}


MleDwpFiletype::~MleDwpFiletype()
{
    if (m_filetype)
       mlFree(m_filetype);

    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);
}


const char*
MleDwpFiletype::getFiletype(void) const
{
    return m_filetype;
}

void
MleDwpFiletype::setFiletype(const char *buffer)
{
    m_filetype = strdup(buffer);
}

int
MleDwpFiletype::readContents(MleDwpInput *in)
{
    // Declare local variables.
    char buffer[MLE_DWP_INPUT_BUFSIZE];
  
    if (in->readString(buffer))
        return 1;
    setFiletype(buffer);

    notify(MleDwpChangeNotification);

    return 0;
}

int
MleDwpFiletype::writeContents(MleDwpOutput *out)
{
    out->writeString(getFiletype());
    return 0;
}

void *
MleDwpFiletype::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpFiletype::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpFiletype::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
MleDwpFiletype::operator delete[](void* p)
{
    mlFree(p);
}
