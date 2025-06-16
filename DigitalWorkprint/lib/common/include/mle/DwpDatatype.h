/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpDatatype.h
 * @ingroup MleDWPType
 *
 * This file defines the base data type used by the Magic Lantern Digital
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

#ifndef __MLE_DWP_DATATYPE_H_
#define __MLE_DWP_DATATYPE_H_

// Include Digital Workprint header files.
#include "mle/DwpStrKeyDict.h"

class MleDwpInput;
class MleDwpOutput;
//class MleDppActorGroupOutput;
class MleDwpDatatype;
class MleDwpDataUnion;

/**
 * An MleDwpDatatypeType is an identifier that belongs to a datatype class.
 * Its actual type (a function pointer) is an artifact of implementation
 * and can be ignored.  Normal usage is equality testing between two
 * instances of MleDwpDatatypeType, or as an argument to isa().
 */
typedef MleDwpDatatype *(*MleDwpDatatypeType)(void);

/**
 * This is the property datatype registry class.  It is a subclass
 * of the string dictionary type.  The only addition is that
 * it supplies entries that know to delete the datatype when
 * the dictionary is deleted.
 */
class MLE_DWP_API MleDwpDatatypeDict : public MleDwpStrKeyDict
{
  public:

	MleDwpDatatypeDict(int hashsize) : MleDwpStrKeyDict(hashsize) {}

	virtual MleDwpDictEntry *makeEntry(void);

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
 *
 * @brief This is the workprint datatype base class.
 *
 * This class contains the API to read, write, and transfer
 * data of a particular type.  For every different property type,
 * there is a subclass of MleDwpDatatype and one instance of that
 * subclass.  The instance is obtained through the type registry
 * and can then be used to operate on a property.
 *
 * MleDwpDatatype has been modified not to operate on property values
 * directly, but to work on a MleDwpDataUnion that the MleDwpProperty
 * item includes to hold its data.  This allows newly defined items
 * to include the union object and to have it manipulated by the
 * type objects.
 *
 * @see MleDwpDataUnion
 */
class MLE_DWP_API MleDwpDatatype
{
  public:

	/**
	 * @brief Get the identity of the data type.
	 *
	 * @returns The name of the datatype ("int", "float", etc.) is returned.
	 */
	virtual const char *getName(void) const = 0;

	/**
	 * @brief Determine if specified type is of this type.
	 *
	 * @param type The specified type to test.
	 *
	 * @returns Nonzero is returned if the instance is of the specified type.
	 * This includes subclasses of the type. 
	 */
	virtual int isa(MleDwpDatatypeType type) const = 0;

	/**
	 * @brief Read the property values for the properties.
	 *
	 * A MleDwpProperty item in a workprint
	 * contains a name and then some data it doesn't know
	 * how to read by itself.  This function will
	 * perform the i/o on that data.
	 */
	virtual int read(MleDwpInput *in, MleDwpDataUnion *data) const = 0;

	/**
	 * @brief Write the property values for the properties.
	 *
	 * A MleDwpProperty item in a workprint
	 * contains a name and then some data it doesn't know
	 * how to write by itself.  This function will
	 * perform the i/o on that data.
	 */
	virtual int write(MleDwpOutput *out, MleDwpDataUnion *data) const = 0;

	//virtual int write(MleDppActorGroupOutput *out, MleDwpDataUnion *data) const = 0; 

	/**
	 * Deallocate memory set aside for reading the data type.
	 *
	 * When a type is set or read, space may be allocated for
	 * the type data.  If it is, a corresponding deallocator
	 * is needed.  The base class function does nothing.
	 */
	virtual void release(MleDwpDataUnion *data) const ;

	/**
	 * Get the size information for this data type.
	 *
	 * @returns The number of bytes this data type requires during rehearsal is returned.
	 */
	virtual int getSize(void) const = 0;

	/**
	 * Copies the value pointed to by src into the
	 * property, allocating memory to do that if necessary.
	 */
	virtual void set(MleDwpDataUnion *data,void *src) const = 0;

	/**
	 * Copies the value in the property to a memory
	 * area pointed to by dst.
     */
	virtual void get(MleDwpDataUnion *data,void *dst) const = 0;

	/**
	 * Registry lookup for the specified type.
	 *
	 * @returns findType() is a static member function that returns
	 * a type object given the name of the type.
	 */
	static const MleDwpDatatype *findType(const char *type);

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
     * Dump the MleDwpDatatype Dictionary for debugging purposes.
     */
    static void dumpRegistry(void);

  protected:

#if defined(_WINDOWS)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
    static MleDwpDatatypeDict *g_registry;
#if defined(_WINDOWS)
#pragma data_seg()
#endif
};

/**
 * This macro should be included in every subclass definition.  It
 * defines some member functions for identity and class registration.
 * The single macro argument is the new class name.
 */
#define MLE_DWP_DATATYPE_HEADER(C) \
	public: \
		virtual const char *getName(void) const; \
		virtual int isa(MleDwpDatatypeType type) const; \
		static MleDwpDatatype *typeId(void); \
		static void initClass(void)

/**
 * This macro should be included in every subclass implementation.  It
 * implements some functions for identity and class registration.  The
 * arguments to the macro are the new class name, the data type name and
 * the immediate superclass.
 */
#define MLE_DWP_DATATYPE_SOURCE(C,N,S) \
	MleDwpDatatype *C::typeId(void) { return new C; } \
	const char *C::getName(void) const { return N; } \
	int C::isa(MleDwpDatatypeType type) const { \
		if ( type == typeId ) return 1; \
		else return S::isa(type); \
	} \
	void C::initClass(void) { g_registry->set(N,new C); }

/**
 * This macro should be used in place of MLE_DWP_DATATYPE_HEADER for abstract classes.
 */
#define MLE_DWP_DATATYPE_ABSTRACT_HEADER(C) \
	public: \
		virtual const char *getName(void) const = 0; \
		virtual int isa(MleDwpDatatypeType type) const; \
		static MleDwpDatatype *typeId(void)

/**
 * This macro should be used in place of MLE_DWP_DATATYPE_SOURCE for abstract classes.
 */
#define MLE_DWP_DATATYPE_ABSTRACT_SOURCE(C,S) \
	MleDwpDatatype *C::typeId(void) { return NULL; } \
	int C::isa(MleDwpDatatypeType type) const { \
		if ( type == typeId ) return 1; \
		else return S::isa(type); \
	}

#endif /* __MLE_DWP_DATATYPE_H_ */
