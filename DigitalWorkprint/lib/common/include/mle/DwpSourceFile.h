/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpSourceFile.h
 * @ingroup MleDWPModel
 *
 * This file defines the File workprint item.
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

#ifndef __MLE_DWP_SOURCEFILE_H_
#define __MLE_DWP_SOURCEFILE_H_

// Include system header files.
#include "mle/DwpItem.h"

/**
 * This is the item that identifies a source file that implements an
 * actor class.  It is always a subitem of a MleDwpActorDef.  There may be
 * more than one SourceFile item if there are multiple source files
 * that implement an actor class.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (SourceFile <name>)
 * 
 * <name> actor class source file (char *)
 * 
 * Contained by: MleDwpActorDef
 * Contains: none
 * END
 */
class MLE_DWP_API MleDwpSourceFile : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpSourceFile);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpSourceFile(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpSourceFile(void);

	/**
	 * Sets the source file name.  The source file name
	 * is the same as the item name (for finding purposes).
	 */
	void setFilename(const char *filename) { setName(filename); }

	/**
	 * Returns the source file name.  NULL is returned
	 * if no source file has been set.  The source file name is the
	 * same as the item name (for finding purposes).
	 */
	const char *getFilename(void) const { return getName(); }

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
};

#endif /* __MLE_DWP_SOURCEFILE_H_ */
