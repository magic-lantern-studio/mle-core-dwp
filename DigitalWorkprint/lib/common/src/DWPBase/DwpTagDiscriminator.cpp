/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpDiscriminator.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the discriminator used by the Magic Lantern Digital
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Includee system header files.
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/DwpTagDiscriminator.h"
#include "mle/DwpItem.h"


MleDwpTagDiscriminator::MleDwpTagDiscriminator(void)
{
	// Initialize variables.
	m_tagList = NULL;
}

MleDwpTagDiscriminator::~MleDwpTagDiscriminator()
{
	// Get rid of all the tags.
	while ( m_tagList )
	{
		_MleDwpTag *tmp = m_tagList;
		m_tagList = m_tagList->m_next;

		delete tmp->m_tag;
		delete tmp;
	}
}

MleDwpTagDiscriminator::State
MleDwpTagDiscriminator::discriminate(const MleDwpItem *item)
{
	// If disabled, item is always used.
	if (m_enabled == FALSE)
		return USE;

	// An item with no tags matches everything.
	if ( !item->hasAnyTag() )
		return USE;

	// Loop over tags.
	_MleDwpTag *tag = m_tagList;
	while ( tag )
	{
		if ( item->hasTag(tag->m_tag) )
			return USE;
		tag = tag->m_next;
	}

	return SKIP;
}

void
MleDwpTagDiscriminator::addTag(const char *tag)
{
	MLE_ASSERT(tag);

	// Check to see if this tag is already here.
	_MleDwpTag *entry = m_tagList;
	while ( entry )
	{
		if ( !strcmp(entry->m_tag,tag) )
			return;
		
		entry = entry->m_next;
	}
	
	// Otherwise, prepend a new list entry.
	entry = new _MleDwpTag;
	entry->m_tag = strcpy(new char[strlen(tag) + 1],tag);
	entry->m_next = m_tagList;
	m_tagList = entry;
}


void
MleDwpTagDiscriminator::removeTag(const char *tag)
{
	MLE_ASSERT(tag);

	_MleDwpTag **entry = &m_tagList;
	while ( *entry )
	{
		// Compare for a match.
		if ( !strcmp((*entry)->m_tag,tag) )
		{
			// Save the pointer.
			_MleDwpTag *tmp = *entry;

			// Do list removal.
			*entry = (*entry)->m_next;

			// Do teardown and deletion.
			delete tmp->m_tag;
			delete tmp;

			break;
		}

		entry = &(*entry)->m_next;
	}
}


void *
MleDwpTagDiscriminator::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpTagDiscriminator::operator delete(void *p)
{
	mlFree(p);
}


void *
_MleDwpTag::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
_MleDwpTag::operator delete(void *p)
{
	mlFree(p);
}
