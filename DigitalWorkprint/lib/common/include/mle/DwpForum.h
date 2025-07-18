/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpForum.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @deprecated The MleDwpForum class has been replaced with the
 * MleDwpSet class.
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


#ifndef __MLE_DWP_FORUM_H_
#define __MLE_DWP_FORUM_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This defines a forum instance.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (Forum <name> <type>)
 * 
 * <name> title-specific forum name (char *)
 * <type> forum class name (char *)
 * 
 * Contained by: none
 * Contains: MleDwpProperty
 * END
 * 
 * @see MleDwpForumDef
 */
class MLE_DWP_API MleDwpForum : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpForum);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpForum(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpForum(void);

	/**
	 * Sets the forum type (C++ class).  There is usually
	 * also a MleDwpForumDef instance with the type as a name in the
	 * same workprint.
	 */
	void setType(const char *type);

	/**
	 *  Returns the forum type (C++ class).  NULL is returned
	 * if no forum type has been set.
	 */
	const char *getType(void) const;

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

	char *m_type;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	//virtual int writeContents(MleDppActorGroupOutput *out);

	//virtual int write(MleDppActorGroupOutput *out);

};

#endif /* __MLE_DWP_FORUM_H_ */

