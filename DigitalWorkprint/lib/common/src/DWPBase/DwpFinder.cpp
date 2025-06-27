/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpFinder.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements a utility used by the Magic Lantern Digital
 * Workprint Library API to find Workprint items.
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/DwpFinder.h"
#include "mle/DwpItem.h"


MleDwpFinder::MleDwpFinder(const char *t,const char *n,int flag)
{
	/* Initialize data members. */
	m_name = NULL;
	m_typeString = NULL;
	m_type = NULL;
	m_callback = NULL;

	m_numItems = 0;
	m_items = NULL;

	/* Initialize from constructor arguments. */
	if ( t ) setType(t);
	if ( n ) setName(n);

	m_findAll = flag;
}

MleDwpFinder::MleDwpFinder(MleDwpType t,const char *n,int flag)
{
	/* Initialize data members. */
	m_name = NULL;
	m_typeString = NULL;
	m_type = NULL;
	m_callback = NULL;

	m_numItems = 0;
	m_items = NULL;

	/* Initialize from constructor arguments. */
	if ( t ) setType(t);
	if ( n ) setName(n);

	m_findAll = flag;
}


MleDwpFinder::~MleDwpFinder()
{
	/* Reclaim memory. */
	if ( m_name ) mlFree(m_name);
	if ( m_typeString ) mlFree(m_typeString);

	if ( m_items ) mlFree(m_items);
}

/*
 * This function executes the find.  Note that this function must
 * be called in order to get any results.  The pointer to the first
 * item found, if any, will be returned.
 */
MleDwpItem *
MleDwpFinder::find(MleDwpItem *top)
{
	MLE_ASSERT(top);

	/* Clear the current list of items. */
	m_numItems = 0;

	/* Call the static function to do the search. */
	execute(this,top);

	if ( m_numItems )
		return m_items[0];
	else
		return NULL;
}

/*
 * This function sets an item name to look for.  If NULL is passed
 * in, names are disregarded for the find.
 */
void
MleDwpFinder::setName(const char *n)
{
	/* Free memory if necessary. */
	if ( m_name ) mlFree(m_name);

	/* Set the new name. */
	m_name = n ? strdup(n) : NULL;
}

/*
 * This function sets an item type to look for.  If NULL is passed
 * in, types are disregarded for the find.
 */
void
MleDwpFinder::setType(MleDwpType t)
{
	/* Free memory if necessary. */
	if ( m_typeString ) mlFree(m_typeString);

	m_type = t;
}

/*
 * This function sets an item type to look for.  If NULL is passed
 * in, types are disregarded for the find.
 */
void
MleDwpFinder::setType(const char *t)
{
	m_type = NULL;

	m_typeString = strdup(t);
}

/*
 * This function sets the user-defined callback to use to
 * determine if an item should be found.  The callback will
 * be called with an item on the traversal and the client
 * data.  The callback function should return nonzero to
 * indicate a match.
 */
void
MleDwpFinder::setCallback(int (*cb)(MleDwpItem *,void *),void *data)
{
	MLE_ASSERT(cb);

	m_callback = cb;
	m_cbdata = data;
}

/*
 * This function sets or unsets the findAll flag on the finder.  If
 * the flag is nonzero, all items matching the criteria will be
 * available through the finder.  Otherwise, traversal will terminate
 * after the first match is found.
 */
void
MleDwpFinder::setFindAll(int flag)
{
	m_findAll = flag;
}

/*
 * This function returns the first item matched after a find().
 * NULL is returned if there are none.
 */
MleDwpItem *
MleDwpFinder::getItem(void) const
{
	/* Return the first item if there are any. */
	return m_numItems ? m_items[0] : NULL;
}

/*
 * This function returns a pointer to an array of item pointers
 * matched after a find().
 */
MleDwpItem **
MleDwpFinder::getItems(void) const
{
	/* Return the item list. */
	return m_items;
}

/*
 * This function returns the number of items matched after a find().
 */
int
MleDwpFinder::getNumItems(void) const
{
	return m_numItems;
}

/*
 * This private static member function implements the find procedure.
 */
int
MleDwpFinder::execute(MleDwpFinder *finder,MleDwpItem *item)
{
	MLE_ASSERT(finder);
	MLE_ASSERT(item);

	int rval = 0;
	int pass = 1;

	/* Test for type. */
	if ( pass && finder->m_type )
		pass = (pass && item->isa(finder->m_type));
	else if ( pass && finder->m_typeString )
		pass = (pass && item->isa(finder->m_typeString));

	/* Test for name. */
	if ( pass && finder->m_name )
		pass = (pass && !strcmp(finder->m_name,item->getName()));
	
	/* Test for user-supplied function. */
	if ( pass && finder->m_callback )
		pass = (pass && (*finder->m_callback)(item,finder->m_cbdata));

	/* Add this item to the list. */
	if ( pass )
	{
		/* Set return flag. */
		rval = 1;

		finder->m_items = (MleDwpItem **)mlRealloc(finder->m_items,
			(finder->m_numItems + 1)*sizeof(MleDwpItem *));
		
		finder->m_items[finder->m_numItems] = item;
		finder->m_numItems++;

		/* Terminate if findAll flag not set. */
		if ( finder->m_findAll == 0 )
			return 1;
	}

	/* Recurse over children. */
	MleDwpItem *child = item->getFirstChild();
	while ( child )
	{
		/* Terminate on success if findAll flag is not set. */
		if ( MleDwpFinder::execute(finder,child) )
		{
			rval = 1;
			
			if (finder->m_findAll == 0 )
				return 1;
		}

		child = child->getNext();
	}

	return rval;
}


void *
MleDwpFinder::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpFinder::operator delete(void *p)
{
	mlFree(p);
}


void*
MleDwpFinder::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}


void
MleDwpFinder::operator delete[](void* p)
{
	mlFree(p);
}
