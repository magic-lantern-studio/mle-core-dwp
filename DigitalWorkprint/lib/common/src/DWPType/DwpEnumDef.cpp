/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpEnumDev.cpp
 * @ingroup MleDWPType
 *
 * This file implements the enumeration definition used by the Magic Lantern Digital
 * Workprint Library API.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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
#if defined(WIN32) || defined(__linux__) || defined(__APPLE__)
#include <string.h>
#else
#include <strings.h>
#endif

// Include Digital Workprint header files.
#include "mle/DwpEnumDef.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"


MLE_DWP_SOURCE(MleDwpEnumDef,MleDwpItem);

MleDwpEnumDef::MleDwpEnumDef()
{
    m_enumStrings = NULL;
    m_length = 0;

    // Call the creation notification callbacks for this type of wp item.
    notify(MleDwpCreateNotification);
}

MleDwpEnumDef::~MleDwpEnumDef()
{
    // Call the notification callbacks for this specific wp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

    // Call the notification callbacks for those who registered
    // general interest on all wp items (NULL key).
    m_notifier.notify(NULL, this, MleDwpDeleteNotification);

    if (m_enumStrings) {
	    for (int i = 0; i < m_length; i++) {
	        free(m_enumStrings[i]);
		}
	    free(m_enumStrings);
    } 
}

int
MleDwpEnumDef::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	m_length = 0;
	for ( ;in->readString(buffer) == 0; )
	{
	    m_length++;
	    m_enumStrings = (char**) mlRealloc(m_enumStrings, m_length);
	    m_enumStrings[m_length-1] = strdup(buffer);
	}

	// call the modification notification callbacks for this wp item
	notify(MleDwpChangeNotification);

	return 0;
}

int
MleDwpEnumDef::writeContents(MleDwpOutput *out)
{
	for (int i = 0; i < m_length; i++) {
	    out->writeString(m_enumStrings[i]);
	}

	return 0;
}

const char*
MleDwpEnumDef::getString(int i)
{
    MLE_ASSERT(m_enumStrings);
    return(m_enumStrings[i]);
}

void *
MleDwpEnumDef::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpEnumDef::operator delete(void *p)
{
	mlFree(p);
}
