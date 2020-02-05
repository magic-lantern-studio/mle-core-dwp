/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpDiscriminator.h
 * @ingroup MleDWPAccess
 *
 * This file defines the discriminator used by the Magic Lantern Digital
 * Workprint Library API.
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

#ifndef __MLE_DWP_DISCRIMINATOR_H_
#define __MLE_DWP_DISCRIMINATOR_H_

// Include Digital Workprint header files.
#include "mle/Dwp.h"


class MleDwpItem;


/**
 * @brief This class is used with the Magic Lantern Digital Workprint
 * item to discrimate workprint traversal.
 *
 * This is the base class for an object that affects traversal of a
 * workprint tree.  A <code>MleDwpDiscriminator</code> may be installed by calling
 * <code>MleDwpItem::setDiscriminator()</code>, which globally modifies the behavior
 * of the <code>MleDwpItem</code> traversal calls <i>getParent()</i>, <i>getNext()</i>, <i>getPrev()</i>,
 * <i>getFirstChild()</i>, and <i>getLastChild()</i>.
 * <p>
 * Discrimination has three states.  The default value is <b>USE</b>,
 * which doesn't not affect traversal at all.  <b>PHANTOM</b> means
 * that all the children of the item are considered to be where the
 * item is.  This is useful for collapsing unwanted hierarchy, such
 * as for file inclusion nodes.  <>bSKIP</b> means that the entire
 * hierarchy at the item should be disregarded.
 * </p><p>
 * A discriminator can be disabled, in which case, it should always
 * return <b>USE</b>.
 * </p><p>
 * This base-class discriminator has a "discriminate none" tag - always 
 * return <b>USE</b>.
 * </p>
 *
 * @see MleDwpItem, MleDwpTagDiscriminator.
 */
class MLE_DWP_API MleDwpDiscriminator
{

  public:

	/**
	 * @brief An enumeration specifying the state of discrimination.
	 */
	enum State {
		USE,        /**< The discriminator is in use. */
		PHANTOM,    /**< The discriminator is lurking. */
		SKIP,       /**< The discriminator is being skipped. */
	};


	/**
	 * @brief The default constructor.
	 *
	 * By default, the discriminator is enabled.
	 */
	MleDwpDiscriminator(void) { m_enabled = 1; }

	/**
	 * @brief The evaluation function.
	 *
	 * This is the key function for the discriminator.
	 *
	 * @param item A pointer to the workprint item being
	 * discriminated.
	 *
	 * @return State The state of the discrimination is returned.
	 */
	virtual State discriminate(const MleDwpItem *item);

	/**
	 * @brief Check if the discriminator is enabled.
	 *
	 * @return Indication of disciminator enabled state is returned.
	 * If the discriminator is enabled, <b>TRUE</b> will be returned,
	 * Otherwise, <b>FALSE</b> will be returned.
	 */
	short getEnabled(void) { return m_enabled; };

	/**
	 * @brief Enable or disable discriminator, returning previous state.
	 *
	 * @param flag The state to set the discriminator in.
	 *
	 * @return The previous state is returned.
	 */
	short setEnabled(short flag)
		{ short temp = m_enabled; m_enabled = flag; return temp; };

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

	/** Flag indicating if this discriminator is enabled. */
	short m_enabled;
};


#endif /* __MLE_DWP_DISCRIMINATOR_H_ */
