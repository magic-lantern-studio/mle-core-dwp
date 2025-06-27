/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpBoot.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
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


#ifndef __MLE_DWP_BOOT_H_
#define __MLE_DWP_BOOT_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This is the item that defines automatic loading of an actor group during
 * the beginning of a title.  Multiple boot groups are allowed by using
 * multiple instances of MleDwpBoot items.
 *
 * WORKPRINT SYNTAX
 *
 * EXAMPLE
 * Syntax:		(Boot <name>)
 *
 * <name> name of the actor group (char *)
 *
 * Contained by:	none
 * Contains:		none
 * END
 */
class MLE_DWP_API MleDwpBoot : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpBoot);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpBoot(void);

	/**
	 * The destructor.
	 */
    virtual ~MleDwpBoot(void);

	/**
	 * Sets the boot group name.  The group name is the
	 * same as the item name (for finding purposes).
	 */
	void setGroup(const char *name) { setName(name); }

	/**
	 * Returns the boot group name.  NULL is returned
	 * if no group has been set.  The group name is the same as
	 * the item name (for finding purposes).
     */
	const char *getGroup(void) const { return getName(); }

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

#endif /* __MLE_DWP_BOOT_H_ */
