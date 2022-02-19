/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpDatatype.cpp
 * @ingroup MleDWPType
 *
 * This file implements the data type used by the Magic Lantern Digital
 * Workprint Library API.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2022 Wizzer Works
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include "mle/DwpDatatype.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpItem.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"

#if defined(WIN32)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
//MleDwpDatatypeDict MleDwpDatatype::g_registry(MLE_DWP_DICT_DEFHASHSIZE);
MleDwpDatatypeDict *MleDwpDatatype::g_registry = new MleDwpDatatypeDict(MLE_DWP_DICT_DEFHASHSIZE);
#if defined(WIN32)
#pragma data_seg()
#pragma comment(linker, "/section:.GLOBALS,rws")
#endif

// This class is the datatype dict entry that knows how to delete the
// datatype when the datatype registry is deleted.
class MleDwpDatatypeDictEntry : public MleDwpStrKeyDictEntry
{
  public:

	virtual ~MleDwpDatatypeDictEntry() { delete (MleDwpDatatype *)m_value; }

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

// This function is the override for MleDwpStrKeyDict that
// returns an entry of the above new type.
MleDwpDictEntry *
MleDwpDatatypeDict::makeEntry(void)
{
	return new MleDwpDatatypeDictEntry;
}

void *
MleDwpDatatypeDict::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpDatatypeDict::operator delete(void *p)
{
	mlFree(p);
}

// This function is a virtual that tests to see if this item
// is descended from the type passed in as an argument.  This
// is really the terminal test.  In subclasses, this is implemented
// using a macro:
//
// int C::isa(MleDwpDatatypeType type) const {
//    if ( type == typeId ) return 1;
//        else return S::isa(type);
// }
//
// Note that this potentially makes calls all the way up the
// class hierarchy.
int
MleDwpDatatype::isa(MleDwpDatatypeType type) const
{
	return type == NULL;
}

// This function is a static member function that returns a
// pointer to a type item given a type name.
const MleDwpDatatype *
MleDwpDatatype::findType(const char *type)
{
	MleDwpDatatype *rval = (MleDwpDatatype *)g_registry->find(type);

	// If not found, try a DSO load.
	if ( rval == NULL && !MleDwpItem::g_loader.loadClass(type,"MleDwp") ) {
		rval = (MleDwpDatatype *)g_registry->find(type);
	}

	return rval;
}

// This is the default virtual deallocator for property data.
// Property data is kept in a union inside the property.  One
// of the union members is a void*, which can be used by subclasses
// to point to allocated data.  If it does, subclasses need to
// override this function to deallocate the data.
void
MleDwpDatatype::release(MleDwpDataUnion *) const
{
	// Do nothing for now.
}

void *
MleDwpDatatype::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpDatatype::operator delete(void *p)
{
	mlFree(p);
}

void
MleDwpDatatype::dumpRegistry(void)
{
	MleDwpDictIter iterator(*g_registry);

	do {
        const void *key = iterator.getKey();
        fprintf(stdout, "DwpDatatype key: %s\n", (const char *)key);

        const void *value = iterator.getValue();
	} while (iterator.next());
}

void *
MleDwpDatatypeDictEntry::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpDatatypeDictEntry::operator delete(void *p)
{
	mlFree(p);
}
