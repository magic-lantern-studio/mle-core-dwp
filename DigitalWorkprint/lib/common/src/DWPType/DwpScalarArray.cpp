/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpScalarArray.h
 * @ingroup MleDWPType
 *
 * This file implements the scalar array data type used by the Magic Lantern Digital
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include "mle/DwpScalar.h"
#include "mle/DwpScalarArray.h"

//
// int array class
//
MLE_DWP_DATATYPE_SOURCE(MleDwpScalarArray,"ScalarArray",MleDwpArray);

//
// The MlScalar type might be a float or a long 16.16 fixed point
// number.  Or it might be a class encapsulating one of those.
// Read/write the thing as a floating point number for the sake
// of numerics and having a fixed interchange format (esp if we
// go to 16.16 AND 20.12 fixed point numbers).
//
// Read into the data union as a float, then convert to fixed/float
// scalar type on the get()/set().

int
MleDwpScalarArray::readElement(MleDwpInput *in,void *data) const
{
	return in->readFloat((float *)data);
}

int
MleDwpScalarArray::writeElement(MleDwpOutput *out,void *data) const
{
	return out->writeFloat(*(float *)data);
}

#if 0
int
MleDwpScalarArray::writeElement(MleDppActorGroupOutput *out,void *data) const
{
	return out->writeScalar(*(float *)data);
}
#endif /* 0 */

int
MleDwpScalarArray::getElementSize(void) const
{
	return sizeof(MlScalar);
}

void
MleDwpScalarArray::setElement(MleDwpDataUnion *data,void *src) const
{
	SET_FLOAT_FROM_SCALAR(*(float *)(data->m_u.v),src);
}

void
MleDwpScalarArray::getElement(MleDwpDataUnion *data,void *dst) const
{
	SET_SCALAR_FROM_FLOAT(dst,*(float *)(data->m_u.v));
}

void 
MleDwpScalarArray::set(MleDwpDataUnion *data,void *src) const
{
    data->setDatatype(this);

    data->m_u.v = new MleArray<MlScalar> ( *(MleArray<MlScalar> *)src );
}

void 
MleDwpScalarArray::get(MleDwpDataUnion *data,void *dst) const
{
    *(MleArray<MlScalar> *)dst = *(MleArray<MlScalar> *)data->m_u.v;
}

void *
MleDwpScalarArray::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpScalarArray::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpScalarArray::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpScalarArray::operator delete[](void* p)
{
	mlFree(p);
}
