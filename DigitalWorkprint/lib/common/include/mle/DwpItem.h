/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpItem.h
 * @ingroup MleDWPAccess
 *
 * This file defines the digital workprint item used by the Magic Lantern Digital
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

#ifndef __MLE_DWP_ITEM_H_
#define __MLE_DWP_ITEM_H_


// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/MleDsoLoader.h"

// Include Digital Workprint header files.
#include "mle/Dwp.h"
#include "mle/DwpMonitor.h"
#include "mle/DwpStrKeyDict.h"

class MleDwpItem;
class MleDwpInput;
class MleDwpOutput;
//class MleDwpStrKeyDict;
class MleDwpDiscriminator;
struct _MleDwpTag;


/**
 * This typedef defines a <code>MleDwpType</code>.  There is a unique typeId static
 * member variable on each item class.  This can be used to do
 * faster type checks than with the type name, which requires
 * string manipulation.  The typedef is to a function pointer
 * because typeId is really the instance creation function pointer.
 * This allows the linker to set up the unique namespace for us.
 */
typedef MleDwpItem *(*MleDwpType)(void);


/**
 * @brief The base class for all workprint items.
 *
 * This is the base class for all workprint items, which are objects
 * in the workprint.  New workprint items should be subclassed from
 * <code>MleDwpItem</code> and override the virtual <i>readContents()</i>
 * and <i>writeContents()</i> member functions (if they are anything more
 * than pure containers) to transcribe themselves in and out correctly.
 */
class MLE_DWP_API MleDwpItem : public MleDwpMonitor
{

  public:

