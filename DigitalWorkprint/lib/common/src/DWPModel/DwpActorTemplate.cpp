/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpActorTemplate.cpp
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
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
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include "mle/DwpActorTemplate.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DwpTblMgr.h"

// Include Runtime Player header files.
//#include "mle/actor.h"
//#include "mle/actorclass.h"


MLE_DWP_SOURCE(MleDwpActorTemplate,MleDwpItem);

MleDwpActorTemplate::MleDwpActorTemplate(void)
{
	m_actorClass = NULL;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpActorTemplate::~MleDwpActorTemplate()
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	if ( m_actorClass )
		mlFree(m_actorClass);
}

void
MleDwpActorTemplate::setActorClass(const char *ac)
{
	/* Get rid of the old actor class name. */
	if ( m_actorClass )
		mlFree(m_actorClass);
	
	/* Set the new actor class. */
	m_actorClass = ac ? strdup(ac) : NULL;

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

const char *
MleDwpActorTemplate::getActorClass(void) const
{
	return m_actorClass;
}

int
MleDwpActorTemplate::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	if ( in->readString(buffer) )
	{
		in->reportError("error reading Actor name");
		in->skip();
		return 1;
	}
	setName(buffer);

	if ( in->readString(buffer) )
	{
		in->reportError("error reading Actor class");
		in->skip();
		return 1;
	}
	setActorClass(buffer);

	return 0;
}

int
MleDwpActorTemplate::writeContents(MleDwpOutput *out)
{
	out->writeString(getName());
	out->writeString(getActorClass());

	return 0;
}

void *
MleDwpActorTemplate::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpActorTemplate::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpActorTemplate::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpActorTemplate::operator delete[](void* p)
{
	mlFree(p);
}
