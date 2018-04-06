/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpDict.h
 * @ingroup MleDWPAccess
 *
 * This file defines the dictionary used by the Magic Lantern Digital
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_DWP_DICT_H_
#define __MLE_DWP_DICT_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Digital Workprint header files.
#include "mle/Dwp.h"


/**
 * Default hash table size for dictionaries.
 */
#define MLE_DWP_DICT_DEFHASHSIZE 15


/**
 * @brief This is an entry in a dictionary.
 *
 * It is a basic linked list element with two values, a key for lookup
 * and a value.
 */
class MLE_DWP_API MleDwpDictEntry
{
  // Declare member variables.

  public:

    void *m_key;    /**< The dictionary key for the entry. */
    void *m_value;  /**< The value of the dictionary entry. */
    
    MleDwpDictEntry *m_next; /**< The next entry in the dictionary. */


  // Declare member functions.

  public:

	/**
	 * @brief The default constructor.
	 */
    MleDwpDictEntry(void) { m_next = NULL; m_key = NULL; m_value = NULL; }

	/**
	 * @brief The destructor.
	 */
    virtual ~MleDwpDictEntry() {}
    
    /**
	 * @brief Copies the key into an entry.
	 *
	 * The default just does a pointer copy.
	 * This virtual is provided so that copies can
     * be made either by reference or by value.
	 *
	 * @param key A pointer to the key to set.
	 */
    virtual void setKey(const void *key);

    /**
	 * @brief Copies the value into an entry.
	 *
	 * The default just does a pointer copy.
	 * This virtual is provided so that copies
     * can be made either by reference or by value.
	 *
	 * @param value A pointer to the value  to set.
	 */
    virtual void setValue(const void *value);

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
 * @brief This is the Digital Workprint dictionary base class.
 *
 * There are both workprint dictionaries and player dictionaries.  The
 * workprint dictionary exists only on the workstation side of things,
 * and doesn't do any pool allocation.
 */
class MLE_DWP_API MleDwpDict
{
  // Declare member variables.

  protected:

    int               m_hashsize; /**< The size of the hash table. */
    MleDwpDictEntry **m_htable;   /**< A pointer to the hash table. */


  // Declare member functions.

  public:

    /**
	 * @brief A constructor.
	 *
	 * By default, a dictionary is created with <code>MLE_DWP_DICT_DEFHASHSIZE</code>
	 * entries.
     *
     * @param hashsize The hash table size. For best results, 
     * it should be about the number of items expected
     * to go into the dictionary.
	 */
    MleDwpDict(int hashsize = MLE_DWP_DICT_DEFHASHSIZE);

	/**
	 * @brief The destructor.
	 */
    virtual ~MleDwpDict();
    
    /**
	 * @brief Finds the value associated with a specified
	 * dictionary key.
	 *
	 * @param key The key of the value to search for.
	 *
	 * @return The value associated with the key argument
	 * is returned.
	 */
    void *find(const void *key) const;
    
    /**
	 * @brief Associates a value with a key.
	 *
	 * This overrides any previous association.
	 *
	 * @param key A pointer to the key to use in the dictionary.
	 * @param value A pointer to the value to set.
	 */
    void set(const void *key,const void *value);
    
    /**
	 * @brief Removes the association of a key.
	 *
	 * Subsequent calls to find with this key will return <b>NULL</b>.
	 *
	 * @param key A pointer to the key to use to remove the item
	 * from the dictionary.
	 */
    void remove(const void *key);

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

    /**
	 * @brief Makes a new entry of the appropriate type.
	 *
	 * Usually new entry types are subclassed to implement key or value copying
     * that does allocation.  This generally means a destructor is
     * necessary to deallocate memory.
	 *
	 * @return A pointer to a new dictionary entry is returned.
	 */
    virtual MleDwpDictEntry *makeEntry(void);
    
    /**
	 * @brief Implements the integer hashing function for the
     * dictionary.
	 *
	 * The number returned will have modulo hashsize
     * taken to get the hash value.  By default, the hashing function
     * is a very simple pointer to number conversion with low order
     * bits removed.  Subclasses can override this function to provide
     * better functions, or functions that hash on what the pointers
     * point to, e.g. a string hash.
	 *
	 * @param key A pointer to the key on which to perform the hash.
	 *
	 * @return The hashed value is returned.
	 */
    virtual unsigned int hash(const void *key) const;
    
    /**
	 * @brief Compares two keys for equality.
	 *
	 * The default compare function just does pointer equality.
     * <p>
     * IMPORTANT NOTE: <i>compare()</i> should return ZERO on a match,
     * NONZERO on a mismatch, just as <i>strcmp()</i> does.
     * </p><p>
     * Subclasses can override this function to provide comparison
     * on what the pointers point to, e.g. a string compare.
	 * </p>
	 *
	 * @param key0 A pointer to the first key to compare against.
	 * @param key1 A pointer to the seconde key to compary against.
	 *
	 * @return <b>0</b> will be returned if key0 and key1 are equal.
	 * <b>1</b> will be returned if key0 is greater than key1. If key0 is less than
	 * key1, then <b>-1</b> will be returned.
	 */
    virtual int compare(const void *key0,const void *key1) const;
    
    /**
	 * Find an entry in the dictionary.
	 *
	 * This routine is for internal use only, and looks up an
     * entry structure for the given key.
	 *
	 * @param key A pointer to a key for the entry to find.
	 *
	 * @return A pointer to the dictionary entry is returned.
	 * <b>NULL</b> will be returned if an entry is not found for the 
	 * corresponding key.
	 */
    MleDwpDictEntry *findEntry(const void *key) const;

    friend class MleDwpDictIter;
};


/**
 * @brief This is an iterator for a <code>MleDwpDict</code>.
 */
class MLE_DWP_API MleDwpDictIter
{
  public:

    /**
	 * A constructor,
	 *
     * @param dict A reference to the dictionary to iterate over.
	 */
    MleDwpDictIter(const MleDwpDict& dict);
    
    /**
	 * @brief Get the current key.
	 *
	 * @return A pointer to the current key is returned. Otherwise,
	 * <b>NULL</b> is returned if none are remaining to be iterated over.
	 */
    const void *getKey(void) const;

    /**
	 * @brief Get the current value.
	 *
	 * @return A pointer to the current value is returned. Otherwise,
	 * <b>NULL</b> is returned if none are remaining to be iterated over.
	 */
     const void *getValue(void) const;
    
    /**
	 * @brief Iterates to the next item in the dictionary.
	 *
	 * @return <b>0</b> is returned if no items are left,
	 * nonzero if there is a new current item.
	 */
    int next();

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

  private:

    const MleDwpDict *m_dict;       // The dictionary to iterate over.
    
    int m_index;                    // Current hash table bucket.
    const MleDwpDictEntry *m_entry; // Current list item within a bucket.
};


#endif /* __MLE_DWP_DICT_H_ */
