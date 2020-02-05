/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpNotifier.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the notifier used by the Magic Lantern Digital
 * Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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

// Include system header files.
#include <stdio.h>
#include <string.h>
//#include <memory.h>
//#include <bstring.h>

// Include Digital Workprint header files.
#include "mle/DwpNotifier.h"

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"


MleDwpNotifierPool::MleDwpNotifierPool(int p)
{
    // Initialize variables.
    m_poolsize = p;
    m_link = NULL;

    // Allocate the initial entries.
    makePool();
}


MleDwpNotifierPool::~MleDwpNotifierPool()
{
    // Delete all the pools.
    // Each pool block is actually one larger than the poolsize,
    // and the first entry is used to link the blocks together.
    // This code follows those links to delete the blocks when
    // the object is destroyed.
    while ( m_link )
    {
        MleDwpNotifierEntry *next = m_link->next;
        delete m_link;
        m_link = next;
    }
}


void
MleDwpNotifierPool::makePool(void)
{
    // Allocate an array of list items.
    // A pool block is actually one larger than the pool size.
    // The extra entry is used to link blocks together.
    m_unused = new MleDwpNotifierEntry[m_poolsize + 1];

    // Link to the old pool.
    m_unused[0].next = m_link;
    m_link = m_unused;

    // Initialize the free list.
    m_unused = &m_link[1];
    for ( int i = 0; i < m_poolsize - 1; i++ )
        m_unused[i].next = &m_unused[i + 1];
    m_unused[m_poolsize - 1].next = NULL;
}


// This function is used by a dictionary to get a new entry (instead
// of newing one off the heap).  It simply pulls the first element
// off the free list.
MleDwpNotifierEntry *
MleDwpNotifierPool::get(void)
{
    MleDwpNotifierEntry *rval;    // temporary for return value

    // Make a new pool if the current one is used up.
    if ( m_unused == NULL )
    {
#if defined(MLE_DEBUG)
        //printf("MleDwpNotifierPool::get: warning - expanding pool.\n");
#endif
        makePool();
    }

    // Pull an entry off the free list.
    rval = m_unused;
    m_unused = m_unused->next;
    return rval;
}


// This function is used by a dictionary to dispose of an entry
// (instead of deleting it from the heap).  It puts the entry
// back onto the free list.
void
MleDwpNotifierPool::put(MleDwpNotifierEntry *entry)
{
    // Put the entry on the free list.
    entry->next = m_unused;
    m_unused = entry;
}


void *
MleDwpNotifierPool::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpNotifierPool::operator delete(void *p)
{
	mlFree(p);
}


// This notifier constructor makes a new pool.
MleDwpNotifier::MleDwpNotifier(int hashsize,int poolsize)
{
    m_pool = new MleDwpNotifierPool(poolsize);

    initialize(hashsize);
}


// This notifier constructor uses an existing pool.
MleDwpNotifier::MleDwpNotifier(int hashsize,MleDwpNotifierPool *p)
{
    m_pool = p;

    initialize(hashsize);
}


MleDwpNotifier::~MleDwpNotifier()
{
    // Loop over all the hash table lists.
    for ( int i = 0; i < m_nbuckets; i++ )
    {
        // Prepend all the hash table lists to the pool free list.
        // Use intimate knowledge of the hash table instead of
        // doing this one at a time.
        if ( m_bucket[i] )
        {
            // Go to the last entry.
            MleDwpNotifierEntry *entry = m_bucket[i];
            while ( entry->next )
                entry = entry->next;
            
            entry->next = m_pool->m_unused;
            m_pool->m_unused = m_bucket[i];
        }
    }
}


// This is an initialization function that constructors can share.
void
MleDwpNotifier::initialize(int hashsize)
{
    m_nbuckets = hashsize;
    m_bucket = new MleDwpNotifierEntry*[m_nbuckets];

    // Zero out the bucket.
    memset(m_bucket,0,m_nbuckets*sizeof(MleDwpNotifierEntry *));
}


