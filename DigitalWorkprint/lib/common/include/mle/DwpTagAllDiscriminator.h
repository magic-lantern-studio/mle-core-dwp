/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpTagAllDiscriminator.h
 * @ingroup MleDWPAccess
 *
 * This file defines a discriminator used by the Magic Lantern Digital
 * Workprint Library API. The descriminator uses tags to discriminate.
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

#ifndef __MLE_DWP_TAGALLDISCRIMINATOR_H_
#define __MLE_DWP_TAGALLDISCRIMINATOR_H_

// Inlucde Digital Workprint header files.
#include "mle/DwpTagDiscriminator.h"

/**
 * This is the discriminator used by Magic Lantern tools.
 *
 * Discrimination is done by the following method:
 * <li>
 * <ol>If an item has no tags, it's shown.</ol>
 * <ol>All of an item's tags have to be in the discriminator.  This is
 * an AND, i.e. the item tags have to be a subset of the 
 * discriminator tags.</ol>
 * </li>
 *
 * @see MleDwpTagDiscriminator, MleDwpDiscriminator
 */
class MLE_DWP_API MleDwpTagAllDiscriminator : public MleDwpTagDiscriminator
{
  public:

	/**
	 * This is the discrimination function.
	 */
	virtual State discriminate(const MleDwpItem *item);

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

	int hasTag(const char *itemTagString);
};

#endif /* __MLE_DWP_TAGALLDISCRIMINATOR_H_ */
