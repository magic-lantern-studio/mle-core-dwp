/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpMediaRefClass.cpp
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
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
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

//Include Digital Workprint header files.
#include "mle/DwpMediaRefClass.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
#include "mle/DwpDatatype.h"

MLE_DWP_SOURCE(MleDwpMediaRefClass,MleDwpItem);

MleDwpMediaRefClass::MleDwpMediaRefClass(void)
{
	m_className = NULL;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpMediaRefClass::~MleDwpMediaRefClass()
{
	// call the notification callbacks for this specific dwp item.
	m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	if (m_className) mlFree(m_className);
}

int
MleDwpMediaRefClass::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	/* read the prop name */
	if ( in->readString(buffer) )
	{
		in->reportError("can't read MediaRefClass's property name");
		in->skip();
		return 1;
	}
	setName(buffer);

	/* read the media ref class name */
	if ( in->readString(buffer) )
	{
		in->reportError("can't read MediaRefClass's property class name");
		in->skip();
		return 1;
	}
	setClassName(buffer);

	return 0;
}

void
MleDwpMediaRefClass::setClassName(const char *t)
{

	if (m_className) mlFree(m_className);

	m_className = NULL;
	if (t) m_className = strdup(t);

	// call the modification notification callbacks for this wp item
	notify(MleDwpChangeNotification);
}

const char *
MleDwpMediaRefClass::getClassName(void) const
{
	return m_className;
}

int
MleDwpMediaRefClass::writeContents(MleDwpOutput *out)
{
	out->writeString(getName());
	out->writeString(getClassName());
	return 0;
}

void *
MleDwpMediaRefClass::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpMediaRefClass::operator delete(void *p)
{
	mlFree(p);
}
