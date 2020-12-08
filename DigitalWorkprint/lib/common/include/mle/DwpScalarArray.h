/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpScalarArray.h
 * @ingroup MleDWPType
 *
 * This file defines the scalar array data type used by the Magic Lantern Digital
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_DWP_SCALARARRAY_H_
#define __MLE_DWP_SCALARARRAY_H_

// #include "mle/MleScalar.h"
#include "mle/DwpArray.h"

/**
 * This is the datatype object for scalar arrays.
 *
 * BUGS
 * Values and commas separating values in the workprint must be delimited
 * by white space to be read property.  That is,
 *
 *    3.12,4.5,9.1            will be read incorrectly
 *    3.12 , 4.5, 9.1         will be read correctly
 *
 * Arrays written by this datatype are output so they can be read properly.
 *
 * @see MleDwpArray, MleDwpDatatype
 */
class MLE_DWP_API MleDwpScalarArray : public MleDwpArray
{
    MLE_DWP_DATATYPE_HEADER(MleDwpScalarArray);

  public:

    /**
	 */
    virtual int readElement(MleDwpInput *in,void *data) const;

    /**
	 */
    virtual int writeElement(MleDwpOutput *out,void *data) const;

    //virtual int writeElement(MleDppActorGroupOutput *out,void *data) const;

    /**
	 */
    virtual int getElementSize(void) const;

    /**
	 */
    virtual void setElement(MleDwpDataUnion *data,void *src) const;

    /**
	 */
    virtual void getElement(MleDwpDataUnion *data,void *dst) const;

    /**
	 */
    virtual void set(MleDwpDataUnion *data,void *src) const;

    /**
	 */
    virtual void get(MleDwpDataUnion *data,void *dst) const;

	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);

};

#endif /* __MLE_DWP_SCALARARRAY_H_ */
