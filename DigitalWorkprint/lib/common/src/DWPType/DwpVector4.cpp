/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpVector4.cpp
 * @ingroup MleDWPType
 *
 * This file implements the 4-element vector data type used by the Magic Lantern Digital
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.

// Include Magic Lantern header files.
#include "math/vector.h"
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include DigitalWorkprint header files.
#include "mle/DwpVector4.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"

MLE_DWP_DATATYPE_SOURCE(MleDwpVector4,"MlVector4",MleDwpDatatype);

//
// The MlScalar type might be a float or a long 16.16 fixed point
// number.  Or it might be a class encapsulating one of those.
// Read/write the thing as a floating point number for the sake
// of numerics and having a fixed interchange format (esp if we
// go to 16.16 AND 20.12 fixed point numbers).
//

int
MleDwpVector4::read(MleDwpInput *in,MleDwpDataUnion *data) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[4]));
	
	// Read floats and stuff into memory as fixed/float.
	if (in->readFloat(&((float*)data->m_u.v)[0])) return 1;
	if (in->readFloat(&((float*)data->m_u.v)[1])) return 1;
	if (in->readFloat(&((float*)data->m_u.v)[2])) return 1;
	if (in->readFloat(&((float*)data->m_u.v)[3])) return 1;

	return 0;
}

int
MleDwpVector4::write(MleDwpOutput *out,MleDwpDataUnion *data) const
{
	if (out->writeFloat(((float*)data->m_u.v)[0])) return 1;
	if (out->writeFloat(((float*)data->m_u.v)[1])) return 1;
	if (out->writeFloat(((float*)data->m_u.v)[2])) return 1;
	if (out->writeFloat(((float*)data->m_u.v)[3])) return 1;

	return 0;
}

#if 0
int
MleDwpVector4::write(FwPpActorGroupOutput *out,MleDwpDataUnion *data) const
{
	out->writeOffsetLength(16);

	switch (out->getScalarFormat()) {
	default:
	case MLE_PP_SCALAR_FLOAT:
		if (out->writeFloat(((float*)data->m_u.v)[0])) return 1;
		if (out->writeFloat(((float*)data->m_u.v)[1])) return 1;
		if (out->writeFloat(((float*)data->m_u.v)[2])) return 1;
		if (out->writeFloat(((float*)data->m_u.v)[3])) return 1;
		return 0;
	case MLE_PP_SCALAR_FIXED_16_16:
		if (out->writeInt(mlPpFixed1616(((float*)data->m_u.v)[0]))) return 1;
		if (out->writeInt(mlPpFixed1616(((float*)data->m_u.v)[1]))) return 1;
		if (out->writeInt(mlPpFixed1616(((float*)data->m_u.v)[2]))) return 1;
		if (out->writeInt(mlPpFixed1616(((float*)data->m_u.v)[3]))) return 1;
		return 0;
	case MLE_PP_SCALAR_FIXED_20_12:
		if (out->writeInt(mlPpFixed2012(((float*)data->m_u.v)[0]))) return 1;
		if (out->writeInt(mlPpFixed2012(((float*)data->m_u.v)[1]))) return 1;
		if (out->writeInt(mlPpFixed2012(((float*)data->m_u.v)[2]))) return 1;
		if (out->writeInt(mlPpFixed2012(((float*)data->m_u.v)[3]))) return 1;
		return 0;
	}
}
#endif /* 0 */

int
MleDwpVector4::getSize(void) const
{
	return sizeof(MlVector4);
}

void
MleDwpVector4::set(MleDwpDataUnion *data,void *src) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[4]));
	
	int i;
	for (i=0; i<4; i++) {
		SET_FLOAT_FROM_SCALAR( ((float *)data->m_u.v)[i],
				       &(*(MlVector4 *)src)[i]);
	}
}

void
MleDwpVector4::get(MleDwpDataUnion *data,void *dst) const
{
	int i;
	for (i=0; i<4; i++) {
		SET_SCALAR_FROM_FLOAT( &(*(MlVector4 *)dst)[i],
				       ((float *)data->m_u.v)[i]);
	}
}

void *
MleDwpVector4::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpVector4::operator delete(void *p)
{
	mlFree(p);
}
