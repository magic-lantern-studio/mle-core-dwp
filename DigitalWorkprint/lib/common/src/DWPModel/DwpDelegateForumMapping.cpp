/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpDelegateForumMapping.cpp
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @deprecated The MleDwpDelegateForumMapping class has been replaced with the
 * MleDwpRoleSetMapping class.
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
#include "mle/DwpDelegateForumMapping.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"
//#include "mle/DwpTblMgr.h"


MLE_DWP_SOURCE(MleDwpDelegateForumMapping,MleDwpItem);

MleDwpDelegateForumMapping::MleDwpDelegateForumMapping(void)
{
	m_forum = NULL;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpDelegateForumMapping::~MleDwpDelegateForumMapping()
{
    // Call the notification callbacks for this specific wp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	mlFree(m_forum);
}

const char *
MleDwpDelegateForumMapping::getForum(void) const
{
	return m_forum;
}

void
MleDwpDelegateForumMapping::setForum(const char *f)
{
	// Free the old forum name.
	if ( m_forum )
		mlFree(m_forum);

	// Set the new forum
	m_forum = f ? strdup(f) : NULL;

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

int
MleDwpDelegateForumMapping::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	if ( in->readString(buffer) )
	{
		in->reportError("error reading delegate class");
		in->skip();
		return 1;
	}
	setName(buffer);

	if ( in->readString(buffer) )
	{
		in->reportError("error reading forum name");
		in->skip();
		return 1;
	}
	setForum(buffer);
		
	return 0;
}

int
MleDwpDelegateForumMapping::writeContents(MleDwpOutput *out)
{
	out->writeString(getName());
	out->writeString(getForum());
	return 0;
}

void *
MleDwpDelegateForumMapping::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpDelegateForumMapping::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpDelegateForumMapping::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpDelegateForumMapping::operator delete[](void* p)
{
	mlFree(p);
}
