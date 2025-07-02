/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpRotation.h
 * @ingroup MleDWPType
 *
 * This file defines the Rotation data type used by the Magic Lantern Digital
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

#ifndef __MLE_DWP_ROTATION_H_
#define __MLE_DWP_ROTATION_H_

// Include Digital Workprint header files.
#include "mle/DwpDatatype.h"

/**
 * This is the datatype object for a rotation.
 *
 * Rotations are written in a workprint as a unit quaternion, which is a
 * 4-element vector of unit length.  The first three numbers are the axis
 * of rotation, and the last number is related to the angular amount of
 * rotation (but is not a direct amount measurable in degrees or radians).
 *
 * The identity rotation is 0 0 0 1.
 *
 * @see MleDwpDatatype
 */
class MLE_DWP_API MleDwpRotation : public MleDwpDatatype
{
	MLE_DWP_DATATYPE_HEADER(MleDwpRotation);

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
     * Override operator new array.
     *
     * @param tSize The size, in bytes, to allocate.
     */
	void* operator new[](size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);

	/**
     * Override operator delete array.
     *
     * @param p A pointer to the memory to delete.
     */
	void  operator delete[](void* p);
};

#endif /* __MLE_DWP_ROTATION_H_ */
