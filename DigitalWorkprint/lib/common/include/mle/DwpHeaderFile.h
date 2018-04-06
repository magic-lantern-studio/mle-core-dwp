/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpHeaderFile.h
 * @ingroup MleDWPModel
 *
 * This file defines the File workprint item.
 *
 * @author Mark S. Millard
 * @date July 10, 2003
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

#ifndef __MLE_DWP_HEADERFILE_H_
#define __MLE_DWP_HEADERFILE_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This is the item that identifies the header file that declares an
 * actor class.  It is always a subitem of an ActorDef.  The header
 * file information is used by the mastering process.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax: (HeaderFile <name>)
 * 
 * <name> actor class header file (char *)
 * 
 * Contained by: MleDwpActorDef
 * Contains: none
 * END
 */
class MLE_DWP_API MleDwpHeaderFile : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpHeaderFile);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpHeaderFile(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpHeaderFile(void);

	/**
	 * Sets the header file. The header file is the same
	 * as the item name (for finding purposes).
	 */
	void setHeader(const char *header) { setName(header); }

	/**
	 * Returns the header file.  NULL is returned if no
	 * header file has been set.  The header file is the same as the
	 * item name (for finding purposes).
	 */
	const char *getHeader(void) const { return getName(); }

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

#endif

