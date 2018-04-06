/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpEnumDef.h
 * @ingroup MleDWPType
 *
 * This file defines the enumeration definition used by the Magic Lantern Digital
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

#ifndef __MLE_DWP_ENUMDEF_H_
#define __MLE_DWP_ENUMDEF_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 *
 * This defines the strings for an enumerated type.  These strings are used
 * by authoring tools to present a proper interface for an enumeration.  The
 * values stored in actor properties, however, are simple integers.
 *
 * WORKPRINT SYNTAX
 *
 *   EXAMPLE
 *   Syntax:  (EnumDef <Str0> <Str1> <Str2> ...)
 *
 *   <StrN> name of enum string N
 *
 *  Contained by:	MleDwpActorDef
 * Contains:		none
 * END
 */
class MLE_DWP_API MleDwpEnumDef : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpEnumDef);

	/**
	 * The default constructor.
	 */
	MleDwpEnumDef();

	/**
	 * The destructor.
	 */
	~MleDwpEnumDef();

  public:

	/**
	 * Returns the string associated with the integer passed in
	 */
    const char* getString(int i);

	/**
	 * Returns the number of values in the enumeration.
	 */
	int getNumEnums() { return m_length; }

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

  protected:

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	char **m_enumStrings;
	int m_length;
};

#endif /* __MLE_DWP_ENUMDEF_H_ */
