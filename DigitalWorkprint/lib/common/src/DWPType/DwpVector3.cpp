/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpVector3.cpp
 * @ingroup MleDWPType
 *
 * This file implements the 3-element vector data type used by the Magic Lantern Digital
 * Workprint Library API.
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.#include <stdio.h>


// Include Magic Lantern header files.
#include "math/vector.h"
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/DwpVector3.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"

MLE_DWP_DATATYPE_SOURCE(MleDwpVector3,"MlVector3",MleDwpDatatype);

//
// The MlScalar type might be a float or a long 16.16 fixed point
// number.  Or it might be a class encapsulating one of those.
// Read/write the thing as a floating point number for the sake
// of numerics and having a fixed interchange format (esp if we
// go to 16.16 AND 20.12 fixed point numbers).
//

int
MleDwpVector3::read(MleDwpInput *in,MleDwpDataUnion *data) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[3]));
	
	// Read floats and stuff into memory as fixed/float.
	if (in->readFloat(&((float*)data->m_u.v)[0])) return 1;
	if (in->readFloat(&((float*)data->m_u.v)[1])) return 1;
	if (in->readFloat(&((float*)data->m_u.v)[2])) return 1;

	return 0;
}

int
MleDwpVector3::write(MleDwpOutput *out,MleDwpDataUnion *data) const
{
	if (out->writeFloat(((float*)data->m_u.v)[0])) return 1;
	if (out->writeFloat(((float*)data->m_u.v)[1])) return 1;
	if (out->writeFloat(((float*)data->m_u.v)[2])) return 1;

	return 0;
}

#if 0
int
MleDwpVector3::write(MleDppActorGroupOutput *out,MleDwpDataUnion *data) const
{
	out->writeOffsetLength(12);

	switch (out->getScalarFormat()) {
	default:
	case MLE_PP_SCALAR_FLOAT:
		if (out->writeFloat(((float*)data->m_u.v)[0])) return 1;
		if (out->writeFloat(((float*)data->m_u.v)[1])) return 1;
		if (out->writeFloat(((float*)data->m_u.v)[2])) return 1;
		return 0;
	case MLE_PP_SCALAR_FIXED_16_16:
		if (out->writeInt(mlPpFixed1616(((float*)data->m_u.v)[0]))) return 1;
		if (out->writeInt(mlPpFixed1616(((float*)data->m_u.v)[1]))) return 1;
		if (out->writeInt(mlPpFixed1616(((float*)data->m_u.v)[2]))) return 1;
		return 0;
	case MLE_PP_SCALAR_FIXED_20_12:
		if (out->writeInt(mlPpFixed2012(((float*)data->m_u.v)[0]))) return 1;
		if (out->writeInt(mlPpFixed2012(((float*)data->m_u.v)[1]))) return 1;
		if (out->writeInt(mlPpFixed2012(((float*)data->m_u.v)[2]))) return 1;
		return 0;
	}
}
#endif /* 0 */

int
MleDwpVector3::getSize(void) const
{
	return sizeof(MlVector3);
}

void
MleDwpVector3::set(MleDwpDataUnion *data,void *src) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[3]));
	
	// Scalar copy
	int i;
	for (i=0; i<3; i++) {
		SET_FLOAT_FROM_SCALAR( ((float *)data->m_u.v)[i],
				       &(*(MlVector3 *)src)[i]);
	}
}

void
MleDwpVector3::get(MleDwpDataUnion *data,void *dst) const
{
	// Scalar copy
	int i;
	for (i=0; i<3; i++) {
		SET_SCALAR_FROM_FLOAT( &(*(MlVector3 *)dst)[i],
				       ((float *)data->m_u.v)[i]);
	}
}

void *
MleDwpVector3::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpVector3::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpVector3::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpVector3::operator delete[](void* p)
{
	mlFree(p);
}
