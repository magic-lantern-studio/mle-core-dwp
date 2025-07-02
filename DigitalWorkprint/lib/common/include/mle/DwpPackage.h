/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpPackage.h
 * @ingroup MleDWPModel
 *
 * This file defines the Package workprint item.
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

#ifndef __MLE_DWP_PACKAGE_H_
#define __MLE_DWP_PACKAGE_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This is the item that identifies the package that an Actor
 * belongs to.  It is always a subitem of an ActorDef.  The package
 * information is used by the mastering process.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (Package <name>)
 * 
 * <name> actor class package (char *)
 * 
 * Contained by: MleDwpActorDef
 * Contains: none
 * END
 */
class MLE_DWP_API MleDwpPackage : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpPackage);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpPackage(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpPackage(void);

	/**
	 * Sets the package. The package name is the same
	 * as the item name (for finding purposes).
	 */
	void setPackage(const char *package) { setName(package); }

	/**
	 * Returns the package.  NULL is returned if no
	 * package has been set.  The package name is the same as the
	 * item name (for finding purposes).
	 */
	const char *getPackage(void) const { return getName(); }

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

  protected:

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);
};

#endif /* __MLE_DWP_PACKAGE_H_ */
