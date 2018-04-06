/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpArray.h
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

#ifndef __MLE_DWP_ARRAY_H_
#define __MLE_DWP_ARRAY_H_

// Include system header files.
// #include <stdio.h>
// #include <malloc.h>
#include <string.h>

// Include Magic Lantern utility header files.
// #include "mle/mlAssert.h"
#include "mle/mlArray.h"

// Include Digital Workprint header files.
#include "mle/DwpDatatype.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"


/**
 * @brief This is an abstract class that defines a datatype for arrays of data
 * elements.
 *
 * The general syntax for arrays in the workprint is the entire array is
 * within square brackets ([]) and individual elements are separated by
 * commas (,).  For example:
 *
 *     [ 1 , 2 , 3 , 4 ]
 *
 * The final value may have a comma after it or it may be omitted.
 *
 * BUGS
 * There is currently a bug in many of the array types that causes input
 * errors when the commas separating array elements are not delimited by
 * spaces.  That is
 *
 *     [ 1,2,3,4 ]        will be read incorrectly,
 *     [ 1 , 2 , 3 , 4 ]  will be read correctly.
 *
 * @see MleDwpIntArray, MleDwpScalarArray, MleDwpVector3Array
 *
 */
class MLE_DWP_API MleDwpArray : public MleDwpDatatype
{
	MLE_DWP_DATATYPE_ABSTRACT_HEADER(MleDwpDatatype);

  public:

    /**
	 */
    virtual int readElement(MleDwpInput *in, void *data) const = 0;

    /**
	 */
    virtual int writeElement(MleDwpOutput *out, void *data) const = 0;

    //virtual int writeElement(MleDppActorGroupOutput *,void *) const = 0;

    /**
	 */
    virtual int getElementSize(void) const = 0;

    /**
	 */
    virtual void setElement(MleDwpDataUnion *data, void *src) const = 0;

    /**
	 */
    virtual void getElement(MleDwpDataUnion *data, void *dst) const = 0;

    /**
	 */
    virtual int read(MleDwpInput *in, MleDwpDataUnion *data) const;

    /**
	 */
    virtual int write(MleDwpOutput *out, MleDwpDataUnion *data) const;

    //virtual int write(MleDppActorGroupOutput *out, MleDwpDataUnion *data) const;

    /**
	 */
    virtual int getSize(void) const;

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

#endif /* __MLE_DWP_ARRAY_H_ */
