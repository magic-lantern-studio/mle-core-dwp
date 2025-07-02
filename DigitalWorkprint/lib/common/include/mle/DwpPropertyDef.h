/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpPropertyDef.h
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


#ifndef __MLE_DWP_PROPERTYDEF_H_
#define __MLE_DWP_PROPERTYDEF_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

class MleDwpDatatype;

/**
 * This defines a property on an actor or forum class.  This is always a
 * subitem of MleDwpActorDef or MleDwpForumDef.  The property def item exists for
 * the benefit of the tools to create useful interfaces for actors.  It
 * is not needed by the rehearsal runtime or the mastering process.

 * WORKPRINT SYNTAX

 * EXAMPLE
 * Syntax: (PropertyDef <name> <type>)

 * <name> property name (char *)
 * <type> property datatype (char *)

 * Contained by: MleDwpActorDef or MleDwpForumDef
 * Contains: MleDwpLimits, MleDwpEnumDef
 * END
 *
 * @see MleDwpProperty
 */
class MLE_DWP_API MleDwpPropertyDef : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpPropertyDef);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpPropertyDef(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpPropertyDef(void);

	/**
	 * Sets the datatype name (subclass of MleDwpDatatype).
	 */
	void setType(const char *);

	/**
	 * Returns the datatype name.  NULL is returned if the
	 * datatype has not been set.
	 */
	const char *getType(void) const;

	/**
	 * Returns the datatype object for the property.
	 */
	const MleDwpDatatype *getDatatype(void) const;

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
};

#endif /* __MLE_DWP_PROPERTYDEF_H_ */
