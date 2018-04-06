/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpScalar.cpp
 * @ingroup MleDWPType
 *
 * This file implements the scalar data type used by the Magic Lantern Digital
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
#include "math/scalar.h"
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/DwpScalar.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"

MLE_DWP_DATATYPE_SOURCE(MleDwpScalar,"MlScalar",MleDwpDatatype);

//
// The MlScalar type might be a float or a long 16.16 fixed point
// number.  Or it might be a class encapsulating one of those.
// Read/write the thing as a floating point number for the sake
// of numerics and having a fixed interchange format (especially if we
// go to 16.16 AND 20.12 fixed point numbers).
//
// Read into the data union as a float, then convert to fixed/float
// scalar type on the get()/set().

int
MleDwpScalar::read(MleDwpInput *in,MleDwpDataUnion *data) const
{
	data->setDatatype(this);

	return (in->readFloat(&data->m_u.f));
}

int
MleDwpScalar::write(MleDwpOutput *out,MleDwpDataUnion *data) const
{
	return out->writeFloat(data->m_u.f);
}

#if 0
int
MleDwpScalar::write(MleDppActorGroupOutput *out,MleDwpDataUnion *data) const
{
	out->writeOffsetLength(4);

	switch (out->getScalarFormat()) {
	default:
	case MLE_PP_SCALAR_FLOAT:
		return out->writeFloat(data->u.f);
	case MLE_PP_SCALAR_FIXED_16_16:
		return out->writeInt(mlPpFixed1616(data->u.f));
	case MLE_PP_SCALAR_FIXED_20_12:
		return out->writeInt(mlPpFixed2012(data->u.f));
	}
}
#endif /* 0 */

int
MleDwpScalar::getSize(void) const
{
	return sizeof(MlScalar);
}

void
MleDwpScalar::set(MleDwpDataUnion *data,void *src) const
{
	data->setDatatype(this);

	SET_FLOAT_FROM_SCALAR(data->m_u.f,src);
}

void
MleDwpScalar::get(MleDwpDataUnion *data,void *dst) const
{
	SET_SCALAR_FROM_FLOAT(dst,data->m_u.f);
}

void *
MleDwpScalar::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpScalar::operator delete(void *p)
{
	mlFree(p);
}
