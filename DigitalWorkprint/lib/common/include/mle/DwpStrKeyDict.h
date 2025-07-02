/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpStrKeyDict.h
 * @ingroup MleDWPAccess
 *
 * This file defines the string dictionary used by the Magic Lantern Digital
 * Workprint Library API.
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_DWP_STRKEYDICT_H_
#define __MLE_DWP_STRKEYDICT_H_

// Include Digital Workprint header files.
#include "mle/Dwp.h"
#include "mle/DwpDict.h"


/**
 * @brief A key entry based on a string.
 */
class MLE_DWP_API MleDwpStrKeyDictEntry : public MleDwpDictEntry
{
  public:

	/**
	 * The destructor.
	 */
    virtual ~MleDwpStrKeyDictEntry();
    
	/**
	 * @brief Set the key.
	 *
	 * @param key A pointer to a key. The key must
	 * reference a character string.
	 */
    virtual void setKey(const void *key);

	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
     * Override operator new array.
     *
     * @param tSize The size, in bytes, to allocate.
     */
	void* operator new[](size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);

	/**
     * Override operator delete array.
     *
     * @param p A pointer to the memory to delete.
     */
	void  operator delete[](void* p);
};


/**
 * @brief Implements a dictionary based on keys that
 * reference strings.
 */
class MLE_DWP_API MleDwpStrKeyDict : public MleDwpDict
{
  public:

	/*
	 * @brief The constructor.
	 *
	 * @param hashsize The size of the hash table to use.
	 */
    MleDwpStrKeyDict(int hashsize = MLE_DWP_DICT_DEFHASHSIZE)
      : MleDwpDict(hashsize) {}

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
    
    /**
     * Dump the contents of the dictionary.
     */
    void dump();

  protected:

    /**
	 * @brief Makes a new entry of type MleDwpStrKeyDictEntry.
	 *
	 * @return A pointer to a new dictionary entry is returned.
	 */
    virtual MleDwpDictEntry *makeEntry(void);

    /**
	 * @brief Implements the integer hashing function for the
     * dictionary.
	 *
	 * The hash will be performed on the key that points to
	 * a character string.
	 *
	 * @param key A pointer to the key on which to perform the hash.
	 *
	 * @return The hashed value of the string is returned.
	 */
    virtual unsigned int hash(const void *key) const;

    /**
	 * @brief Compares two keys for equality.
	 *
	 * @param key0 A pointer to the first key to compare against.
	 * @param key1 A pointer to the seconde key to compary against.
	 *
	 * @return 0 will be returned if key0 and key1 are equal. 1 will
	 * be returned if key0 is greater than key1. If key0 is less than
	 * key1, the -1 will be returned.
	 */
    virtual int compare(const void *key0,const void *key1) const;
};


#endif /* __MLE_DWP_STRKEYDICT_H_ */
