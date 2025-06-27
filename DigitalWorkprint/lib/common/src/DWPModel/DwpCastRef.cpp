/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpCastRef.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
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


// Include system header files.
#include <stdlib.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprint header files.
#include "mle/DwpDatatype.h"
#include "mle/DwpFinder.h"
#include "mle/DwpCast.h"
#include "mle/DwpCastRef.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"


MLE_DWP_SOURCE(MleDwpCastRef,MleDwpItem);

MleDwpCastRef::MleDwpCastRef(void)
{
	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpCastRef::~MleDwpCastRef(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key)
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

}

int
MleDwpCastRef::readContents(MleDwpInput *in)
{
    char buffer[MLE_DWP_INPUT_BUFSIZE];

    if ( in->readString(buffer) )
    {
        in->reportError("error reading CastRef cast-name");
        in->skip();
        return 1;
    }
    setName(buffer);

    return 0;
}

int
MleDwpCastRef::writeContents(MleDwpOutput *out)
{
    out->writeString(getName());
    return 0;
}

#if 0
int
MleDwpCastRef::writeContents(MleDppActorCastOutput *)
{
    // Our name is written by the base class write() method
    // so we have nothing to add.

    return 0;
}
#endif /* 0 */

int
MleDwpCastRef::write(MleDwpOutput *out,int writeChildren)
{
    return(MleDwpItem::write(out, writeChildren));
}

void *
MleDwpCastRef::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpCastRef::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpCastRef::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
MleDwpCastRef::operator delete[](void* p)
{
    mlFree(p);
}

#if 0
int
MleDwpCastRef::write(MleDppActorCastOutput *)
{
    // Must start our finding from the root to find the cast we refer to.
    MleDwpItem *root = this;
    while ( root->getParent() )
	    root = root->getParent();

    // find all the cast we refer to
    MleDwpFinder castFinder(MleDwpCast::typeId, getName(), 1);
    castFinder.find(root);

    MleDwpCast *cast = (MleDwpCast *)castFinder.getItem();
    if (NULL == cast) {
	    fprintf(stderr,"Error: finding Cast %s from CastRef. Expecting 1 result, found %d\n", getName(), castFinder.getNumItems());
	    return -1;
    }
    MLE_ASSERT(NULL != cast);

    // The cast will already be written to a playprint chunk from
    // elsewhere in the workprint, so there is really nothing we have
    // to write.

    return 0;
}
#endif /* 0 */
