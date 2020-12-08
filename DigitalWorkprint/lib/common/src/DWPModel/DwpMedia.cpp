/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpMedia.cpp
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
#include <stdlib.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include <mle/DwpMedia.h>
#include <mle/DwpInput.h>
#include <mle/DwpOutput.h>
#include <mle/DwpFinder.h>

// ***** MleDwpMedia WPITEM SOURCE *****

MLE_DWP_SOURCE(MleDwpMedia, MleDwpItem);


MleDwpMedia::MleDwpMedia(void)
    :m_label(NULL),
     m_flags(0)
{
    // Call the creation notification callbacks for this type of dwp item.
    notify(MleDwpCreateNotification);
}


MleDwpMedia::~MleDwpMedia()
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

    // Call the notification callbacks for those who registered
    // general interest on all wp items (NULL key).
    m_notifier.notify(NULL, this, MleDwpDeleteNotification);

    if (m_label)
       mlFree(m_label);
}


void
MleDwpMedia::setFlags(const int value)
{
    // Set new value.
    m_flags = value;

    // Call the modification notification callbacks for this dwp item.
    notify(MleDwpChangeNotification);
}


void
MleDwpMedia::setFilename(const char *name)
{
    // Set new one.
    m_filename.setName(name);
    m_filename.setPath(NULL);

    // Call the modification notification callbacks for this dwp item.
    notify(MleDwpChangeNotification);
}


void
MleDwpMedia::setLabel(const char *name)
{
    // Free old one.
    if (m_label)
        mlFree(m_label);

    // Set new one
   m_label = name ? strdup(name) : NULL;

    // Call the modification notification callbacks for this dwp item.
    notify(MleDwpChangeNotification);
}


int
MleDwpMedia::getFlags(void) const
{
    return m_flags;
}


const char*
MleDwpMedia::getFilename(void) const
{
    return m_filename.getPath() ? m_filename.getPath() : m_filename.getName();
}

const char*
MleDwpMedia::getLabel(void) const
{
    return m_label;
}


int
MleDwpMedia::readContents(MleDwpInput *in)
{
    // Declare local variables.
    char buffer[MLE_DWP_INPUT_BUFSIZE];
    int num;
  
    if (in->readInt(&num))
        return 1;
    setFlags(num);
  
    if (in->readString(buffer))
        return 1;
    setLabel(buffer);

    if (in->readFilename(&m_filename))
        return 1;

    notify(MleDwpChangeNotification);

    return 0;
}


int
MleDwpMedia::writeContents(MleDwpOutput *out)
{
    out->writeInt(getFlags());
    out->writeString(getLabel());
    out->writeFilename(&m_filename);
    return 0;
}

MleDwpItem *
MleDwpMedia::copy(int copyChildren,MleDwpItem *parent)
{
	// Call the base class copy.
	MleDwpItem *rval = MleDwpItem::copy(copyChildren,parent);

	// If the filename has a path, then set the path on the copy.
	if ( m_filename.getPath() )
		((MleDwpMedia *)rval)->m_filename.setPath(m_filename.getPath());
	
	return rval;
}

void *
MleDwpMedia::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpMedia::operator delete(void *p)
{
	mlFree(p);
}
