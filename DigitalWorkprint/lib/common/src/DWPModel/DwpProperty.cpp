/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpProperty.cpp
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

#include "mle/DwpProperty.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"
//#include "mle/DwpActor.h"
//#include "mle/DwpActorDef.h"
//#include "mle/DwpActorTemplate.h"
//#include "mle/DwpForum.h"
//#include "mle/DwpForumDef.h"
//#include "mle/DwpPropertyDef.h"
//#include "mle/DwpFinder.h"
#include "mle/DwpDatatype.h"

// Include Runtime Player header files.
//#include "mle/actorclass.h"


MLE_DWP_SOURCE(MleDwpProperty,MleDwpItem);

MleDwpProperty::MleDwpProperty(void)
{
	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpProperty::~MleDwpProperty(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);
}

int
MleDwpProperty::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	if ( in->readString(buffer) )
	{
		in->reportError("error reading Property name");
		in->skip();
		return 1;
	}
	setName(buffer);

	if ( in->readString(buffer) )
	{
		in->reportError("error reading Property type");
		in->skip();
		return 1;
	}
	m_data.setDatatype(MleDwpDatatype::findType(buffer));
	
	if ( m_data.m_datatype == NULL )
	{
		char *msg = new char[1024 + strlen(buffer)];
		sprintf(msg,"unknown datatype %s",buffer);
		in->reportError(msg);
		in->skip();
		delete msg;
		return 1;
	}

	m_data.m_datatype->read(in,&m_data);

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);

	return 0;
}

int
MleDwpProperty::writeContents(MleDwpOutput *out)
{
	// Write the property name.
	out->writeString(getName());

	// Make sure we have a datatype.
	if ( m_data.m_datatype == NULL )
	{
		fprintf(stderr,"no property datatype for %s.\n",getName());
		return 1;
	}

	// Write the datatype name.
	out->writeString(m_data.m_datatype->getName());

	// Write the data.
	m_data.m_datatype->write(out,&m_data);

	return 0;
}

#if 0
int
MleDwpProperty::writeContents(MleDppActorGroupOutput *out)
{
	if ( m_data.m_datatype == NULL )
		return 1;
    
	out->currentProperty = this->getName(); // This is used later to find the property offset.
	m_data.m_datatype->write(out,&m_data);

	return 0;
}
#endif /* 0 */

void *
MleDwpProperty::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpProperty::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpProperty::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpProperty::operator delete[](void* p)
{
	mlFree(p);
}

const MleDwpDatatype *
MleDwpProperty::getDatatype(void) const
{
	return m_data.m_datatype;
}

void
MleDwpProperty::setDatatype(const MleDwpDatatype *datatype)
{
	m_data.setDatatype(datatype);

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

void
MleDwpProperty::setDatatype(const char *datatype)
{
	setDatatype(MleDwpDatatype::findType(datatype));
}
