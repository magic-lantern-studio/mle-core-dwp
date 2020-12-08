/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpNotifier.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the string key dictionary used by the Magic Lantern Digital
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/DwpStrKeyDict.h"


unsigned int
MleDwpStrKeyDict::hash(const void *key) const
{
	int value = 0;

	do {
		/* Rotate left. */
		value = (value << 8) | (value >> 24);

		/* Add character. */
		value += *(char *)key;
	} while ( *(char *)(key = (char *)key + 1) );

	return value;
}


void
MleDwpStrKeyDict::dump()
{
    MleDwpDictIter iter(*this);
    do {
        const void *key = iter.getKey();
        const void *value = iter.getValue();
        fprintf(stdout, "key = %s, value = 0x%p\n", (char *)key, value);
    } while(iter.next());
}


void *
MleDwpStrKeyDict::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpStrKeyDict::operator delete(void *p)
{
	mlFree(p);
}


int
MleDwpStrKeyDict::compare(const void *key0,const void *key1) const
{
	return strcmp((char *)key0,(char *)key1);
}


MleDwpDictEntry *
MleDwpStrKeyDict::makeEntry(void)
{
	return new MleDwpStrKeyDictEntry;
}


MleDwpStrKeyDictEntry::~MleDwpStrKeyDictEntry()
{
	delete (char *)m_key;
}


void
MleDwpStrKeyDictEntry::setKey(const void *k)
{
	delete (char *)m_key;

	m_key = k ? strcpy(new char[strlen((char *)k) + 1],(char *)k) : NULL;
}


void *
MleDwpStrKeyDictEntry::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpStrKeyDictEntry::operator delete(void *p)
{
	mlFree(p);
}
