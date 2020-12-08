/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpFinder.h
 * @ingroup MleDWPAccess
 *
 * This file defines a utility used by the Magic Lantern Digital
 * Workprint Library API to find Workprint items.
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

#ifndef __MLE_DWP_FINDER_H_
#define __MLE_DWP_FINDER_H_


// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * Typedef of the callback function that can be optionally used by a MleDwpFinder.
 */
typedef int (*MleDwpFinderCallback)(MleDwpItem *,void *);

/**
 * This class is a utility object that finds items in a MleDwpItem hierarchy.
 *
 * Items may be found by name, by type, or by other criteria by passing
 * in a callback function.  Here is an example usage that finds an
 * actor Group named "group17":
 * <p>
 * @code
 *    MleDwpItem *rootItem;
 *    ...
 *    MleDwpFinder finder;
 *    finder.setName("group17");
 *    finder.setType(MleDwpGroup::typeId);
 *    finder.find(rootItem);
 *
 *    MleDwpItem *group = finder.getItem();
 * @endcode
 * </p><p>
 * This example showed finding by name and type in combination.  Name
 * and type can also be used exclusively.  Either the first item found
 * or all items matching the find criteria can be returned.
 * </p>
 *
 * @see MleDwpItem
 */
class MLE_DWP_API MleDwpFinder
{
  public:

	/**
	 * The constructor takes the string name of the item class
	 * to look for, the string name of the item instance to
	 * look for, and the flag to find all matching items.
	 *
	 * @param type If type is <b>NULL</b>, any type will match.
	 * @param name If name is <b>NULL</b>, any name will match.
	 * @param findAll If findAll is nonzero, the finder will find all matching
	 * items in the hierarchy.
	 */
	MleDwpFinder(const char * type, const char *name = NULL,
		int findAll = 0);

	/**
	 * The constructor takes the type id of the item class
	 * to look for, the string name of the item instance to
	 * look for, and the flag to find all matching items.
	 *
	 * @param type If type is <b>NULL</b>, any type will match.
	 * @param name If name is <b>NULL</b>, any name will match.
	 * @param findAll If findAll is nonzero, the finder will find all matching
	 * items in the hierarchy.
	 */
	MleDwpFinder(MleDwpType type = NULL, const char *name = NULL,
		int findAll = 0);

	/**
	 * The destructor.
	 */
	~MleDwpFinder();

	/**
	 * Execute a search.
	 *
	 * <i>find()</i> is passed the top of a workprint hierarchy.  The
	 * result of the find operation is retained within the finder
	 * object.  For convenience, the first matching item found
	 * is the return value.
	 *
	 * @param top The top of the workprint hierarchy to being searching.
	 *
	 * @return The first matching item found is returned. <b>NULL</b>
	 * will be returned if now matching item is found.
	 */
	virtual MleDwpItem *find(MleDwpItem *top);

	/**
	 * Set search type.
	 *
	 * <i>setType()</i> can be used to find a particular item
	 * type.  If the type is not set or is set to <b>NULL</b>,
	 * then it is not used as criteria.  This parameter may
	 * also be set in the constructor when the finder is created.
	 *
	 * @param type The workprint type to set.
	 */
	void setType(const char *type);

	/**
	 * Set search type.
	 *
	 * <i>setType()</i> can be used to find a particular item
	 * type.  If the type is not set or is set to <b>NULL</b>,
	 * then it is not used as criteria.  This parameter may
	 * also be set in the constructor when the finder is created.
	 *
	 * @param type The workprint type to set.
	 */
	void setType(MleDwpType type);

	/**
	 * Set search name.
	 *
	 * <i>setName()</i> can be used to find a particular item
	 * name.  If the name is  not set or is set to <b>NULL</b>,
	 * then it is not used as criteria.  This parameter may
	 * also be set in the constructor when the finder is created.
	 *
	 * @param name A pointer to a character string representing the
	 * name of the workprint to set.
	 */
	void setName(const char *name);

	/**
	 * <i>setCallback()</i> allows the application to pass in a function
	 * to implement a search criterion.  The function will receive
	 * a pointer to every item on the traversal and the client data
	 * argument passed in.  The function should return nonzero on
	 * a match.
	 *
	 * @param cb A pointer to the callback.
	 * @param data A pointer to client data to associate with the callback.
	 */
	void setCallback(MleDwpFinderCallback cb,void *data);

	/**
	 * <i>setFindAll()</i> configures find termination.  If the argument
	 * passed is nonzero, all items matching the criteria will
	 * be found.  Otherwise, only the first matching item will
	 * be found.  This parameter may also be set in the constructor
	 * when the finder is created.
	 *
	 * @param flag A flag indicating whether all items matching the criteria
	 * should be found, or just the first one.
	 */
	void setFindAll(int flag);

	/**
	 * Get first result of search.
     *
	 * <i>getItem()</i> returns a pointer to the first item found (whether
	 * the finder terminated after the first item or not).
	 *
	 * @return It returns <b>NULL</b> if no items were found. If an item
	 * matching the criteria is found, the a pointer to it will be
	 * returned.
	 */
	MleDwpItem *getItem(void) const;

	/**
	 * Get results of search.
	 *
	 * @return A pointer to an array of the item pointers is returned.
	 * If no items matching the criteria were found, then
	 * <b>NULL</b> will be returned.
	 */
	MleDwpItem **getItems(void) const;

	/**
	 * Get the number of items found.
     *
	 * @return The number of items found is returned.
	 */
	int getNumItems(void) const;

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

	/** The name of the item to find. */
	char *m_name;
	/** The type of item to find. */
	char *m_typeString;
	/** The type of item to find. */
	MleDwpType m_type;
	/** Flag indicating whether to find all matching items, or just the first one. */
	int m_findAll;

	/** The callback to use when each item is found. */
	MleDwpFinderCallback m_callback;
	/** Client data ssociated with the callback. */
	void *m_cbdata;

	/** The number of items found. */
	int m_numItems;
	/** A pointer to an array of found items. */
	MleDwpItem **m_items;

	/**
	 * Execute the search algorithm.
	 *
	 * @param finder A reference to the finder specifying the search
	 * criteria.
	 * @param item The top of the workprint hierarchy to begin
	 * execution against.
	 */
	static int execute(MleDwpFinder *finder,MleDwpItem *item);
};

#endif /* __MLE_DWP_FINDER_H_ */
