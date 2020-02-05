/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpGroupRef.h
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


#ifndef __MLE_DWP_GROUPREF_H_
#define __MLE_DWP_GROUPREF_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This is the item that refers to a group declared elsewhere.
 * This reference is meant to indicate the use of the group in a 
 * scene and is provided to allow a single group to be used in
 * multiple scenes.  MleDwpGroupRef has no exact run time counterpart,
 * but is represented by a group object corresponding to the named
 * MleDwpGroup.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (GroupRef <name>)
 * 
 * <name> name of the group (char *)
 * 
 * Contained by: MleDwpScene
 * Contains:		
 * END
 * 
 *  @see MleDwpScene, MleDwpGroup, MleDwpItem, FwGroup
 */
class MLE_DWP_API MleDwpGroupRef : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpGroupRef);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpGroupRef(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpGroupRef(void);

	/**
	 * Writes a group ref to the workprint.
	 */
	virtual int write(MleDwpOutput *out,int writeChildren = 1);

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

	//virtual int write(MleDppActorGroupOutput *out);
};

#endif /* __MLE_DWP_GROUPREF_H_ */

