/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpCastRef.h
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


#ifndef __MLE_DWP_CASTREF_H_
#define __MLE_DWP_CASTREF_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This is the item that refers to a cat declared elsewhere.
 * This reference is meant to indicate the use of the cast in a 
 * scene and is provided to allow a single cast to be used in
 * multiple scenes.  MleDwpCastRef has no exact run time counterpart,
 * but is represented by a cast object corresponding to the named
 * MleDwpCast.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (CastRef <name>)
 * 
 * <name> name of the cast (char *)
 * 
 * Contained by: MleDwpScene
 * Contains:		
 * END
 * 
 *  @see MleDwpScene, MleDwpCast, MleDwpItem, MleCast
 */
class MLE_DWP_API MleDwpCastRef : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpCastRef);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpCastRef(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpCastRef(void);

	/**
	 * Writes a cast ref to the workprint.
	 */
	virtual int write(MleDwpOutput *out,int writeChildren = 1);

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

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	//virtual int writeContents(MleDppActorCastOutput *out);

	//virtual int write(MleDppActorCastOutput *out);
};

#endif /* __MLE_DWP_CASTREF_H_ */

