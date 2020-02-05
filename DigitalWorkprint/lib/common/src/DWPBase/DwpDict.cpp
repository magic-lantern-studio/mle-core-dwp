/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpDict.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the dictionary used by the Magic Lantern Digital
 * Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2019 Wizzer Works
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

// Include Digital Workprint header files.
#include "mle/mlMalloc.h"
#include "mle/DwpDict.h"


MleDwpDict::MleDwpDict(int h)
{
    /* Set the hash table size. */
    m_hashsize = h;

    /* Allocate hash table entries. */
    m_htable = new MleDwpDictEntry *[m_hashsize];

    /* Zero out the table. */
    memset(m_htable,0,m_hashsize*sizeof(MleDwpDictEntry *));
}


MleDwpDict::~MleDwpDict()
{
    /* Clean up all the entries. */
    for ( int i = 0; i < m_hashsize; i++ )
    {
        MleDwpDictEntry *entry = m_htable[i];

        while ( entry )
        {
            MleDwpDictEntry *tmp = entry;
            entry = entry->m_next;
            delete tmp;
        }
    }

    delete m_htable;
}


MleDwpDictEntry *
MleDwpDict::makeEntry(void)
{
    return new MleDwpDictEntry;
}


void *
MleDwpDict::find(const void *key) const
{
    MleDwpDictEntry *entry = findEntry(key);

    if ( entry )
        return entry->m_value;
    else
        return NULL;
}


MleDwpDictEntry *
MleDwpDict::findEntry(const void *key) const
{
    /* Pick up the right list of entries. */
    MleDwpDictEntry *entry = m_htable[hash(key) % m_hashsize];

    /* Linear search through the list. */
    while ( entry )
    {
        /* If found, return the entry. */
        if ( !compare(entry->m_key,key) )
            return entry;
        
        /* Follow the linked list. */
        entry = entry->m_next;
    }

    return NULL;
}


void
MleDwpDict::set(const void *key,const void *value)
{
    /* Look if the entry already exists. */
    MleDwpDictEntry *entry = findEntry(key);

    /* If so, then set it. */
    if ( entry )
    {
        entry->setValue(value);
        return;
    }

    /* Otherwise, create a new one. */
    entry = makeEntry();
    entry->setKey(key);
    entry->setValue(value);

    /* Prepend new entry to beginning of appropriate list. */
    int index = hash(key) % m_hashsize;
    entry->m_next = m_htable[index];
    m_htable[index] = entry;
}


void
MleDwpDict::remove(const void *key)
{
    /* Find the hash table index for the entry. */
    int h = hash(key) % m_hashsize;

    /* Search through the list of entries. */
    MleDwpDictEntry **entry = &m_htable[h];
    while ( *entry )
    {
        /* Compare against the key. */
        if ( !compare((*entry)->m_key,key) )
        {
            /* Save the pointer. */
            MleDwpDictEntry *tmp = *entry;

            /* Do list removal. */
            *entry = (*entry)->m_next;

            /* Do teardown and deletion. */
            delete tmp;

            break;
        }

        /* Keep going. */
        entry = &(*entry)->m_next;
    }
}


void
MleDwpDict::dump()
{
    MleDwpDictIter iter(*this);
    do {
        const void *key = iter.getKey();
        const void *value = iter.getValue();
        fprintf(stdout, "key = 0x%x, value = 0x%x\n", key, value);
    } while(iter.next());
}


unsigned int
MleDwpDict::hash(const void *key) const
{
#ifdef __linux__
	return (long)key >> 2;
#else
    return (int)key >> 2;
#endif
}


int
MleDwpDict::compare(const void *key0,const void *key1) const
{
    return key0 != key1;
}


void *
MleDwpDict::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpDict::operator delete(void *p)
{
	mlFree(p);
}

void
MleDwpDictEntry::setKey(const void *k)
{
    m_key = (void *)k;
}


void
MleDwpDictEntry::setValue(const void *v)
{
    m_value = (void *)v;
}


void *
MleDwpDictEntry::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpDictEntry::operator delete(void *p)
{
	mlFree(p);
}


MleDwpDictIter::MleDwpDictIter(const MleDwpDict& d)
{
    m_dict = &d;

    m_index = 0;                         // Starting bucket.
    m_entry = m_dict->m_htable[m_index]; // Starting entry.

    /* Skip to the first nonzero entry. */
    if ( m_entry == NULL )
        next();
}


const void *
MleDwpDictIter::getKey(void) const
{
    return m_entry ? m_entry->m_key : NULL;
}


const void *
MleDwpDictIter::getValue(void) const
{
    return m_entry ? m_entry->m_value : NULL;
}


int
MleDwpDictIter::next(void)
{
    /* Keep doing list traversal while you can. */
    if ( m_entry )
        m_entry = m_entry->m_next;
    
    /* No list, then find a bucket that isn't empty. */
    while ( m_entry == NULL && (m_index += 1) < m_dict->m_hashsize )
        m_entry = m_dict->m_htable[m_index];
    
    return m_entry != NULL;
}


void *
MleDwpDictIter::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpDictIter::operator delete(void *p)
{
	mlFree(p);
}
