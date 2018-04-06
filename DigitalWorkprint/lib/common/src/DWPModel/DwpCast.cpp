/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpCast.cpp
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2018 Wizzer Works
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
#include "mle/DwpActor.h"
#include "mle/DwpDatatype.h"
#include "mle/DwpDelegateAttachment.h"
#include "mle/DwpFinder.h"
#include "mle/DwpCast.h"
#include "mle/DwpCastRef.h"
#include "mle/DwpInput.h"
//#include "mle/DwpMediaRef.h"
#include "mle/DwpOutput.h"
#include "mle/DwpProperty.h"
//#include "mle/DppActorCastOutput.h"

//#include "mle/castclass.h"


MLE_DWP_SOURCE(MleDwpCast,MleDwpItem);

MleDwpCast::MleDwpCast(char *cc)
{
	// Default value of gc is non-null, so constructor must have valid cc.
	MLE_ASSERT(NULL != cc);
	m_castClass = strdup(cc);

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpCast::~MleDwpCast()
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	if ( m_castClass )
		mlFree(m_castClass);
}

void
MleDwpCast::setCastClass(const char *cc)
{
	/* Get rid of the old cast class name. */
	if ( m_castClass )
		mlFree(m_castClass);
	
	/* Set the new cast class. */
	m_castClass = cc ? strdup(cc) : NULL;
}

const char *
MleDwpCast::getCastClass(void) const
{
	return m_castClass;
}

int 
MleDwpCast::castOrCastRefFinderCB( MleDwpItem *item, void* )
{
	MleDwpType t = item->getTypeId();

	return (t == MleDwpCast::typeId || t == MleDwpCastRef::typeId);
}

int
MleDwpCast::readContents(MleDwpInput *in)
{
    char buffer[MLE_DWP_INPUT_BUFSIZE];

    if ( in->readString(buffer) )
    {
        in->reportError("error reading Cast name");
        in->skip();
        return 1;
    }
    setName(buffer);

    if ( in->readString(buffer) )
    {
	    in->reportError("error reading Cast class");
	    in->skip();
	    return 1;
    }
    setCastClass(buffer);

    return 0;
}

int
MleDwpCast::writeContents(MleDwpOutput *out)
{
    out->writeString(getName());
    out->writeString(getCastClass());
    return 0;
}

#if 0
int
MleDwpCast::writeContents(MleDppActorCastOutput *out)
{
	// find the cast class
	const char *className = getCastClass();

	int castClassNumber = out->table->castClassRuntimeName(className); 
	out->writeInt(castClassNumber);
	out->currentCastClass = className;

    MleDwpFinder findActors;
    findActors.setType(MleDwpActor::typeId);
    findActors.setFindAll(1);
    findActors.find(this);
    out->writeIndex(findActors.getNumItems());

    return 0;
}

// Used by qsort to sort media references by name.
static int CompareReferences(const void* ref1, const void* ref2)
{
    return strcmp((*(const MleDwpMediaRef**) ref1)->getName(),
        (*(const MleDwpMediaRef**) ref2)->getName());
}


// Used by bsearch in MediaIsReferencedByProperty to find a named property type.
static int FindPropertyData(const void* key, const void* property)
{
    return strcmp((const char*) key, (const char*) (*(MleDwpProperty**) property)->data.u.v);
}
#endif /* 0 */

int
MleDwpCast::write(MleDwpOutput *out,int writeChildren)
{
    return(MleDwpItem::write(out, writeChildren));
}

void *
MleDwpCast::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpCast::operator delete(void *p)
{
	mlFree(p);
}

