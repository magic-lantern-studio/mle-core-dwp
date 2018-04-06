/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpDelegateForumMapping.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 *
 * @deprecated The MleDwpDelegateForumMapping class has been replaced with the
 * MleDwpRoleSetMapping class.
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


#ifndef __MLE_DWP_DELEGATEFORUMMAPPING_H_
#define __MLE_DWP_DELEGATEFORUMMAPPING_H_


// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This defines what type of delegate to create for an actor instance,
 * and what type of forum it should go into.  The information will be
 * used to generate a MleDwpDelegateBinding, which must choose a forum
 * instance of the specified forum type.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (DelegateForumMapping <type> <forum>)
 * 
 * <type> delegate class name (char *)
 * <forum> forum name (char *)
 * 
 * Contained by: MleDwpActorDef, MleDwpActorTemplate
 * Contains: none
 * 
 * @see MleDwpDelegateBinding
 */
class MLE_DWP_API MleDwpDelegateForumMapping : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpDelegateForumMapping);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpDelegateForumMapping(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpDelegateForumMapping();

	/**
	 * Sets the delegate type (C++ class).  The
	 * delegate type is also the item name (for finding purposes).
	 */
	void setDelegateClass(const char *delegate) { setName(delegate); }

	/**
	 * Returns the delegate type (C++ class).  NULL
	 * is returned if the delegate type has not been set.  The
	 * delegate type is also the item name ( for finding purposes).
	 */
	const char *getDelegateClass(void) const { return getName(); }

	/**
	 * Sets the forum type (C++ class) that the delegate
	 * should go into.
	 */
	void setForum(const char *);

	/**
	 * Returns the forum type.  NULL is returned if no
     * forum is set.
	 */
	const char *getForum(void) const;

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

	char *m_forum;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);
};

#endif

