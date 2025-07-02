/** @defgroup MleDWPType Magic Lantern Digital Workprint Library API - Type */

/**
 * @file DwpVoidStarType.h
 * @ingroup MleDWPType
 *
 * This file implements the void * data type used by the Magic Lantern Digital
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Inlucde system header files.
#include <string.h>
#include <stdlib.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Inlclude Digital Workprint header files.
#include "mle/DwpVoidStarType.h"
#include "mle/DwpDataUnion.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DwpMediaRef.h"
//#include "mle/DppActorGroupOutput.h"

MLE_DWP_DATATYPE_SOURCE(MleDwpMediaRefType,"MediaRef",MleDwpVoidStar);

MLE_DWP_DATATYPE_SOURCE(MleDwpVoidStar,"void*",MleDwpDatatype);

int
MleDwpVoidStar::read(MleDwpInput *in,MleDwpDataUnion *data) const
{
    // Set the datatype (and allow release of any existing data).
    data->setDatatype(this);

    // Malloc space.
    data->m_u.v = mlMalloc(MLE_DWP_INPUT_BUFSIZE);

    // Read the string.
    if ( in->readString((char *)data->m_u.v) )
        return 1;

    // Realloc to conserve space.
	data->m_u.v = mlRealloc(data->m_u.v,strlen((char *)data->m_u.v) + 1);

    return 0;
}

int
MleDwpVoidStar::write(MleDwpOutput *out,MleDwpDataUnion *data) const
{
    if ( data->m_u.v )
        out->writeFancyString((char *)data->m_u.v);
    else
        out->writeString("");

    return 0;
}

#if 0
// Used by bsearch to find actor with name key
static int FindMediaRefs(const void* key, const void* mediaRef)
{
	return strcmp((const char*) key, (*(const MleDwpMediaRef**) mediaRef)->getName());
}

int
MleDwpVoidStar::write(MleDppActorGroupOutput *out,MleDwpDataUnion *data) const
{
	out->writeOffsetLength(0);
	MleDwpMediaRef **mediaPtr = (MleDwpMediaRef**) bsearch((char *) data->m_u.v,
		out->references, out->numReferences,
		sizeof(out->references[0]), FindMediaRefs);
	return mediaPtr != NULL ? out->writeIndex(mediaPtr - out->references) : 1;
}
#endif /* 0 */

int
MleDwpVoidStar::getSize(void) const
{
	return sizeof(void*);
}

void
MleDwpVoidStar::set(MleDwpDataUnion *data,void *src) const
{
    // Set the datatype (to allow release of any existing data).
    data->setDatatype(this);

    // Allocate space the length of the string.
    //   Note that the src pointer is a pointer to the string pointer,
    //   *not* a pointer to the string.
    if ( *(char **)src )
    {
        data->m_u.v = mlMalloc(strlen(*(char **)src) + 1);

        // Copy the string.
        strcpy((char *)data->m_u.v,*(char **)src);
    }
    else
        data->m_u.v = NULL;
}

void
MleDwpVoidStar::get(MleDwpDataUnion *data,void *dst) const
{
    // Allocate space to write the string
    if ( data->m_u.v )
    {
        *(char **)dst = (char *)mlMalloc(strlen((char *)data->m_u.v) + 1);

        // Write the string.
        strcpy(*(char **)dst,(char *)data->m_u.v);
    }
    else
        *(char **)dst = NULL;
}

void
MleDwpVoidStar::release(MleDwpDataUnion *data) const
{
	mlFree(data->m_u.v);
	data->m_u.v = NULL;
}

void *
MleDwpVoidStar::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpVoidStar::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpVoidStar::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
MleDwpVoidStar::operator delete[](void* p)
{
    mlFree(p);
}

void *
MleDwpMediaRefType::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpMediaRefType::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpMediaRefType::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
MleDwpMediaRefType::operator delete[](void* p)
{
    mlFree(p);
}
