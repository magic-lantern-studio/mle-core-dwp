/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpMediaRefClass.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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


#ifndef __MLE_DWP_MEDIAREFCLASS_H_
#define __MLE_DWP_MEDIAREFCLASS_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

class MleDwpDatatype;

/**
 * This is a subitem of a PropertyDef statement.  It lets the tools know
 * what are valid classes for a particular media ref.
 *
 * Syntax: (MediaRefClass <name> <mediaRefClassName>)
 * <name> property name (char *)
 * <mediaRefClassName> (char*)
 *
 * Contained by: PropertyDef (only media ref PropertyDef)
 * Contains:     None.
 *
 * @see MleDwpProperty
 */
class MLE_DWP_API MleDwpMediaRefClass : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpMediaRefClass);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpMediaRefClass(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpMediaRefClass();

	/**
	 * Sets the class name .
	 */  
	void setClassName(const char *);

	/**
	 * Gets the class name.
	 */
	const char *getClassName(void) const;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

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

	char *m_className;
};

#endif /* __MLE_DWP_MEDIAREFCLASS_H_ */
