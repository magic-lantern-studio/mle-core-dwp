/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API */

/**
 * @file DwpRoleAttachment.cpp
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

// Include Digital Workprint header files
#include "mle/DwpActor.h"
#include "mle/DwpRoleAttachment.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"


MLE_DWP_SOURCE(MleDwpRoleAttachment,MleDwpItem);

MleDwpRoleAttachment::MleDwpRoleAttachment(void) : m_parentIndex(0)
{
	/* Initialize variables. */
	m_parent = NULL;
	m_child = NULL;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpRoleAttachment::~MleDwpRoleAttachment()
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	/* Free storage. */
	delete m_parent;
	delete m_child;
}

int
MleDwpRoleAttachment::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	/* Read the parent name. */
	if ( in->readString(buffer) )
	{
		in->reportError("error reading RoleAttachment parent");
		in->skip();
		return 1;
	}
	setParent(buffer);

	/* Read the child name. */
	if ( in->readString(buffer) )
	{
		in->reportError("error reading RoleAttachment child");
		in->skip();
		return 1;
	}
	setChild(buffer);

	return 0;
}

int
MleDwpRoleAttachment::writeContents(MleDwpOutput *out)
{
	/* Write out parent and child. */
	out->writeString(m_parent ? m_parent : "NULL");
	out->writeString(m_child ? m_child : "NULL");

	return 0;
}

#if 0
int
MleDwpRoleAttachment::writeContents(MleDppActorGroupOutput *out)
{
    // Write this out only once; it could be embedded in the actor instance.
	if (m_parentIndex > 0) {
		out->writeOpcode(m_parentRoleOpcode);
		out->writeIndex(m_parentIndex); 
		m_parentIndex = 0;
	}
	return 0;
}
#endif /* 0 */

void
MleDwpRoleAttachment::setParent(const char *p)
{
	delete m_parent;

	m_parent = p ? strcpy(new char[strlen(p) + 1],p) : NULL;
}

void
MleDwpRoleAttachment::setChild(const char *c)
{
	delete m_child;

	m_child = c ? strcpy(new char[strlen(c) + 1],c) : NULL;
}

const char *
MleDwpRoleAttachment::getParent(void) const
{
	return m_parent;
}

const char *
MleDwpRoleAttachment::getChild(void) const
{
	return m_child;
}

void *
MleDwpRoleAttachment::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpRoleAttachment::operator delete(void *p)
{
	mlFree(p);
}
