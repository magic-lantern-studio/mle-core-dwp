/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpDelegateAttachment.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @deprecated The MleDwpDelegateAttachment class has been replaced with
 * MleDwpRoleAttachment class.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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


#ifndef __MLE_DWP_DELEGATEATTACHMENT_H_
#define __MLE_DWP_DELEGATEATTACHMENT_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This defines a parent and child relationship for the delegates of
 * two actors.  The delegates are identified by the names of their
 * actors, which must be declared in the same Group.  Although
 * this pairwise linkage specification is general enough to define
 * any directed graph, it is currently considered an error to give
 * a delegate more than one parent.

 * WORKPRINT SYNTAX

 * EXAMPLE
 * Syntax: (DelegateAttachment <parent> <child>)

 * <parent> name of owning actor for parent delegate (char *)
 * <child> name of owning actor for child delegate (char *)

 * Contained by: MleDwpGroup
 * Contains: none
 * END
 * 
 */
class MLE_DWP_API MleDwpDelegateAttachment : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpDelegateAttachment);

  public:
	  
	int m_parentIndex;

	MleDwpDelegateAttachment(void);

	virtual ~MleDwpDelegateAttachment();

	/**
	 * Sets the name of the parent actor.
	 */
	void setParent(const char *parent);

	/**
	 * Sets the name of the child actor.
	 */
	void setChild(const char *child);

	/**
	 * Returns the name of the parent actor.  NULL is
	 * returned if no parent actor has been set.
	 */
	const char *getParent(void) const;

	/**
	 * Returns the name of the child actor.  NULL is
	 * returned if no child actor has been set.
	 */
	const char *getChild(void) const;

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

	//virtual int writeContents(MleDppActorGroupOutput *out);

  private:

	char *m_parent;
	char *m_child;
};

#endif /* __MLE_DWP_DELEGATEATTACHMENT_H_ */
