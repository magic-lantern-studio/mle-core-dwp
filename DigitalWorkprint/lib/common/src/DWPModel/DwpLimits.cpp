/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpLimits.cpp
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

// Include Digital Workprint header files.
#include "mle/DwpLimits.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"
#include "mle/DwpDatatype.h"

MLE_DWP_SOURCE(MleDwpLimits,MleDwpItem);

MleDwpLimits::MleDwpLimits(void)
{
	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpLimits::~MleDwpLimits(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Dall the notification callbacks for those who registered
	// general interest on all wp items (NULL key)
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);
}

int
MleDwpLimits::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	if ( in->readString(buffer) )
	{
		in->reportError("error reading Limits type");
		in->skip();
		return 1;
	}
	m_minData.setDatatype(MleDwpDatatype::findType(buffer));
	m_maxData.setDatatype(MleDwpDatatype::findType(buffer));
	
	if ( m_minData.m_datatype == NULL || m_maxData.m_datatype == NULL )
	{
		char *msg = new char[1024 + strlen(buffer)];
		sprintf(msg,"unknown datatype %s",buffer);
		in->reportError(msg);
		in->skip();
		delete msg;
		return 1;
	}

	if (m_minData.m_datatype->read(in,&m_minData))
	{
	    char buf[500];
	    strcpy(buf,"Can not read min limit");
	    in->reportError(buf);
	    in->skip();
	    return 1;
	}

	if (m_maxData.m_datatype->read(in,&m_maxData))
	{
	    char buf[500];
	    strcpy(buf,"Can not read max limit");
	    in->reportError(buf);
	    in->skip();
	    return 1;
	}

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);

	return 0;
}

int
MleDwpLimits::writeContents(MleDwpOutput *out)
{
	// Make sure we have a datatype.
	if ( m_minData.m_datatype == NULL  || m_maxData.m_datatype == NULL)
	{
		fprintf(stderr,"no property datatype for %s.\n",getName());
		return 1;
	}

	// Write the datatype name.
	out->writeString(m_minData.m_datatype->getName());

	// Write the data
	m_minData.m_datatype->write(out,&m_minData);
	m_maxData.m_datatype->write(out,&m_maxData);

	return 0;
}

const MleDwpDatatype *
MleDwpLimits::getDatatype(void) const
{
	return m_minData.m_datatype;
}

void
MleDwpLimits::setDatatype(const MleDwpDatatype *datatype)
{
	m_minData.setDatatype(datatype);
	m_maxData.setDatatype(datatype);

	// call the modification notification callbacks for this wp item
	notify(MleDwpChangeNotification);
}

void
MleDwpLimits::setDatatype(const char *datatype)
{
	setDatatype(MleDwpDatatype::findType(datatype));
}

void *
MleDwpLimits::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpLimits::operator delete(void *p)
{
	mlFree(p);
}