	/**
	 * @brief A constructor.
	 *
	 * Subclasses of <code>MleDwpItem</code> should have either no constructor
	 * or a constructor that takes no arguments in order for the
	 * macros defined below to work properly in providing a creation
	 * function for the type registry.
	 */
	MleDwpItem(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpItem(void);

	/**
	 * @brief Either copy an item without its children, or
	 * copy a subtree.
	 * 
	 * In the base class, this capability is implemented
	 * by using transcription which gives inherited functionality
	 * for free.  The copy can optionally be placed under a parent
	 * item.
	 *
	 * @param copyChildren A flag indicating whether to copy the
	 * subtree. By default, it is set to copy the children.
	 * @param parent A pointer to the parent in which to place
	 * the copy.
	 *
	 * @return A pointer to the copy of the workprint item is returned.
	 */
	virtual MleDwpItem * copy(int copyChildren = 1,MleDwpItem *parent = NULL);

	/**
	 * @brief Does a full copy of an item, by turning off the discriminator.
	 *
	 * Without doing this, (if you call dwpItem->copy() directly), 
	 * child objs that don't match discrim won't be copied.
	 *
	 * @param copyChildren A flag indicating whether to copy the
	 * subtree. By default, it is set to copy the children.
	 * @param parent A pointer to the parent in which to place
	 * the copy.
	 *
	 * @return A pointer to the copy of the workprint item is returned.
	 */
	virtual MleDwpItem * copyWithoutDiscrim(int copyChildren = 1, 
	    MleDwpItem *parent = NULL);

	/**
	 * @brief Get the parent of this workprint item.
	 *
	 * This functionality is implemented as member functions
	 * so that traversal can be modified under certain conditions.
	 * For example, some workprint items can be marked as present
	 * for certain target platform types.  File inclusion nodes
	 * can also be skipped using this mechanism.
	 *
	 * @return A pointer to the workprint item's parent is returned.
	 */
	MleDwpItem *getParent(void) const;

	/**
	 * @brief Get the first ancestor of the given type in
	 * the containership hierarchy.
	 *
	 * @param type The type of workprint item to retrieve.
	 *
	 * @return The first ancestor of the specified type is
	 * returned.
	 */
	MleDwpItem *getAncestor(MleDwpType type);

	/**
	 * @brief Get the first child in the containership hierarchy.
	 *
	 * @return A pointer to the first child is returned.
	 */
	MleDwpItem *getFirstChild(void) const;

	/**
	 * @brief Get the last child in the containership hierarchy.
	 *
	 * @return A pointer to the last child is returned.
	 */
	MleDwpItem *getLastChild(void) const;

	/**
	 * @brief Get the previous child in the containership hierarchy.
	 *
	 * @return A pointer to the previous child is returned.
	 */
	MleDwpItem *getPrev(void) const;

	/**
	 * @brief Get the next child in the containership hierarchy.
	 *
	 * @return A pointer to the next child is returned.
	 */
	MleDwpItem *getNext(void) const;

	/**
	 * @brief Set discrimination.
	 *
	 * This functionality allows controlling traversal by attaching
	 * an object that evaluates how an item should be traversed.
	 * <i>setDiscriminator()</i> replaces the existing discriminator without
	 * deleting it (so discriminator state may be saved and restored
	 * by doing a "get" followed by a "set").  Passing <b>NULL</b> means
	 * that discrimination is not active, but is reserved for
	 * temporary conditions.
	 *
	 * Discriminators are shared by all items in a <code>MleDwpItem</code> tree, and
	 * thus, setting a discriminator results in the set to the same
	 * discriminator for all descendents.
	 *
	 * @param discrim A pointer to a discriminator.
	 */
	void setDiscriminator(MleDwpDiscriminator *discrim);

	/**
	 * @brief Get the discriminator.
	 *
	 * @return A pointer to the discriminator is returned. <b>NULL</b> will be
	 * returned if one does not exist.
	 */
	MleDwpDiscriminator *getDiscriminator();


	/**
	 * @brief Insert a child workprint item.
	 *
	 * <i>insertChild()</i> attaches a child in the position before beforeChild.
	 * If beforeChild is <b>NULL</b>, the child is appended to the end.
	 *
	 * @param child A pointer to the child workprint item to insert.
	 * @param beforeChild A pointer to the child workprint item to insert
	 * the new child ahead of.
	 */
	void insertChild(MleDwpItem *child,MleDwpItem *beforeChild = NULL);

	/**
	 * @brief Remove a child workprint item.
	 *
	 * <i>removeChild()</i> removes a linkage, but does not delete the
	 * former child.
	 *
	 * @param child A pointer to the workprint item to remove
	 * all linkages from.
	 */
	void removeChild(MleDwpItem *child);


	/**
	 * @brief Get the name of the workprint item.
	 *
     * Subclasses must redefine this method to test against their subclass
     * specific workprint item type identifier.  The <b>MLE_DWP_*</b> macros below
     * do this automatically.
	 *
	 * @return A pointer to a character string representing the name of
	 * the workprint is returned.
	 */
	const char *getName(void) const;

	/**
	 * @brief Set the name of the workprint item.
	 *
	 * @param name A pointer to a character string representing the name of
	 * the workprint item.
	 */
	void setName(const char *name);

	/**
	 * @brief Get the workprint type name.
	 *
	 * @return A pointer to the workprint's type name is returned.
	 */
	virtual const char *getTypeName(void) const;

	/**
	 * @brief Get the workprint type identifier.
	 *
     * Subclasses must redefine this method to return their subclass
     * specific workprint item type identifier.  The <b>MLE_DWP_*</b> macros below 
     * do this automatically.
	 *
	 * @return A workprint type identifier is returned.
	 */
	virtual MleDwpType getTypeId(void) const;

    /**
	 * @brief A type identifier for generic workprint items.
	 *
	 * This is provided for workprint items which do not specify a
	 * specifc type.
	 *
	 * @return A pointer to a generic workprint type identifier is
	 * returned.
	 */
    static MleDwpItem *typeId(void);

	/**
	 * @brief Determine if the workprint item is of a specified type.
	 *
     * Subclasses must redefine this method to test against their subclass
     * specific workprint item type identifier.  The <b>MLE_DWP_*</b> macros below
     * do this automatically.
	 *
	 * @param type A pointer to a character string representing a workprint
	 * type in which to compare against.
	 *
	 * @return If the workprint item is the specified type, then <b>TRUE</b> will
	 * be returned. Otherwise, <b>FALSE</b> will be returned.
	 */
	virtual int isa(const char *type) const;

	/**
	 * @brief Determine if the workprint item is of the specified type.
	 *
     * Subclasses must redefine this method to test against their subclass
     * specific workprint item type identifier.  The <b>MLE_DWP_*</b> macros below
     * do this automatically.
	 *
	 * @param type A MleDwpType in which to compare against.
	 *
	 * @return If the workprint item is the specified type, then <b>TRUE</b> will
	 * be returned. Otherwise, <b>FALSE</b> will be returned.
	 */
	virtual int isa(MleDwpType type) const;


	/**
	 * @brief Add a tag.
	 *
	 * This functionality is a very simple mechanism to associate
	 * strings with items. It should not be used to store large
	 * numbers of strings. The intent is to have discriminators
	 * that use tags.
	 *
	 * @param tag A pointer to a character string representing a tag.
	 */
	void addTag(const char *tag);

	/**
	 * @brief Remove a tag.
	 *
	 * @param The tag to remove from the workprint item.
	 */
	void removeTag(const char *tag);

	/**
	 * @brief Check if the workprint item has a specific tag.
	 *
	 * @param tag The tag to check against.
	 *
	 * @return If the workprint item has the specified tag, then <b>TRUE</b> will be
	 * returned. Otherwise, <b>FALSE</b> will be returned.
	 */
	int hasTag(const char *tag) const;

	/**
	 * @brief Check if the workprint item has any tags.
	 *
	 * @return If the workprint item has any tags, then <b>TRUE</b> will be
	 * returned. Otherwise, <b>FALSE</b> will be returned.
	 */
	int hasAnyTag(void) const;

	/**
	 * @brief Get the tag located in the specifed positiion.
	 *
	 * @param n The index of the tag to retrieve.
	 *
	 * @return A pointer to the character string representing the nth
	 * tag is returned. <b>NULL</b> will be returned if the nth tag does not
	 * exist.
	 */
	const char * getNthTag(int n) const;


	/**
	 * @brief Notify register listeners of activity in the workprint
	 * item hierarchy.
	 *
	 * <i>notify()</i> informs registered listeners of item creation,
     * modification, or deletion.  By default (ie. a <b>NULL</b> key parameter),
     * creation notifications go to those registered for the LEAF class
     * type.  Parent class notification must specify the parent class'
     * typeId as the key parameter.  The key parameter is ignored for
     * chance and delete notifications.  This method is ordinarily invoked
     * by subclasses in their <i>set*()</i> methods.
	 *
	 * @param notifyType The type of notification to perform. Valid
	 * types include:
	 * @param key A key used to extend notification.
	 */
    void notify(MleDwpNotifierType notifyType, void *key = NULL);


    /**
	 * @brief Write out the workprint item.
	 *
	 * <i>write()</i> writes the item out, setting up the container,
	 * invoking the <i>readContents()</i> virtual function, and recursively
	 * writing all the contained items.
	 *
	 * @param out The output stream to write the workprint item to.
	 * @param writeChildren A flag indicating whether to write out
	 * the children of the workprint item. By default, the child
	 * hierachy will be written out.
	 *
	 * @return Zero is returned on a successful write, nonzero on error.
	 */
	virtual int write(MleDwpOutput *out,int writeChildren = 1);

	/**
	 * @brief Read in workprint items.
	 *
	 * <i>read()</i> reads an item in (note it is a static member function),
	 * and attaches it to an optional parent item argument.  <i>read()</i>
	 * reads the item type, uses the registry to create a new item
	 * of that type, calls the virtual <i>readContents()</i> function to
	 * read in the type-specific information, and then recursively
	 * reads any contained items.
	 *
	 * @param in The input stream to read the workprint items from.
	 * @param parent A pointer to a workprint item in which to attach
	 * the new workprint items too.
	 */
	static MleDwpItem *read(MleDwpInput *in,MleDwpItem *parent = NULL);

	/**
	 * @brief Read all items from the specifed input stream.
	 *
	 * <i>readAll()</i> reads in everything from an input object and
	 * returns it all under a container.  This is a special container
	 * that writes its children out without writing itself, so a
	 * <i>readAll()</i> and a <i>write()</i> will not add a layer of hierarchy
	 * to the workprint file.  There are two versions of <i>readAll()</i> -
	 * one takes an input object and one takes a filename.
	 *
	 * @param in The input stream to read the workprint from.
	 *
	 * @return The resulting workprint hierarchy is returned.
	 */
	static MleDwpItem *readAll(MleDwpInput *in);
	
	/**
	 * @brief Read all items from the specifed file.
	 *
	 * <i>readAll()</i> reads in everything from a file and
	 * returns it all under a container.  This is a special container
	 * that writes its children out without writing itself, so a
	 * <i>readAll()</i> and a <i>write()</i> will not add a layer of hierarchy
	 * to the workprint file.  There are two versions of <i>readAll()</i> -
	 * one takes an input object and one takes a filename.
	 *
	 * @param in The input file to read the workprint from.
	 *
	 * @return The resulting workprint hierarchy is returned.
	 */
	static MleDwpItem *readAll(const char *filename);

	/**
	 * @brief Set some user data.
	 *
	 * <i>setUserData()</i> sets the user data field on an item.  This field
	 * is available for use by applications.
	 */
	void setUserData(void *u) { m_userData = u; }

	/**
	 * @brief Get the user data.
	 *
	 * <i>getUserData()</i> returns the user data field for an item.  This
	 * field is available for use by applications.
	 *
	 * @return The user data is returned.
	 */
	void *getUserData(void) const { return m_userData; }

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
	 * @brief A DSO loader.
	 *
	 * This is a static DSO loader object that can be used to load
	 * in new item classes or datatype classes.
	 */
	static MleDSOLoader g_loader;


  protected:

	// Basic tree connectivity pointers.
	MleDwpItem *m_parent;  /**< The parent of this item. */
	MleDwpItem *m_first;   /**< The first sibling. */
	MleDwpItem *m_last;    /**< The last sibling. */
	MleDwpItem *m_prev;    /**< The previous sibling. */
	MleDwpItem *m_next;    /**< The next sibling. */

	/** The discriminator. */
	MleDwpDiscriminator *m_discriminator;

	// The tag list.
	//
	// There are two pointers to manage the list of tags because
	// we want to append them as they are read in so they will be
	// written out in the same order.
	_MleDwpTag *m_tagFirst, *m_tagLast;

	// Optional item name (for use in search, etc.).
	char *m_name;      /**< The name of the item. */

	// User data field.
	void *m_userData;  /**< Associated user data. */

	// Rile I/O.
	//
	// readContents() and writeContents() perform the item-specific
	// reading of workprint information.  It does not handle
	// transcription of contained objects; that is handled by
	// the base transcription routines.  Application programs
	// will not call these functions; they will be invoked by
	// read() or write().  Zero is returned for success, nonzero
	// for failure.

	/**
	 * Read the contents of the workprint item.
	 *
	 * @param The input object to read from.
	 *
	 * @return If successful, <b>0</b> will be returned.
	 * If unsuccessful, then a nonzero value will be returned.
	 */
	virtual int readContents(MleDwpInput *in);

	/**
	 * Write the contents of the workprint item.
	 *
	 * @param out The output object to write to.
	 *
	 * @return If successful, <b>0</b> will be returned.
	 * If unsuccessful, then a nonzero value will be returned.
	 */
	virtual int writeContents(MleDwpOutput *out);

#if defined(_WINDOWS)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
	/**
	 * Type registry to support reading files.
	 *
	 * In order to be able to read new items out of a workprint,
	 * classes register themselves with their workprint name and
	 * a function that creates an instance into a registry that
	 * the <code>MleDwpItem</code> class manages.  When a type name is encountered
	 * in a read, the registry is consulted and the creation function
	 * is used to create an appropriate instance.  If a registered
	 * class is not found, an instance of type <code>MleDwpUnknown</code> is created.
	 */
	static MleDwpStrKeyDict typeRegistry;
#if defined(_WINDOWS)
#pragma data_seg()
#endif

	/**
	 * Register the specified DWP item.
	 *
	 * @param name The name of the DWP item type.
	 * @param func The creation function for workprints of this type.
	 */
	static void registerType(const char *name,MleDwpItem *(*func)(void));
};


/**
 * This macro should be included in every subclass definition.  It
 * defines some member functions for identity and class registration.
 * The single macro argument is the new class name.
 */
#define MLE_DWP_HEADER(C) \
	public: \
		virtual const char *getTypeName(void) const; \
		virtual MleDwpType getTypeId(void) const; \
		virtual int isa(const char *type) const; \
		virtual int isa(MleDwpType type) const; \
		static MleDwpItem *typeId(void); \
		static void initClass(void)


/**
 * This macro should be included in every subclass implementation.  It
 * implements some functions for identity and class registration.  The
 * arguments to the macro are the new class name and the immediate
 * superclass.
 */
#define MLE_DWP_SOURCE(C,S) \
	const char *C::getTypeName(void) const { return #C; } \
	MleDwpType C::getTypeId(void) const { return typeId; } \
	MleDwpItem *C::typeId(void) { return new C; } \
	extern "C" int strcmp(const char *,const char *); \
	int C::isa(const char *type) const { \
		if ( !strcmp(type,#C) ) return 1; \
		else return S::isa(type); \
	} \
	int C::isa(MleDwpType type) const { \
		if ( type == typeId ) return 1; \
		else return S::isa(type); \
	} \
	void C::initClass(void) { \
		MleDwpItem::registerType(#C,typeId); \
	}


/**
 * This macro should be used in place of MLE_DWP_HEADER for abstract classes.
 */
#define MLE_DWP_ABSTRACT_HEADER(C) \
	public: \
		virtual const char *getTypeName(void) const = 0; \
		virtual MleDwpType getTypeId(void) const = 0; \
		virtual int isa(const char *type) const; \
		virtual int isa(MleDwpType type) const; \
		static MleDwpItem *typeId(void); \
		static void initClass(void)


/**
 * This macro should be used in place of MLE_DWP_SOURCE for abstract classes.
 */
#define MLE_DWP_ABSTRACT_SOURCE(C,S) \
	MleDwpItem *C::typeId(void) { return NULL; } \
	extern "C" int strcmp(const char *,const char *); \
	int C::isa(const char *type) const { \
		if ( !strcmp(type,#C) ) return 1; \
		else return S::isa(type); \
	} \
	int C::isa(MleDwpType type) const { \
		if ( type == typeId ) return 1; \
		else return S::isa(type); \
	} \
	void C::initClass(void) { \
	}


#endif /* __MLE_DWP_ITEM_H_ */
