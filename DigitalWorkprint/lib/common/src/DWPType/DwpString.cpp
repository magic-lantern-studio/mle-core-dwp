/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpString.h
 * @ingroup MleDWPType
 *
 * This file implements the string data type used by the Magic Lantern Digital
 * Workprint Library API.
 *
 * @author Mark S. Millard
 * @date February 6, 2004
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

// Inlcude Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/DwpString.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"


MLE_DWP_DATATYPE_SOURCE(MleDwpString,"string",MleDwpDatatype);

int
MleDwpString::read(MleDwpInput *in,MleDwpDataUnion *data) const
{
	// Set the datatype (and allow release of any existing data).
	data->setDatatype(this);

	// Malloc space.
	data->m_u.v = mlMalloc(MLE_DWP_INPUT_BUFSIZE);
	
	// Read the string.
	if ( in->readString((char *)data->m_u.v) )
		return 1;

	// Realloc to conserve space.
	int len = strlen((char *)data->m_u.v);
	if ( len )
		data->m_u.v = mlRealloc(data->m_u.v,strlen((char *)data->m_u.v) + 1);
	else
	{
		mlFree(data->m_u.v);
		data->m_u.v = NULL;
	}

	return 0;
}

int
MleDwpString::write(MleDwpOutput *out,MleDwpDataUnion *data) const
{
	if ( data->m_u.v )
		out->writeFancyString((char *)data->m_u.v);
	else
		out->writeString("");

	return 0;
}

#if 0
//
// Strings in the Playprint must be preceeded by a command that will allocate
// the storage for them.
//

int
MleDwpString::write(MleDppActorGroupOutput *out,MleDwpDataUnion *data) const
{
	out->writeOpcode(copyDynamicPropertyOpcode);

	if ( data->m_u.v )
	{
		out->writeInt(strlen((char *)data->m_u.v));
		out->writeString((char *)data->m_u.v);
	}
	else
	{
		out->writeInt(0);
		printf("warning: zero length dynamic property written.\n");
	}

	return 0;
}
#endif /* 0 */

int
MleDwpString::getSize(void) const
{
	// This is the pointer size.
	//   This is the pointer size so it will be transferred correctly
	//   into an actor's memory.  Note that this is the *wrong* size
	//   to use for any binary transcription.
	return sizeof(char *);
}

void
MleDwpString::set(MleDwpDataUnion *data,void *src) const
{
	// Set the datatype (to allow release of any existing data).
	data->setDatatype(this);

	// Allocate space the length of the string.
	//   Note that the src pointer is a pointer to the string pointer,
	//   *not* a pointer to the string.
	if ( *(char **)src )
	{
		data->m_u.v = mlMalloc(strlen(*(char **)src) + 1);
	
		// copy the string
		strcpy((char *)data->m_u.v,*(char **)src);
	}
	else
		data->m_u.v = NULL;
}

void
MleDwpString::get(MleDwpDataUnion *data,void *dst) const
{
	// Allocate space to write the string.
	if ( data->m_u.v )
	{
		*(char **)dst = (char *)mlMalloc(strlen((char *)data->m_u.v) + 1);

		// Write the string.
		strcpy(*(char **)dst,(char *)data->m_u.v);
	}
	else
		*(char **)dst = NULL;
}

void
MleDwpString::release(MleDwpDataUnion *data) const
{
	mlFree(data->m_u.v);
	data->m_u.v = NULL;
}

void *
MleDwpString::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpString::operator delete(void *p)
{
	mlFree(p);
}
