/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpUnknown.h
 * @ingroup MleDWPAccess
 *
 * This file defines the unknwown workprint item used by the Magic Lantern Digital
 * Workprint Library API.
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

#ifndef __MLE_DWP_UNKNOWN_H_
#define __MLE_DWP_UNKNOWN_H_


// Include Digital Workprint header files.
#include "mle/Dwp.h"
#include "mle/DwpItem.h"


/**
 * @brief An unknown workprint item.
 *
 * When a workprint item is read in and its type is not recognized, the
 * reader will try to locate a DSO for the type.  If that is unsuccessful,
 * the reader will create an instance of MleDwpUnknown in place of that
 * item.
 *
 * The contents of the unknown item are simply the verbatim data (including
 * subitems) through the item delimiter.  This will be written back out
 * with the original type heading.  In this manner, most workprints containing
 * unknown types can be read and written without harming their meaning.
 */
class MLE_DWP_API MleDwpUnknown : public MleDwpItem
{
  public:

	/**
	 * @brief The constructor.
	 *
	 * @param type A pointer to a character string identifying the type of
	 * digital workprint item.
	 */
	MleDwpUnknown(const char *type);

	/**
	 * @brief The destructor.
	 */
	~MleDwpUnknown(void);

	/**
	 * @brief Write the unknown workprint item to the specified output
	 * sink
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

  private:

	char *m_type;
	char *m_buffer;
	int m_size;
	int m_allocSize;
};


#endif /* __MLE_DWP_UNKNOWN_H_ */
