/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpMediaRefTarget.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
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


#ifndef __MLE_DWP_MEDIAREFTARGET_H_
#define __MLE_DWP_MEDIAREFTARGET_H_

// Include DigitalWorkprint header files.
#include "mle/DwpItem.h"

/**
 * MleDwpMediaRefTarget provides information on how the media should be
 * referenced by the target player.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * MediaRefTarget <tags> <flags>
 *   (Media ...)
 *   ...
 *   (Converter ...)
 * )
 * 
 * <tags>::= target discriminator (a string,i.e. +rehearsal,+sgi,+win32)
 * <flags>::= target info (an integer,used for internal/external flag)
 * 
 * Contained by: MleDwpMediaRef
 * Contains:     MleDwpMedia and MleDwpConverter
 * END
 * 
 * @see MleDwpMediaRef, MleDwpMedia, MleDwpConverter
*/
class MLE_DWP_API MleDwpMediaRefTarget : public MleDwpItem
{
    MLE_DWP_HEADER(MleDwpMediaRefTarget);

  public:

    MleDwpMediaRefTarget(void);

    virtual ~MleDwpMediaRefTarget(void);

    // Accessors
    void setFlags(const int value);

    int getFlags(void) const;

    virtual int readContents(MleDwpInput *in);

    virtual int writeContents(MleDwpOutput *out);

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

  private:

    int m_flags;
};


#endif /* __MLE_DWP_MEDIAREFTARGET_H_ */
