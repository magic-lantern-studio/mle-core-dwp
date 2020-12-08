/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpVector2.h
 * @ingroup MleDWPType
 *
 * This file defines the 2-element vector data type used by the Magic Lantern Digital
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

#ifndef __MLE_DWP_VECTOR2_H_
#define __MLE_DWP_VECTOR2_H_

// Include Digital Workprint header files.
#include "mle/DwpScalar.h"
#include "mle/DwpDatatype.h"


/**
 *
 * This is the 2-element vector datatype object.  Each element of the vector is a
 * MleScalar.
 *
 * @see MleDwpDatatype
 */
class MLE_DWP_API MleDwpVector2 : public MleDwpDatatype
{
	MLE_DWP_DATATYPE_HEADER(MleDwpVector2);

  public:

	virtual int read(MleDwpInput *in,MleDwpDataUnion *data) const;

	virtual int write(MleDwpOutput *out,MleDwpDataUnion *data) const;

	//virtual int write(MleDppActorGroupOutput *out,MleDwpDataUnion *data) const;

	virtual int getSize(void) const;

	virtual void set(MleDwpDataUnion *data,void *src) const;

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


#endif /* __MLE_DWP_VECTOR2_H_ */
