/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpTagDiscriminator.h
 * @ingroup MleDWPAccess
 *
 * This file defines a discriminator used by the Magic Lantern Digital
 * Workprint Library API.
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

#ifndef __MLE_DWP_TAGDISCRIMINATOR_H_
#define __MLE_DWP_TAGDISCRIMINATOR_H_

// Include Digital Workprint header files.
#include "mle/DwpDiscriminator.h"

// Define the linked list item for item tags
struct MLE_DWP_API _MleDwpTag
{
	char *m_tag;
	_MleDwpTag *m_next;

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
};

/**
 * This is a discriminator that uses tags. It operates under the following
 * rules:
 * <li>
 * <ol>If any of the tags set on the discriminator is set on an item,
 * the discriminator will return USE.</ol>
 * <ol>If an item has no tags, the discriminator will return USE.</ol>
 * <ol>If an item has tags, but they match no tags on the discriminator,
 * the discriminator will return skip.</ol>
 * </li>
 *
 * @see MleDwpItem, MleDwpDiscriminator
 */
class MLE_DWP_API MleDwpTagDiscriminator : public MleDwpDiscriminator
{
  public:

	/**
	 * The default constructor.
	 */
	MleDwpTagDiscriminator(void);

	/**
	 * The destructor.
	 */
    virtual ~MleDwpTagDiscriminator(void);

	/**
	 * This is the discrimination function.
	 */
	virtual State discriminate(const MleDwpItem *item);

	/**
	 * Adds a string tag to the discriminator.
	  */
	void addTag(const char *tag);

	/**
	 * Removes the string tag from the discriminator.
	 */
	void removeTag(const char *tag);

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

	/** The list of tags. */
	_MleDwpTag *m_tagList;
};

#endif /* __MLE_DWP_TAGDISCRIMINATOR_H_ */
