/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpTransform.h
 * @ingroup MleDWPType
 *
 * This file defines the Transform data type used by the Magic Lantern Digital
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
#include "math/transfrm.h"
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/DwpTransform.h"
#include "mle/DwpScalar.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"

MLE_DWP_DATATYPE_SOURCE(MleDwpTransform,"MlTransform",MleDwpDatatype);

//
// The MlScalar type might be a float or a long 16.16 fixed point
// number.  Or it might be a class encapsulating one of those.
// Read/write the thing as a floating point number for the sake
// of numerics and having a fixed interchange format (esp if we
// go to 16.16 AND 20.12 fixed point numbers).
//

int
MleDwpTransform::read(MleDwpInput *in,MleDwpDataUnion *data) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[4][3]));
	
	// Read floats and stuff into memory as fixed/float.
	for (int i=0; i<4; i++) {
		for (int j=0; j<3; j++) {
			if (in->readFloat(&((float(*)[3])data->m_u.v)[i][j])) return 1;
		}
	}

	return 0;
}

int
MleDwpTransform::write(MleDwpOutput *out,MleDwpDataUnion *data) const
{
	for (int i=0; i<4; i++) {
		for (int j=0; j<3; j++) {
			if (out->writeFloat(((float(*)[3])data->m_u.v)[i][j])) return 1;
		}
	}

	return 0;
}

#if 0
int
MleDwpTransform::write(MleDppActorGroupOutput *out,MleDwpDataUnion *data) const
{
	int i, j;

	out->writeOffsetLength(48);

	switch (out->getScalarFormat()) {
	default:
	case MLE_PP_SCALAR_FLOAT:
		for (i=0; i<4; i++) {
		for (j=0; j<3; j++) {
			if (out->writeFloat(((float(*)[3])data->m_u.v)[i][j])) return 1;
		}
		}
		return 0;
	case MLE_PP_SCALAR_FIXED_16_16:
		for (i=0; i<4; i++) {
		for (j=0; j<3; j++) {
			if (out->writeInt(mlPpFixed1616(((float(*)[3])data->m_u.v)[i][j]))) return 1;
		}
		}
		return 0;
	case MLE_PP_SCALAR_FIXED_20_12:
		for (i=0; i<4; i++) {
		for (j=0; j<3; j++) {
			if (out->writeInt(mlPpFixed2012(((float(*)[3])data->m_u.v)[i][j]))) return 1;
		}
		}
		return 0;
	}
}
#endif /* 0 */

int
MleDwpTransform::getSize(void) const
{
	return sizeof(MlTransform);
}

void
MleDwpTransform::set(MleDwpDataUnion *data,void *src) const
{
	data->setDatatype(this);

	if ( data->m_u.v == NULL )
		data->m_u.v = mlMalloc(sizeof(float[4][3]));
	
	int i, j;
	for (i=0; i<4; i++) {
		for (j=0; j<3; j++) {
			SET_FLOAT_FROM_SCALAR( (((float (*)[3])data->m_u.v)[i][j]),
					       &((*(MlTransform *)src)[i][j]));
		}
	}
}

void
MleDwpTransform::get(MleDwpDataUnion *data,void *dst) const
{
	int i, j;
	for (i=0; i<4; i++) {
		for (j=0; j<3; j++) {
			SET_SCALAR_FROM_FLOAT( &(*(MlTransform *)dst)[i][j],
					       ((float(*)[3])data->m_u.v)[i][j]);
		}
	}
}

void *
MleDwpTransform::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpTransform::operator delete(void *p)
{
	mlFree(p);
}
