/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpMediaRefTarget.cpp
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

// Include DWP header files.
#include "mle/DwpMediaRefTarget.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
#include "mle/DwpFinder.h"
#include "mle/DwpMedia.h"

// Include MAgic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"


#define MAX_ALLOWED_TARGETS 10


// ***** MleDwpMediaRefTarget WPITEM SOURCE *****

MLE_DWP_SOURCE(MleDwpMediaRefTarget, MleDwpItem);


MleDwpMediaRefTarget::MleDwpMediaRefTarget(void)
    :m_flags(0)
{
    // Call the creation notification callbacks for this type of dwp item.
    notify(MleDwpCreateNotification);
}


MleDwpMediaRefTarget::~MleDwpMediaRefTarget(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

    // Call the notification callbacks for those who registered
    // general interest on all wp items (NULL key)
    m_notifier.notify(NULL, this, MleDwpDeleteNotification);
}


void
MleDwpMediaRefTarget::setFlags(const int value)
{
    // Set new value.
    m_flags = value;

    // Call the modification notification callbacks for this dwp item.
    notify(MleDwpChangeNotification);
}


int
MleDwpMediaRefTarget::getFlags(void) const
{
    return m_flags;
}


int
MleDwpMediaRefTarget::readContents(MleDwpInput *in)
{
    // Declare local variables.
    int num;
  
    if (in->readInt(&num))
        return 1;
    setFlags(num);

    return 0;
}


int
MleDwpMediaRefTarget::writeContents(MleDwpOutput *out)
{
    out->writeInt(getFlags());
    return 0;
}


void *
MleDwpMediaRefTarget::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpMediaRefTarget::operator delete(void *p)
{
	mlFree(p);
}
