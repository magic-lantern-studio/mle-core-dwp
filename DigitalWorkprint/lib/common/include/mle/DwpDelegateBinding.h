/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpDelegateBinding.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @deprecated The MleDwpDelegateBinding class has been replaced with the
 * MleDwpRoleBinding class.
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


#ifndef __MLE_DWP_DELEGATEBINDING_H_
#define __MLE_DWP_DELEGATEBINDING_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This defines what type of delegate to create for an actor instance.  It also
 * specifies the forum in which the actor is placed.  The forum must match the
 * type specified by the actor definition's delegate forum mapping.
 * 
 * WORKPRINT SYNTAX
 * EXAMPLE
 * Syntax: (DelegateBinding <name> <forum>)
 * 
 * <name> delegate class name (char *)
 * <forum> forum name (char *)
 * 
 * Contained by: MleDwpActor
 * Contains: none
 * END
 * 
 * @see MleDwpDelegateForumMapping
 */
class MLE_DWP_API MleDwpDelegateBinding : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpDelegateBinding);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpDelegateBinding(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpDelegateBinding();

	/**
	 * Sets the type of delegate (C++ class name)
	 * to bind.  The delegate type is the same as the item name
	 * (for finding purposes).
	 */
	void setDelegateType(const char *type) { setName(type); }

	/**
	 * Returns the delegate type.  NULL is returned
	 * if no delegate type has been set.  The delegate type is the
	 * same as the item name (for finding purposes).
	 */
	const char *getDelegateType(void) const { return getName(); }

	/**
	 * Sets the forum instance name to put the delegate in.
	 * This forum should be declared elsewhere in the workprint in
	 * a MleDwpForum item.
	 */
	void setForum(const char *);

	/**
	 * Returns the forum instance name.  NULL is returned
	 * if no forum has been set.
	 */
	const char *getForum(void) const;

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

	char *m_forum;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	//virtual int writeContents(MleDppActorGroupOutput *out);
};

#endif /* __MLE_DWP_DELEGATEBINDING_H_ */
