/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpDelegateBinding.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 *
 * @deprecated The MleDwpDelegateBinding class has been replaced with the
 * MleDwpRoleBinding class.
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
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

#include "mle/DwpDelegateBinding.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"
//#include "mle/DwpTblMgr.h"

MLE_DWP_SOURCE(MleDwpDelegateBinding,MleDwpItem);

MleDwpDelegateBinding::MleDwpDelegateBinding(void)
{
	m_forum = NULL;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpDelegateBinding::~MleDwpDelegateBinding()
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	mlFree(m_forum);
}

const char *
MleDwpDelegateBinding::getForum(void) const
{
	return m_forum;
}

void
MleDwpDelegateBinding::setForum(const char *f)
{
	// Free the old forum name.
	if ( m_forum )
		mlFree(m_forum);

	// Set the new forum.
	m_forum = f ? strdup(f) : NULL;

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

int
MleDwpDelegateBinding::readContents(MleDwpInput *in)
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
MleDwpDelegateBinding::writeContents(MleDwpOutput *out)
{
	out->writeString(getName());
	out->writeString(getForum());
	return 0;
}

void *
MleDwpDelegateBinding::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpDelegateBinding::operator delete(void *p)
{
	mlFree(p);
}

#if 0
int
MleDwpDelegateBinding::writeContents(MleDppActorGroupOutput *out)
{
	// Write out the forum id.
	out->writeOpcode(setForumOpcode);
	int forumNumber = out->table->forumRuntimeName(getForum());
	out->writeIndex(forumNumber);

	// Write out the delegate class id.
	out->writeOpcode(bindDelegateOpcode);
	int delegateClassNumber =
		out->table->delegateClassRuntimeName(getName()); 
	out->writeIndex(delegateClassNumber);
	return 0;
}
#endif /* 0 */
