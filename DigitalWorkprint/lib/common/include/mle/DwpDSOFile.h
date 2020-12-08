/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpDSOFile.h
 * @ingroup MleDWPAccess
 *
 * This file defines the File workprint item.
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

#ifndef __MLE_DWP_DSOFILE_H_
#define __MLE_DWP_DSOFILE_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"
#include "mle/DwpFilename.h"

/**
 * This is the item that identifies a DSO file that holds a
 * Digital Workprint class.
 * 
 * <p>
 * @verbatim
 * WORKPRINT SYNTAX
 * Syntax: (DSOFile <name>)
 *         <name> actor DSO file (char *)
 * 
 * Contained by: MleDwpActorDef
 * Contains: none
 * 
 * EXAMPLE
 * END
 * @endverbatim
 * </p>
 */
class MLE_DWP_API MleDwpDSOFile : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpDSOFile);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpDSOFile(void);

    /**
	 * The destructor.
	 */
	virtual ~MleDwpDSOFile(void);

	/**
	 * Sets the name of the DSO file on the item.
	 *
	 * @param The name of the dynamically shared object.
	 */
	void setDSOFile(const char *dsofile);

	/**
	 * Retrieves the name of the DSO file for this item.
	 *
	 * @return <b>NULL</b> is returned if the DSO file has not been set.
	 * Otherwise the name of the DSO file will be returned a pointer
	 * to a character string.
	 */
	const char *getDSOFile(void) const;

	/**
	 * Create a copy of the DWP item.
	 *
	 * <i>copy()</i> is an override of <code>MleDwpItem::copy()</code>, not any new interface.
	 * This item needs its own copy function because it can contain
	 * state that is not preserved across transcription (the mechanism
	 * used in the base class copy).
	 *
	 * @param copyChildren A flag indicating whether to copy the children
	 * or not. By default, the children will be copied.
	 * @param parent The parent item to attach the copy to. By default,
	 * no parent is specified (<b>NULL</b>).
	 *
	 * @return A pointer to a copy of this DWP item is returned.
	 */
	virtual MleDwpItem *copy(int copyChildren = 1,MleDwpItem *parent = NULL);

	/**
	 * Sets a class flag to load DSO files when they
	 * are set in the item.
	 *
	 * This enables needed DSOs to be available
	 * when a workprint is read in.  This is not set by default, i.e.
	 * DSOs are not loaded when the DSO file is specified.
	 */
	static void setLoadOnSet(int flag);

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

	/** The name of the DSO file. */
	MleDwpFilename m_dsofile;

	/**
	 * Read the contents of the Digital Workprint item.
	 *
	 * @param in The object to read from.
	 *
	 * @return If the contents are successfully read, then
	 * <b>0</b> will be returned. Otherwise a nonzero value
	 * will be returned upon failure.
	 */
	virtual int readContents(MleDwpInput *in);

	/**
	 * Write the contents of the Digital Workprint item.
	 *
	 * @param in The object to write to.
	 *
	 * @return If the contents are successfully written, then
	 * <b>0</b> will be returned. Otherwise a nonzero value
	 * will be returned upon failure.
	 */
	virtual int writeContents(MleDwpOutput *out);

	/**
	 * Load the DSO file.
	 */
	void load(void);

	/** A flag indicating whether to load the DSO when the item is read. */
	static int g_loadOnSet;
};

#endif /* __MLE_DWP_DSOFILE_H_ */
