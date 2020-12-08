/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpMatrix34.h
 * @ingroup MleDWPType
 *
 * This file defines the Matrix34 data type used by the Magic Lantern Digital
 * Workprint Library API.
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include "mle/DwpMatrix34.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"

//MLE_DWP_DATATYPE_SOURCE(MleDwpMatrix34,"MlMatrix43",MleDwpDatatype);
MLE_DWP_DATATYPE_SOURCE(MleDwpMatrix34,"MlMatrix34",MleDwpDatatype);

int MleDwpMatrix34::read(MleDwpInput *in,MleDwpDataUnion *data) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[4][3]));
	
#define ELEM(I)	if ( in->readFloat(&((float *)data->m_u.v)[I]) ) return 1
	ELEM(0); ELEM(1); ELEM(2); ELEM(3);
	ELEM(4); ELEM(5); ELEM(6); ELEM(7);
	ELEM(8); ELEM(9); ELEM(10); ELEM(11);
#undef ELEM

	return 0;
}

int MleDwpMatrix34::write(MleDwpOutput *out,MleDwpDataUnion *data) const
{
	if ( data->m_u.v == NULL )
		return 1;
	
#define ELEM(I)	out->writeFloat(((float *)data->m_u.v)[I])
	ELEM(0); ELEM(1); ELEM(2); ELEM(3);
	ELEM(4); ELEM(5); ELEM(6); ELEM(7);
	ELEM(8); ELEM(9); ELEM(10); ELEM(11);
#undef ELEM

	return 0;
}

#if 0
int MleDwpMatrix34::write(MleDppActorGroupOutput *out,MleDwpDataUnion *data) const
{
	if ( data->m_u.v == NULL )
		return 1;
	
	out->writeOffsetLength(sizeof(float[4][3]));
#define ELEM(I)	out->writeFloat(((float *)data->m_u.v)[I])
	ELEM(0); ELEM(1); ELEM(2); ELEM(3);
	ELEM(4); ELEM(5); ELEM(6); ELEM(7);
	ELEM(8); ELEM(9); ELEM(10); ELEM(11);
#undef ELEM

	return 0;
}
#endif /* 0 */

int MleDwpMatrix34::getSize(void) const
{
	return sizeof(float[4][3]);
}

void
MleDwpMatrix34::set(MleDwpDataUnion *data,void *src) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[4][3]));
	
#define ELEM(I)	((float *)data->m_u.v)[I] = ((float *)src)[I];
	ELEM(0); ELEM(1); ELEM(2); ELEM(3);
	ELEM(4); ELEM(5); ELEM(6); ELEM(7);
	ELEM(8); ELEM(9); ELEM(10); ELEM(11);
#undef ELEM
}

void
MleDwpMatrix34::get(MleDwpDataUnion *data,void *dst) const
{
#define ELEM(I)	((float *)dst)[I] = ((float *)data->m_u.v)[I];
	ELEM(0); ELEM(1); ELEM(2); ELEM(3);
	ELEM(4); ELEM(5); ELEM(6); ELEM(7);
	ELEM(8); ELEM(9); ELEM(10); ELEM(11);
#undef ELEM
}

void
MleDwpMatrix34::release(MleDwpDataUnion *data) const
{
	mlFree(data->m_u.v);
	data->m_u.v = NULL;
}

void *
MleDwpMatrix34::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpMatrix34::operator delete(void *p)
{
	mlFree(p);
}
