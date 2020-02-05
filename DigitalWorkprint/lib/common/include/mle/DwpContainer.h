/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpContainer.h
 * @ingroup MleDWPAccess
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

#ifndef __MLE_DWP_CONTAINER_H_
#define __MLE_DWP_CONTAINER_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * @brief This is a Digital Workprint item that is a pure container.
 * 
 * A Container may be used anywhere for grouping convenience.
 * An instance may be configured either to write itself out or to write
 * only its children.  By default, the item is write transparent, which
 * simply means that it doesn't write itself as an item to the output.
 * <p>
 * The <i>readAll()</i> call in <code>MleDwpItem</code> creates a Container item
 * with transparency on to hold all the items read from an input object.
 * </p><p>
 * @verbatim
 * WORKPRINT SYNTAX
 * Syntax: (Container)
 *
 * Contained by: any
 * Contains:     any
 *
 * EXAMPLE
 * END
 * @endverbatim
 * </p>
 *
 * @see MleDwpItem
 */
class MLE_DWP_API MleDwpContainer : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpContainer);

  public:

	/**
	 * The default constructor.
	 * <p>
	 * It sets transparency transcription on by default.
	 * </p>
	 */
	MleDwpContainer(void);

	/**
	 * The destructor.
	 */
    virtual ~MleDwpContainer(void);

	/**
	 * Set Container's transparency
	 * <p>
	 * If on (nonzero), the Container will
	 * disappear during transcription to/from files.  By default,
	 * transparency is on.
	 * </p>
	 *
	 * @param onOrOff The transparency value for transcription.
	 */
	void setTransparent(int onOrOff);

	/**
	 * Copy the Container.
	 * <p>
	 * This workprint item needs its own copy function to handle
	 * transparency correctly, but it doesn't add visible functionality
	 * over the base class definition.
	 * </p>
	 *
	 * @param copyChildren A flag indicating whether to copy the children
	 * or not. By default, the children will be copied.
	 * @param parent The parent item to attach the copy to. By default,
	 * no parent is specified (<b>NULL</b>).
	 *
	 * @return A pointer to a copy of this Container is returned.
	 */
	virtual MleDwpItem *copy(int copyChildren = 1,MleDwpItem *parent = NULL);

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

	/** Transparency boolean. */
	int m_transparent;

	/**
	 * Write the Container to the specified output.
	 *
	 * @param out The object to write the Container to.
	 * @param writeChildren A flag indicating whether to write the
	 * children or not. If <b>1</b>, then the children will be written.
	 * If <b>0</b>, then the children will not be written.
	 *
	 * @return If the Container is successfully written, then <b>0</b>
	 * will be returned. Otherwise, a nonzero value will be returned
	 * upon failure.
	 */
	virtual int write(MleDwpOutput *out,int writeChildren);
};

#endif /* __MLE_DWP_CONTAINER_H_ */

