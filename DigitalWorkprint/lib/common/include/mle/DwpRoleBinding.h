/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpRoleBinding.h
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


#ifndef __MLE_DWP_ROLEBINDING_H_
#define __MLE_DWP_ROLEBINDING_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This defines what type of role to create for an actor instance.  It also
 * specifies the set in which the actor is placed.  The set must match the
 * type specified by the actor definition's role set mapping.
 * 
 * WORKPRINT SYNTAX
 * EXAMPLE
 * Syntax: (RoleBinding <name> <set>)
 * 
 * <name> role class name (char *)
 * <set> set name (char *)
 * 
 * Contained by: MleDwpActor
 * Contains: none
 * END
 * 
 * @see MleDwpRoleSetMapping
 */
class MLE_DWP_API MleDwpRoleBinding : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpRoleBinding);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpRoleBinding(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpRoleBinding();

	/**
	 * Sets the type of role (C++ class name)
	 * to bind.  The role type is the same as the item name
	 * (for finding purposes).
	 */
	void setRoleType(const char *type) { setName(type); }

	/**
	 * Returns the role type.  NULL is returned
	 * if no role type has been set.  The role type is the
	 * same as the item name (for finding purposes).
	 */
	const char *getRoleType(void) const { return getName(); }

	/**
	 * Sets the set instance name to put the role in.
	 * This set should be declared elsewhere in the workprint in
	 * a MleDwpSet item.
	 */
	void setSet(const char *);

	/**
	 * Returns the set instance name.  NULL is returned
	 * if no set has been set.
	 */
	const char *getSet(void) const;

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

	char *m_set;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	//virtual int writeContents(MleDppActorGroupOutput *out);
};

#endif /* __MLE_DWP_ROLEBINDING_H_ */
