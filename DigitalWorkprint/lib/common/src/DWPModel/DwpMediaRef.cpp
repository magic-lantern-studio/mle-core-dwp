/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpMediaRef.cpp
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
#include "mle/DwpMediaRef.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
#include "mle/DwpFinder.h"
#include "mle/DwpMedia.h"
#include "mle/DwpMediaRefTarget.h"

// Include MAgic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"


#define MAX_ALLOWED_TARGETS 10


// ***** MleDwpMediaRef WPITEM SOURCE *****

MLE_DWP_SOURCE(MleDwpMediaRef, MleDwpItem);


MleDwpMediaRef::MleDwpMediaRef(void)
  : m_mediaRefClass(NULL)
{
    // Call the creation notification callbacks for this type of dwp item.
    notify(MleDwpCreateNotification);
}


MleDwpMediaRef::MleDwpMediaRef(char *mrname, char *mrclass)
  : m_mediaRefClass(mrclass)
{
    setName(mrname);

    // Call the creation notification callbacks for this type of dwp item.
    notify(MleDwpCreateNotification);
}


MleDwpMediaRef::~MleDwpMediaRef(void)
{
    // Check if we have a parent
    // XXX need to remove ourselves from parent before sending notify or
    // XXX MlMediaRefPropEditor will build new option menu with us still
    // XXX in it!!!!  --- gwu, August 13, 1996
    if ( m_parent )
	{
	    m_parent->removeChild(this);
    }

    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

    // Call the notification callbacks for those who registered
    // general interest on all wp items (NULL key).
    m_notifier.notify(NULL, this, MleDwpDeleteNotification);

    if (m_mediaRefClass)
        mlFree(m_mediaRefClass);
}


void
MleDwpMediaRef::setMediaRefClass(const char *filename)
{
    // Free old storage.
    if (m_mediaRefClass)
        mlFree(m_mediaRefClass);

    // Set new filename.
    m_mediaRefClass = filename ? strdup(filename) : NULL;

    // Call the modification notification callbacks for this dwp item.
    notify(MleDwpChangeNotification);

    // Call the modification notification callbacks for this TYPE of item (media ref).
    notify(MleDwpChangeNotification, (void *)getTypeId());
}


const char*
MleDwpMediaRef::getMediaRefClass(void) const
{
    return m_mediaRefClass;
}


int
MleDwpMediaRef::readContents(MleDwpInput *in)
{
    // Declare local variables.
    char buffer[MLE_DWP_INPUT_BUFSIZE];

    // Read MediaRef name.
    if (in->readString(buffer))
        return 1;
    setName(buffer);

    // Read MediaRef class.
    if (in->readString(buffer))
        return 1;
    setMediaRefClass(buffer);

    return 0;
}


int
MleDwpMediaRef::writeContents(MleDwpOutput *out)
{
    out->writeString(getName());
    out->writeString(getMediaRefClass());
    return 0;
}

int
MleDwpMediaRef::queryTargetEntries(MleDwpMediaRefTargetMedia **entries)
{
    // Declare local variables.
    MleDwpFinder targetFinder;
    MleDwpMediaRefTarget *target,**targetItems;
    MleDwpFinder mediaFinder;
    MleDwpMedia **mediaItems;
    int numItems;
    int retValue;

    // Retrieve MleDwpMRTarget items.
    targetFinder.setType(MleDwpMediaRefTarget::typeId);
    targetFinder.setFindAll(1);
    targetFinder.find(this);
    targetItems = (MleDwpMediaRefTarget **)targetFinder.getItems();
    numItems = targetFinder.getNumItems();

    // There should only be one match determined by the discriminators.
    if (numItems == 1)
	{
        target = targetItems[0];

        // retrieve MleDwpMedia items
        mediaFinder.setType(MleDwpMedia::typeId);
        mediaFinder.setFindAll(1);
        mediaFinder.find(target);
        mediaItems = (MleDwpMedia **)mediaFinder.getItems();
        numItems = mediaFinder.getNumItems();

        if (numItems > 0)
		{
            *entries = (MleDwpMediaRefTargetMedia *) mlMalloc(
                sizeof(MleDwpMediaRefTargetMedia) * numItems);

            for (int i = 0; i < numItems; i++)
			{
                (*entries)[i].m_flags = mediaItems[i]->getFlags();
                (*entries)[i].m_filename = (char *)mediaItems[i]->getFilename();
                (*entries)[i].m_label = (char *)mediaItems[i]->getLabel();
            }
        } else
		{
            *entries = NULL;
        }

        retValue = numItems;
    } else
	{
        *entries = NULL;
        retValue = -1;
    }

    return(retValue);
}

int
MleDwpMediaRef::releaseTargetEntries(MleDwpMediaRefTargetMedia **entries)
{
	if (*entries != NULL)
		mlFree(*entries);

	return 0;
}

void *
MleDwpMediaRef::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpMediaRef::operator delete(void *p)
{
	mlFree(p);
}

void *
_MleDwpMediaRefTargetMedia::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
_MleDwpMediaRefTargetMedia::operator delete(void *p)
{
	mlFree(p);
}
