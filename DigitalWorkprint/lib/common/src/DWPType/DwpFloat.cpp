/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpFloat.cpp
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

// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include "mle/DwpFloat.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"


MLE_DWP_DATATYPE_SOURCE(MleDwpFloat,"float",MleDwpDatatype);

int
MleDwpFloat::read(MleDwpInput *in, MleDwpDataUnion *data) const
{
	data->setDatatype(this);
	return in->readFloat(&data->m_u.f);
}

int
MleDwpFloat::write(MleDwpOutput *out, MleDwpDataUnion *data) const
{
	return out->writeFloat(data->m_u.f);
}

/*
int
MleDwpFloat::write(MleDppActorGroupOutput *out, MleDwpDataUnion *data) const
{
	out->writeOffsetLength(4);
	return out->writeFloat(data->m_u.f);
}
*/

int
MleDwpFloat::getSize(void) const
{
	return sizeof(float);
}

void
MleDwpFloat::set(MleDwpDataUnion *data, void *src) const
{
	data->setDatatype(this);
	data->m_u.f = *(float *)src;
}

void
MleDwpFloat::get(MleDwpDataUnion *data, void *dst) const
{
	*(float *)dst = data->m_u.f;
}

void *
MleDwpFloat::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpFloat::operator delete(void *p)
{
	mlFree(p);
}
