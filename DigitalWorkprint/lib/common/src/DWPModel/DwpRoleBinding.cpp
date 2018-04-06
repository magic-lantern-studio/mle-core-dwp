/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpRoleBinding.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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

#include "mle/DwpRoleBinding.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"
//#include "mle/DwpTblMgr.h"

MLE_DWP_SOURCE(MleDwpRoleBinding,MleDwpItem);

MleDwpRoleBinding::MleDwpRoleBinding(void)
{
	m_set = NULL;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpRoleBinding::~MleDwpRoleBinding()
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	mlFree(m_set);
}

const char *
MleDwpRoleBinding::getSet(void) const
{
	return m_set;
}

void
MleDwpRoleBinding::setSet(const char *f)
{
	// Free the old set name.
	if ( m_set )
		mlFree(m_set);

	// Set the new set.
	m_set = f ? strdup(f) : NULL;

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

int
MleDwpRoleBinding::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	if ( in->readString(buffer) )
	{
		in->reportError("error reading role class");
		in->skip();
		return 1;
	}
	setName(buffer);

	if ( in->readString(buffer) )
	{
		in->reportError("error reading set name");
		in->skip();
		return 1;
	}
	setSet(buffer);
		
	return 0;
}

int
MleDwpRoleBinding::writeContents(MleDwpOutput *out)
{
	out->writeString(getName());
	out->writeString(getSet());
	return 0;
}

void *
MleDwpRoleBinding::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpRoleBinding::operator delete(void *p)
{
	mlFree(p);
}

#if 0
int
MleDwpRoleBinding::writeContents(MleDppActorGroupOutput *out)
{
	// Write out the set id.
	out->writeOpcode(setForumOpcode);
	int setNumber = out->table->setRuntimeName(getForum());
	out->writeIndex(setNumber);

	// Write out the role class id.
	out->writeOpcode(bindRoleOpcode);
	int roleClassNumber =
		out->table->roleClassRuntimeName(getName()); 
	out->writeIndex(roleClassNumber);
	return 0;
}
#endif /* 0 */
