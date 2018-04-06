/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpRoleSetMapping.h
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


#ifndef __MLE_DWP_ROLESETMAPPING_H_
#define __MLE_DWP_ROLESETMAPPING_H_


// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This defines what type of role to create for an actor instance,
 * and what type of set it should go into.  The information will be
 * used to generate a MleDwpRoleBinding, which must choose a set
 * instance of the specified set type.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (RoleSetMapping <type> <set>)
 * 
 * <type> role class name (char *)
 * <set> set name (char *)
 * 
 * Contained by: MleDwpActorDef, MleDwpActorTemplate
 * Contains: none
 * 
 * @see MleDwpRoleBinding
 */
class MLE_DWP_API MleDwpRoleSetMapping : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpRoleSetMapping);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpRoleSetMapping(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpRoleSetMapping();

	/**
	 * Sets the role type (C++ class).  The role
	 * type is also the item name (for finding purposes).
	 */
	void setRoleClass(const char *delegate) { setName(delegate); }

	/**
	 * Returns the role type (C++ class).  NULL
	 * is returned if the role type has not been set.  The
	 * role type is also the item name ( for finding purposes).
	 */
	const char *getRoleClass(void) const { return getName(); }

	/**
	 * Sets the set type (C++ class) that the role
	 * should go into.
	 */
	void setSet(const char *);

	/**
	 * Returns the set type.  NULL is returned if no
     * Set is set.
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
};

#endif /* __MLE_DWP_ROLESETMAPPING_H_ */

