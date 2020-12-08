/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpNotifier.h
 * @ingroup MleDWPAccess
 *
 * This file defines the notification API used by the Magic Lantern
 * Digital Workprint Library API.
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


#ifndef __MLE_DWP_NOTIFIER_H_
#define __MLE_DWP_NOTIFIER_H_


// Include Digital Workprint header files.
#include "mle/Dwp.h"


/**
 * An enumeration of notification types.
 */
enum MleDwpNotifierType
{
    MleDwpDeleteNotification,             /**< When destructor executes. */
    MleDwpChangeNotification,             /**< When various attributes change. */
    MleDwpCreateNotification,             /**< When constructor executes. */
    MleDwpChildInsertedNotification,      /**< When a child is added to KEY item. */
    MleDwpChildRemovedNotification,       /**< When a child is removed from KEY. */
    MleDwpDescendentInsertedNotification, /**< Child, grandchild etc. added to KEY. */
    MleDwpDescendentRemovedNotification   /**< Child, grandchild removed from KEY. */
};

/**
 * This is a typedef for the notifier callback function pointer.
 */
typedef void (*MleDwpNotifierFunc)(const void *data, 
                                   MleDwpNotifierType type,
                                   const void *client);

/**
 * This is the struct managed by a notifier
 * Its use is internal for the most part, but is made public for
 * any exceptions.
 */
struct MLE_DWP_API MleDwpNotifierEntry
{
    void *key;               /**< The key for the entry. */
    MleDwpNotifierFunc func; /**< The callback function. */
    void *client;            /**< The client for the callback. */
    
	/** The next entry in the list of notifiers. */
    MleDwpNotifierEntry *next;

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
 * @brief A collection of available notifiers.
 */
class MLE_DWP_API MleDwpNotifierPool
{
  public:

    /**
	 * A constructor.
	 *
     * @param poolsize Determines the
     * initial number of entries and the number added on every
     * extension.
	 */
    MleDwpNotifierPool(int poolsize);

	/**
	 * The destructor.
	 */
    ~MleDwpNotifierPool();

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

    friend class MleDwpNotifier;
    
    // Pool expansion.
    void makePool(void);
    
    // Allocation and deallocation.
    MleDwpNotifierEntry *get(void);
    void put(MleDwpNotifierEntry *);
    
    int m_poolsize;                 // Allocation block size.
    MleDwpNotifierEntry *m_unused;  // Pointer to free list.
    MleDwpNotifierEntry *m_link;    // Pointer to first block.
};


/**
 * @brief This class implements an Observable pattern with notification
 * callbacks.
 *
 * The workprint notifier object associates callback functions with void *
 * keys.  One or more callbacks can be registered (with client data) for
 * any key.  The notifier can issue all the callbacks for a specified key.
 * <p>
 * The primary use of the workprint notifier is to allow a callback service
 * for whenever workprint items are created or deleted.  However, the notifier
 * is a general mechanism which can be used for other applications.
 * </p>
 */
class MLE_DWP_API MleDwpNotifier
{
  public:

    /**
	 * @brief A constructor.
	 *
     * The notifier does internal allocation of callback entries from a
     * pool in order to avoid memory fragmentation by allocating many
     * small objects.  Each notifier may have its own pool, or pools
     * may be shared.  If a pool size is specified in the constructor,
     * a new pool is created.  The pool size is the number of entries
     * that are allocated at a time; 32 is usually a reasonable number.
     * If a pointer to a pool is passed in instead, the new notifier
     * will use that existing pool.
     * <p>
     * The hash size is the number of hash buckets the notifier will
     * use for internal lookup.  This is a time and space tradeoff; the
     * general rule of thumb is to make the hash size about the number
     * of expected keys.
	 * </p>
	 *
	 * @param hashsize The hash size.
	 * @param poolsize The size of the pool.
     */
    MleDwpNotifier(int hashsize,int poolsize);

    /**
	 * @brief A constructor.
	 *
     * The notifier does internal allocation of callback entries from a
     * pool in order to avoid memory fragmentation by allocating many
     * small objects.  Each notifier may have its own pool, or pools
     * may be shared.  If a pool size is specified in the constructor,
     * a new pool is created.  The pool size is the number of entries
     * that are allocated at a time; 32 is usually a reasonable number.
     * If a pointer to a pool is passed in instead, the new notifier
     * will use that existing pool.
     * <p>
     * The hash size is the number of hash buckets the notifier will
     * use for internal lookup.  This is a time and space tradeoff; the
     * general rule of thumb is to make the hash size about the number
     * of expected keys.
	 * </p>
	 *
	 * @param hashsize The hash size.
	 * @param pool A pointer to a preallocated pool.
     */
    MleDwpNotifier(int hashsize, MleDwpNotifierPool *pool);

    /**
     * The destructor.
     */
    ~MleDwpNotifier();
    
    // Callback management.

    /**
     * @brief Adds a callback function with client data to the
     * specified key.
	 *
	 * Adding a callback more than once will result
     * in the callback function being called multiple times.  A DWP
     * item's typeId can be used, in which case create and delete
     * notifications are sent when any item of the specified type is
     * created or destroyed.  Other types of notifications are not
     * available on a class basis.
	 *
	 * @param key The key for registering the callback function.
	 * @param func The callback function being registered.
	 * @param client The client data to be used when the callback
	 * is invoked.
     */
    void addCallback(const void *key,
                     MleDwpNotifierFunc func,
					 const void *client);

    /**
     * @brief Removes a specific callback entry.  Both the
     * function pointer and client data must match.
	 *
	 * @param key The key used for registering the callback function.
	 * @param func The callback function being removed.
	 * @param client The client data associated with the callback
	 * function.
     */
    void removeCallback(const void *key,
                        MleDwpNotifierFunc func,
						const void *client);

    /**
     * @brief Removes all callback entries associated with
     * the specified key.
	 *
	 * @param key The key associated with the callback functions
	 * to be removed.
     */
    void removeCallbacks(const void *key);
    

    // Callback invocation.

    /**
     * @brief Invokes the callbacks on a key.
	 *
	 * The data and type arguments,
     * in addition to the client data, will be passed to the callback
     * function.
	 *
	 * @param key The key for those callbacks that are to be invoked.
	 * @param data The data to send via the callback function.
	 * @param type The type of notification causing the callback
	 * to be invoked.
     */
    void notify(const void *key, const void *data, 
                MleDwpNotifierType type) const;
    
    /**
     * @brief Get the pool.
	 *
	 * @return A pointer to the notifier pool this notifier is
     * using.  This pointer may be useful to create other notifiers that
     * share the pool.
     */
    MleDwpNotifierPool *getPool(void) const { return m_pool; }

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

    int m_nbuckets;                  /**< Number of hash table entries. */
    MleDwpNotifierEntry **m_bucket;  /**< Pointer to the hash table. */
    
    /**
	 * @brief This is an initialization function that the constructors share.
	 *
	 * @param hashsize The size of the hash table to use.
	 */
    void initialize(int hashsize);

    /**
	 * @brief The hash function.
	 *
	 * @param key A pointer to the key to hash.
	 *
	 * @return The hashed value is returned.
	 */
    unsigned int hash(const void *key) const;

    MleDwpNotifierPool *m_pool;      /**< Pointer to the allocation pool. */
};


#endif /* __MLE_DWP_NOTIFIER_H_ */
