/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpForumDef.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 *
 * @deprecated The MleDwpForumDef class has been replaced with the 
 * MleDwpSetDef class.
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


#ifndef __MLE_DWP_FORUMDEF_H_
#define __MLE_DWP_FORUMDEF_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This is the definition object for an forum class.  It is really just a
 * container with a type name - subitems complete the definition.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (ForumDef <name>)
 * 
 * <name> forum class name (char *)
 * 
 * Contained by: none
 * Contains: MleDwpHeaderFile, MleDwpSourceFile, MleDwpDSOFile,
 * MleDwpPropertyDef, etc.
 * END
 * 
 * @see MleDwpForum
 */
class MLE_DWP_API MleDwpForumDef : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpForumDef);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpForumDef(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpForumDef(void);

	/**
	 * Sets the name of the forum class (this is a
	 * C++ class).  This is also the item name (for finding purposes).
	 */
	void setForumClass(const char *fc) { setName(fc); }

	/**
	 * Returns the name of the forum class.  NULL
	 * is returned if it has not been set.  This is also the item
	 * name (for finding purposes).
	 */
	const char *getForumClass(void) const { return getName(); }

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
};

#endif /* __MLE_DWP_FORUMDEF_H_ */