#if 0
int
MleDwpCast::write(MleDppActorCastOutput *out)
{
    int i;	// Loop variables.

    // Open output file.
    char castChunkFile[FILENAME_MAX];
    sprintf(castChunkFile, "%s.chk", getName());
    out->openFile(castChunkFile);

    // Find all the actors in this cast.
    MleDwpFinder actorFinder(MleDwpActor::typeId,NULL,1);
    actorFinder.find(this);
    const int numActors = actorFinder.getNumItems();
    MleDwpActor **actor = (MleDwpActor **)actorFinder.getItems();

    // Write out the index of this cast\'s class in the runtime
    // table of constructors.
    MleDwpItem *root = this;
    while ( root->getParent() )
	    root = root->getParent();

    FwTblMgr tbl;
    int status = tbl.buildIndexTables(root);
    MLE_ASSERT(status == TRUE);
    TblMgrItems *castClassTable = tbl.getCastClassTable();
    int index = tbl.castClassRuntimeName( this->getCastClass() );
    out->writeInt(index);

    // write out the number of actors in the cast to the chunk
    out->writeIndex(numActors);

    // do weird media reference stuff
    root = this->getParent();
    MleDwpItem *parent;
    while ((parent = root->getParent()) != NULL)
        root = parent;

    // find all media references in this actor set
    // reduce the array to exclude media that is not referred to in a
    // property
    MleDwpFinder findMediaReferences;
    findMediaReferences.setType(MleDwpMediaRef::typeId);
    findMediaReferences.setFindAll(1);
    findMediaReferences.find(root);
    out->numReferences = findMediaReferences.getNumItems();
    MleDwpMediaRef **references = (MleDwpMediaRef**) findMediaReferences.getItems();
    // sort the media references by name
    if (out->numReferences > 0) {
            out->references = new MleDwpMediaRef*[out->numReferences];
            memcpy(out->references, references, sizeof(MleDwpItem**) * out->numReferences);
            qsort(out->references, out->numReferences, sizeof(out->references[0]), CompareReferences);
    }

    // output each actor
    //   Actors are written out in the same order they were created.
    fprintf(out->actorid,"\n/*\n * Global actor index. \n */\n");
    for ( i = 0; i < numActors; i++ ) {
	    // The actor\'s part of the cast chunk file
	    actor[i]->write(out);

	   // The actor\'s part of the actorid.h file.
	    if (out->actorid) {
		    fprintf(out->actorid, "#define FwPPActor_%s %d\n", 
			    actor[i]->getName(), i + out->actorCountCumulative);
		}
	}
    // Expect the actorCount to contain the running sum of every
    // actor we\'ve seen so far.
    out->actorCount += numActors;
    out->actorCountCumulative += numActors;

    // The cast\'s part of the actorid.h file.
    if (out->actorid) {
	    // The actor\'s relative index in the cast.
	    fprintf(out->actorid,"\n/*\n * Actor indices within the cast. \n */\n");
	    for ( i = 0; i < numActors; i++ ) {
		    fprintf(out->actorid,"#define FwPPCastActorID_%s_%s %d\n",
			    getName(), actor[i]->getName(), i);
	    }
    }

    // Write the cast\'s global index as part of the castid.h file.
    // If this is a castRef, then don\'t write anything.
    if (out->castid) {
	    int index = tbl.castInstanceRuntimeName(getName());
	    fprintf(out->castid, "#define FwPPCastID_%s %d\n", 
		    getName(), index);
    }

    // Find all delegate attachments.
    MleDwpFinder attachmentFinder(MleDwpDelegateAttachment::typeId,NULL,1);
    attachmentFinder.find(this);
    const int numAttachments = attachmentFinder.getNumItems();
    const MleDwpDelegateAttachment **attachment =
    	(MleDwpDelegateAttachment **)attachmentFinder.getItems();

    // Loop over the attachments.
    for ( i = 0; i < numAttachments; i++ )
    {
		// Find the parent
		int parentIndex;
		for ( parentIndex = 0; parentIndex < numActors; parentIndex++ )
		{
			if ( !strcmp(actor[parentIndex]->getName(),attachment[i]->getParent()) )
			break;
		}

		// Find the child
		int childIndex;
		for ( childIndex = 0; childIndex < numActors; childIndex++ )
		{
			if ( !strcmp(actor[childIndex]->getName(),attachment[i]->getChild()) )
			break;
		}

		// XXX verification
		//   Should verify here that the actors involved have delegate
		//   bindings in the same forum.  This is a little difficult
		//   because of the possible presence of templates.

		if ( parentIndex == numActors )
		{
			fprintf(stderr,"error generating delegate binding index for %s.\n",attachment[i]->getParent());
			continue;
		}
		if ( childIndex == numActors )
		{
			fprintf(stderr,"error generating delegate binding index for %s.\n",attachment[i]->getChild());
			continue;
		}

		// Write the delegate attachment opcode and the parent and child
		// indices
		out->writeOpcode(parentDelegateOpcode);
		out->writeIndex(parentIndex);
		out->writeOpcode(childDelegateOpcode);
		out->writeIndex(childIndex);
    }

    return 0;
}
#endif /* 0 */