// This function adds a notification request.  Calling this function
// multiple times with identical arguments will result in duplicate
// entries.
void
MleDwpNotifier::addCallback(const void *key,MleDwpNotifierFunc func,
    const void *client)
{
    // Create a new entry.
    MleDwpNotifierEntry *entry = m_pool->get();
    /* XXX 6.2 Beta compilers caught this, should fix need to cast */
    entry->key = (void *)key; 
    entry->func = func;
    /* XXX 6.2 Beta compilers caught this, should fix need to cast */
    entry->client = (void *)client;

    // Prepend it to the right list.
    MleDwpNotifierEntry *&list = m_bucket[hash(key)];
    entry->next = list;
    list = entry;
}


// This function removes a specific notification request.  If multiple
// duplicate requests exist, only the most recently added will be removed.
void
MleDwpNotifier::removeCallback(const void *key,
    MleDwpNotifierFunc func,const void *client)
{
    // Get the right list.
    MleDwpNotifierEntry **entry = &m_bucket[hash(key)];

    while ( *entry )
    {
        // Look for the right entry.
        if ( (*entry)->key == key &&
             (*entry)->func == func &&
             (*entry)->client == client )
        {
            // Save the pointer.
            MleDwpNotifierEntry *tmp = *entry;

            // Make the previous pointer skip over this entry.
            *entry = (*entry)->next;

            // Return the entry to the free list.
            //bzero(tmp, sizeof(*tmp));
			memset((void *)tmp, 0, sizeof(*tmp));
            m_pool->put(tmp);

            return;
        }

        entry = &(*entry)->next;
    }

    // SHOULDN'T EVER GET HERE!!!!!
    // XXX take this out for now; it's causing endless grief
    MLE_ASSERT(0);

    //printf("WARNING: MleDwpNotifier::removeCallback(): Notifier callback failed to match any entries.\n");
}


// This function removes all notification requests associated with a key.
void
MleDwpNotifier::removeCallbacks(const void *key)
{
    // Get the right list.
    MleDwpNotifierEntry **entry = &m_bucket[hash(key)];

    while ( *entry )
    {
        // Look for the right entry.
        if ( (*entry)->key == key )
        {
            // Save the pointer.
            MleDwpNotifierEntry *tmp = *entry;

            // Make the previous pointer skip over this entry.
            *entry = (*entry)->next;

            // Return the entry to the free list.
            //bzero(tmp, sizeof(*tmp));
			memset((void *)tmp, 0, sizeof(*tmp));
            m_pool->put(tmp);
        }
        else
            entry = &(*entry)->next;
    }
}


// This static function does recursive traversal of a bucket.
// Recursion is used instead of iteration to allow post-order traversal.
static void traverse(const void *key, const void *data, 
     MleDwpNotifierType type, MleDwpNotifierEntry *entry)
{
    if ( entry->next )
        traverse(key, data, type, entry->next);
    
    if ( entry->key == key && entry->func != NULL)
        (*entry->func)(data, type,entry->client);
}


// This function notifies all interested parties who have registered with
// the passed key.
void
MleDwpNotifier::notify(const void *key, const void *data, 
             MleDwpNotifierType type) const
{
    // Pick the right bucket.
    MleDwpNotifierEntry *entry = m_bucket[hash(key)];

    // This list can easily be traversed through iteration.  At a
    // slight cost, we traverse through recursion instead to execute
    // the list items from tail to head.  Because list items are
    // inserted at the head, this results in execution in the same
    // order as insertion.
    if ( entry )
        traverse(key, data, type, entry);
}


// This is the default hash function, to map pointer values into
// bucket indices.
unsigned int
MleDwpNotifier::hash(const void *key) const
{
    unsigned int rval;

    // This hash function is a little ad hoc, but seems to work
    // reasonably well.  It is the classic multiply/modulo thing.
#ifdef __linux__
    rval = ((0xa74b2193*(unsigned long)key) >> 16) % m_nbuckets;
#else
    rval = ((0xa74b2193*(unsigned int)key) >> 16) % m_nbuckets;
#endif

    return rval;
}

void *
MleDwpNotifier::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpNotifier::operator delete(void *p)
{
	mlFree(p);
}


void *
MleDwpNotifierEntry::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpNotifierEntry::operator delete(void *p)
{
	mlFree(p);
}
