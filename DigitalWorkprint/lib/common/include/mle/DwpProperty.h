/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpProperty.h
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


#ifndef __MLE_DWP_PROPERTY_H_
#define __MLE_DWP_PROPERTY_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"
#include "mle/DwpDataUnion.h"

//class MleDwpActorMember;
class MleDwpDatatype;

/**
 * This is the actor property workprint item.  It holds initial property
 * values for actor instances.
 *
 * WORKPRINT SYNTAX
 *
 * EXAMPLE
 * Syntax:		(Property <name> <type> <value>)
 *
 * <name>	property name (char *)
 * <type>  property type (char *)
 * <value>	property value (see below)
 *
 * Contained by:	MleDwpActor
 * Contains:		none
 * END
 *
 * Data is held within the property in a MleDwpDataUnion, which is
 * just a struct that contains a union.  The MleDwpProperty item does
 * not operate on this object directly, instead employing a data
 * type object to execute reads, writes, and copies.
 *
 * @see MleDwpDataUnion, MleDwpDatatype
 */
class MLE_DWP_API MleDwpProperty : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpProperty);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpProperty(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpProperty(void);

	// Datatype info.

	/**
	 * Returns a pointer to the current datatype object.
	 */
	const MleDwpDatatype *getDatatype(void) const;

	/**
	 * Sets the datatype of this property.  This
	 * may be done either with a pointer to a datatype object or
	 * a datatype name.  This will in turn set the datatype on
	 * the data union member variable.
	 */
	void setDatatype(const MleDwpDatatype *datatype);

	void setDatatype(const char *datatype);

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

	// The data container.
	MleDwpDataUnion m_data;

  protected:

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	//virtual int writeContents(MleDppActorGroupOutput *out);
};

#endif /* __MLE_DWP_PROPERTY_H_ */
