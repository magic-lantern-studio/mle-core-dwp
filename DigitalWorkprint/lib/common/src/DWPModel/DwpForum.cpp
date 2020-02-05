/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpForum.cpp
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 *
 * @deprecated The MleDwpForum class has been replaced with the
 * MleDwpSet class.
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

//Include Digital Workprint header files.
#include "mle/DwpForum.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"


MLE_DWP_SOURCE(MleDwpForum,MleDwpItem);

MleDwpForum::MleDwpForum(void)
{
	m_type = NULL;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpForum::~MleDwpForum(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	delete m_type;
}

int
MleDwpForum::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	/* Read the forum name. */
	if ( in->readString(buffer) )
	{
		in->reportError("can't read forum name");
		in->skip();
		return 1;
	}
	setName(buffer);

	/* Read the forum type. */
	if ( in->readString(buffer) )
	{
		in->reportError("can't read forum type");
		in->skip();
		return 1;
	}
	setType(buffer);

	return 0;
}

void
MleDwpForum::setType(const char *t)
{
	delete m_type;

	m_type = t ? strcpy(new char[strlen(t) + 1],t) : NULL;

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

const char *
MleDwpForum::getType(void) const
{
	return m_type;
}

int
MleDwpForum::writeContents(MleDwpOutput *out)
{
	out->writeString(getName());
	out->writeString(getType());
	return 0;
}

void *
MleDwpForum::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpForum::operator delete(void *p)
{
	mlFree(p);
}

#if 0
int
MleDwpForum::writeContents(MleDppActorGroupOutput *out)
{
    int forumNumber = out->table->forumRuntimeName(this->getName());

    out->writeOpcode(createForumOpcode);
    out->writeIndex(forumNumber);
    out->currentForumClass = this->getType();

    return 0;
}

int
MleDwpForum::write(MleDppActorGroupOutput *out)
{
    char forumChunkFile[FILENAME_MAX];
    MleDwpItem *child;

    /*
     * Open forum chunk file.
     */
    sprintf(forumChunkFile, "%s.chk", getName());
    out->openForumChunkFile(forumChunkFile);

    /*
     * Write out the createForumOpcode and forum index.
     */
    writeContents(out);

    /*
     * Write out forum (properties) configuration information.
     */
    child = getFirstChild();
    while(child) {
	    child->write(out);
	    child = child->getNext();
    }

    /*
     * Close forum chunk file.
     */
    out->closeForumChunkFile();

    return 0;
}
#endif /* 0 */
