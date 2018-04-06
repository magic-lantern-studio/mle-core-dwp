/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpFloat3.cpp
 * @ingroup MleDWPType
 *
 * This file implements the float data type used by the Magic Lantern Digital
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

// Inlclude system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include "mle/DwpFloat3.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"


MLE_DWP_DATATYPE_SOURCE(MleDwpFloat3,"float3",MleDwpDatatype);

int MleDwpFloat3::read(MleDwpInput *in, MleDwpDataUnion *data) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[3]));
	
	if ( in->readFloat(&((float *)data->m_u.v)[0]) ) return 1;
	if ( in->readFloat(&((float *)data->m_u.v)[1]) ) return 1;
	if ( in->readFloat(&((float *)data->m_u.v)[2]) ) return 1;

	return 0;
}

int MleDwpFloat3::write(MleDwpOutput *out, MleDwpDataUnion *data) const
{
	if ( data->m_u.v == NULL )
		return 1;
	
	out->writeFloat(((float *)data->m_u.v)[0]);
	out->writeFloat(((float *)data->m_u.v)[1]);
	out->writeFloat(((float *)data->m_u.v)[2]);

	return 0;
}

/*
int MleDwpFloat3::write(MleDppActorGroupOutput *out, MleDwpDataUnion *data) const
{
	if ( data->m_u.v == NULL )
		return 1;
	
	out->writeOffsetLength(sizeof(float) * 3);
	out->writeFloat(((float *)data->m_u.v)[0]);
	out->writeFloat(((float *)data->m_u.v)[1]);
	out->writeFloat(((float *)data->m_u.v)[2]);

	return 0;
}
*/

int MleDwpFloat3::getSize(void) const
{
	return sizeof(float[3]);
}

void
MleDwpFloat3::set(MleDwpDataUnion *data, void *src) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[3]));
	
	((float *)data->m_u.v)[0] = ((float *)src)[0];
	((float *)data->m_u.v)[1] = ((float *)src)[1];
	((float *)data->m_u.v)[2] = ((float *)src)[2];
}

void
MleDwpFloat3::get(MleDwpDataUnion *data, void *dst) const
{
	((float *)dst)[0] = ((float *)data->m_u.v)[0];
	((float *)dst)[1] = ((float *)data->m_u.v)[1];
	((float *)dst)[2] = ((float *)data->m_u.v)[2];
}

void
MleDwpFloat3::release(MleDwpDataUnion *data) const
{
	mlFree(data->m_u.v);
	data->m_u.v = NULL;
}

void *
MleDwpFloat3::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpFloat3::operator delete(void *p)
{
	mlFree(p);
}
