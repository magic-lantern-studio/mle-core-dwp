/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpContainer.cpp
 * @ingroup MleDWPAccess
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
#include <stdio.h>

// Include Digital Workprint header files.
#include "mle/mlMalloc.h"
#include "mle/DwpContainer.h"


MLE_DWP_SOURCE(MleDwpContainer,MleDwpItem);

MleDwpContainer::MleDwpContainer(void)
{
	// Set the default to be transparent.
	m_transparent = 1;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpContainer::~MleDwpContainer(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered.
	// general interest on all wp items (NULL key)
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);
}

void
MleDwpContainer::setTransparent(int flag)
{
	m_transparent = flag;

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

int
MleDwpContainer::write(MleDwpOutput *out,int writeChildren)
{
	// Return value.
	int status = 0;

	// See if we are to write out this object or just its children.
	if ( m_transparent )
	{
		if ( writeChildren )
		{
			MleDwpItem *child = getFirstChild();
			while ( child )
			{
				status |= child->write(out,writeChildren);
				child = child->getNext();
			}
		}
	}
	else
		status |= MleDwpItem::write(out,writeChildren);
	
	return status;
}

MleDwpItem *
MleDwpContainer::copy(int copyChildren,MleDwpItem *parent)
{
	// Remember the transparency state.
	int saveTransparent = m_transparent;

	// Set nontransparent.
	setTransparent(0);

	// Call the base class copy().
	MleDwpItem *rval = MleDwpItem::copy(copyChildren,parent);

	// Set transparency on the copy.
	((MleDwpContainer *)rval)->setTransparent(saveTransparent);

	// Restore the transparency state.
	m_transparent = saveTransparent;

	// Call the modification notification callbacks for this wp item.
	rval->notify(MleDwpChangeNotification);

	return rval;
}

void *
MleDwpContainer::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpContainer::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpContainer::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpContainer::operator delete[](void* p)
{
	mlFree(p);
}
