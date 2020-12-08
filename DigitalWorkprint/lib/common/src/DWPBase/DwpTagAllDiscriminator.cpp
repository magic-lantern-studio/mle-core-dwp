/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpTagAllDiscriminator.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements a discriminator used by the Magic Lantern Digital
 * Workprint Library API. The descriminator uses tags to discriminate.
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
#include "mle/DwpTagAllDiscriminator.h"
#include "mle/DwpItem.h"

//
// Discriminates by the following method:
//  1. if item has no tags, it's shown
//  2. all item's tags have to be in the discriminator; this is
//     an AND, i.e., the item tags have to be a subset of the 
//     discriminator tags.
//

MleDwpDiscriminator::State
MleDwpTagAllDiscriminator::discriminate(const MleDwpItem *item)
{
	// If disabled, item is always used.
	if (m_enabled == FALSE)
		return USE;

	// An item with no tags matches everything.
	if ( !item->hasAnyTag() )
		return USE;

	// Loop over item tags.
	int n = 0;
	const char *tag;
	while ((tag = item->getNthTag(n++)) != NULL)
	{
	    // Every tag must be in discriminator tags list.
	    if (!hasTag(tag))
		return SKIP;
	}
	
	return USE;
}

int
MleDwpTagAllDiscriminator::hasTag(const char *itemTagString)
{
    // Loop over tags.
    _MleDwpTag *tag = m_tagList;
    while ( tag )
    {
	if (strcmp(tag->m_tag, itemTagString) == 0)
	    return 1;
	tag = tag->m_next;
    }
    return 0;
}


void *
MleDwpTagAllDiscriminator::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpTagAllDiscriminator::operator delete(void *p)
{
	mlFree(p);
}
