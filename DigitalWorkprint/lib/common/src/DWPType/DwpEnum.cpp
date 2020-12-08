/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpEnum.cpp
 * @ingroup MleDWPType
 *
 * This file implements the enumeration data type used by the Magic Lantern Digital
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


// Inlcude system header files.
#include <stdio.h>
#include <string.h>

// Include Digital Workprint header files.
#include "mle/DwpEnum.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"


MLE_DWP_DATATYPE_SOURCE(MleDwpEnum,"enum",MleDwpDatatype);

int
MleDwpEnum::read(MleDwpInput *in,MleDwpDataUnion *data) const
{
	data->setDatatype(this);
	return in->readInt(&data->m_u.i);
}

int
MleDwpEnum::write(MleDwpOutput *out,MleDwpDataUnion *data) const
{
	return out->writeInt(data->m_u.i);
}

#if 0
int
MleDwpEnum::write(MleDppActorGroupOutput *out,MleDwpDataUnion *data) const
{
	out->writeOffsetLength(4);
	return out->writeInt(data->m_u.i);
}
#endif /* 0 */

int
MleDwpEnum::getSize(void) const
{
	return sizeof(int);
}

void
MleDwpEnum::set(MleDwpDataUnion *data,void *src) const
{
	data->setDatatype(this);
	data->m_u.i = *(int *)src;
}

void
MleDwpEnum::get(MleDwpDataUnion *data,void *dst) const
{
	*(int *)dst = data->m_u.i;
}

void *
MleDwpEnum::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpEnum::operator delete(void *p)
{
	mlFree(p);
}
