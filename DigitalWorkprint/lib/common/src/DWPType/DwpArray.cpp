/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpArray.cpp
 * @ingroup MleDWPType
 *
 * This file defines the array data type used by the Magic Lantern Digital
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include "mle/DwpArray.h"


MLE_DWP_DATATYPE_ABSTRACT_SOURCE(MleDwpArray,MleDwpDatatype);

int MleDwpArray::read(MleDwpInput *in, MleDwpDataUnion *data) const
{
	data->setDatatype(this);

	char * array = NULL;
	int numElements = 0;

	char c;	// Input character buffer.

	if ( in->readChar(&c) )
	    return 1;

	if ( c != '[' )
	{
	    in->reportError("illegal array: missing [");
		return 1;
	}

	int elementSize = getElementSize();
	int arraySize = 0;

	for ( ;; )
	{
	    arraySize += elementSize;
	    array = (char *)mlRealloc(array,arraySize);

	    if ( readElement(in,array+arraySize-elementSize) )
			return 1;

	    numElements++;

	    if ( in->readChar(&c) )
			return 1;

	    if ( c == ']' )
			break;

	    if ( c != ',' )
	    {
			in->reportError("illegal array: missing ,");
			return 1;
	    }
	}

	data->m_u.v = new MleArray<char> ( numElements, array );

	return 0;
}

int MleDwpArray::write(MleDwpOutput *out, MleDwpDataUnion *data) const
{
	MleArray<char> * array = (MleArray<char> *) data->m_u.v;

	int size = getElementSize();

	out->writeString ( "[" );

	for ( int i = 0; i < array->size(); i++ )
	{
	    writeElement ( out, *array + i * size );

	    if ( i < (array->size()-1) )
			out->writeString ( "," );
	}

	out->writeString ( "]" );

	return 0;
}

/*
int MleDwpArray::write(MleDppActorGroupOutput * out,MleDwpDataUnion * data) const
{
	MleArray<char> * array = (MleArray<char> *) data->m_u.v;

	int size = getElementSize();

    out->writeArrayOffsetLength(size,array->size());

	for ( int i = 0; i < array->size(); i++ )
	{
	    writeElement ( out, *array + i * size );
	}

	return 0;
}
*/

int MleDwpArray::getSize(void) const
{
	return ( sizeof(MleArray<char>) );
}

void MleDwpArray::set(MleDwpDataUnion *data,void *src) const
{
	data->setDatatype(this);

	data->m_u.v = new MleArray<char> ( *(MleArray<char> *)src );
}

void MleDwpArray::get(MleDwpDataUnion *data,void *dst) const
{
	*(MleArray<char> *)dst = *(MleArray<char> *)data->m_u.v;
}

void *
MleDwpArray::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpArray::operator delete(void *p)
{
	mlFree(p);
}
