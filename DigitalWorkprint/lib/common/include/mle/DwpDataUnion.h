/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpDataUnion.h
 * @ingroup MleDWPType
 *
 * This file defines the container for data types used by the Magic Lantern Digital
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

#ifndef __MLE_DWP_DATAUNION_H_
#define __MLE_DWP_DATAUNION_H_

// Include Digital Workprint header files.
#include "mle/Dwp.h"

class MleDwpDatatype;

/**
 * @brief A data type container.
 *
 * This is an object that holds data.  For convenience, it has a union that
 * already defines some basic primitives.  One of the union elements can be
 * used for data that fits nicely within it.
 *
 * The data union object is operated on by a MleDwpDatatype object.  The datatype
 * contains the knowledge of how to write the data union to output, read it
 * from input, and transfer values to and from memory.  In general, manipulation
 * of a data union will take place using the datatype object.
 *
 * @see MleDwpDatatype
 */
class MLE_DWP_API MleDwpDataUnion
{
  public:

	/**
	 * The default constructor.
	 */
	MleDwpDataUnion(void);

	/**
	 * The destructor.
	 */
	~MleDwpDataUnion();

	/**
	 * @brief Sets the datatype for this data union.
	 *
	 * If the datatype is the same as currently set, this function does
	 * nothing.  If this is a new datatype, the old datatype will
	 * be called to release the previous value (e.g. deallocate
	 * memory) before the new datatype is set.
	 */
	void setDatatype(const MleDwpDatatype *datatype);

	/**
	 * A union of anonymous data types.
	 */
	union AnonType {
		char	c;
		short	s;
		int		i;
		long	l;
		float	f;
		double	d;
		void	*v;
	};

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

	/**
	 * The union of possible types of values.
	  */
 	AnonType m_u;

	/**
	 * The associated datatype.
	 */
	const MleDwpDatatype *m_datatype;
};

#endif /* __MLE_DWP_DATAUNION_H_ */
