/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpActorSet.h
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


// Include system header files.#include <stdio.h>
#include <stdlib.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/DwpActor.h"
#include "mle/DwpActorSet.h"
//#include "mle/DwpDelegateAttachment.h"
//#include "mle/DwpDelegateBinding.h"
//#include "mle/DwpFinder.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"
//#include "mle/DwpTblMgr.h"


MLE_DWP_SOURCE(MleDwpActorSet,MleDwpItem);

MleDwpActorSet::MleDwpActorSet(void)
{
	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpActorSet::~MleDwpActorSet(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Dall the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);
}

int
MleDwpActorSet::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	if ( in->readString(buffer) )
		return 1;
	setName(buffer);

	return 0;
}

int
MleDwpActorSet::writeContents(MleDwpOutput *out)
{
	out->writeString(getName());
	return 0;
}

void *
MleDwpActorSet::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpActorSet::operator delete(void *p)
{
	mlFree(p);
}


#if 0
int
MleDwpActorSet::writeContents(MleDppActorGroupOutput *out)
{
	out->writeOpcode(beginActorSetOpcode);
	int setNumber = out->table->setRuntimeName(this->getName());
	out->writeIndex(setNumber);
	return 0;
}

// Used by qsort to sort actors by name.
static int CompareActors(const void* actor1, const void* actor2)
{
	return strcmp((*(const MleDwpActor**) actor1)->getName(),
		(*(const MleDwpActor**) actor2)->getName());
}

// Used by bsearch to find actor with name key.
static int FindActors(const void* key, const void* actor)
{
	return strcmp((const char*) key, (*(const MleDwpActor**) actor)->getName());
}

// Used by qsort to sort delegate attachments by child name.
static int CompareChildren(const void* attach1, const void* attach2)
{
	return strcmp((*(const MleDwpDelegateAttachment**) attach1)->getChild(),
		(*(const MleDwpDelegateAttachment**) attach2)->getChild());
}

// Used by qsort to sort delegate attachments in scene graph, then workprint order.
static int CompareAttachments(const void* arg1, const void* arg2)
{
	const MleDwpDelegateAttachment* attach1 = *(const MleDwpDelegateAttachment**) arg1;
	const MleDwpDelegateAttachment* attach2 = *(const MleDwpDelegateAttachment**) arg2;
	
	if (strcmp(attach1->getChild(), attach2->getParent()) == 0)
		return -1; // Put actor1 before actor 2.
	if (strcmp(attach1->getParent(), attach2->getChild()) == 0)
		return 1; // Put actor1 after actor 2.
	// Otherwise return them in work print attachment order.
	return attach1 < attach2 ? -1 ? attach1 > attach2 : 1 : 0;
}

int
MleDwpActorSet::write(MleDppActorGroupOutput *out)
{
	writeContents(out);	// Write out actor set base.

    // Find all delegate attachments in this actor set.
	MleDwpFinder findAttachments;
	findAttachments.setType(MleDwpDelegateAttachment::typeId);
	findAttachments.setFindAll(1);
	findAttachments.find(this);
	MleDwpDelegateAttachment **attachments = (MleDwpDelegateAttachment**) findAttachments.getItems();
	int numAttachments = findAttachments.getNumItems();

    // Find all actors in this actor set.	.
	MleDwpFinder findActors;
	findActors.setType(MleDwpActor::typeId);
	findActors.setFindAll(1);
	findActors.find(this);
	MleDwpActor **actors = (MleDwpActor**) findActors.getItems();
	int numActors = findActors.getNumItems();

    // Sort the actors by name.
	if (numActors > 0) {
		MleDwpActor **actorRegistry = new (MleDwpActor(*[numActors]));
		MleDwpActor **actorRegistryPtr = actorRegistry;
		qsort(actors, numActors, sizeof(actors[0]), CompareActors);
		if (numAttachments < numActors) {
	
	        // Write out actors that don't implicitly have parenting first.
			int actorIndex = 0;
			if (numAttachments > 0) {
				qsort(attachments, numAttachments, sizeof(attachments[0]), CompareChildren);
				int attachmentIndex = 0;
				do {
					int compare;
					do 
						compare = strcmp(actors[actorIndex]->getName(),
							attachments[attachmentIndex]->getChild());
					while (compare > 0 && ++attachmentIndex < numAttachments);
		
				    // If the actor has the same name as the child, advance both.
					if (compare == 0) {
						++attachmentIndex;
						continue;
					}
					 // If actor name has no corresponding attach child.
					if (compare < 0) {
						*actorRegistryPtr++ = actors[actorIndex];
						actors[actorIndex]->write(out);

						out->writeOpcode(parentDelegateOpcode);
						out->writeIndex(0); 
					}
				} while (++actorIndex < numActors && attachmentIndex < numAttachments);
			}
			while (actorIndex < numActors) {	// These actors have names after last child.
				*actorRegistryPtr++ = actors[actorIndex];	
				actors[actorIndex]->write(out);

				out->writeOpcode(parentDelegateOpcode);
				out->writeIndex(0); 

				actorIndex++;
			}
		}

	    // Sort the attachments so that parents are before children
		// and children with NULL parents are first.
		if (numAttachments > 0) {
			qsort(attachments, numAttachments, sizeof(attachments[0]), CompareAttachments);

	        // Write out the children in workprint attach order.
			int index = 0;
			while (index < numAttachments) {
				MleDwpDelegateAttachment *attach = attachments[index];
				const char* childName = attach->getChild();
				const char* parentName = attach->getParent();
				MleDwpActor *parent = *(MleDwpActor**) bsearch(parentName, actors, numActors,
					sizeof(actors[0]), FindActors);
				MleDwpActor **registryPtr = actorRegistryPtr;
				while (--registryPtr >= actorRegistry && *registryPtr != parent)
					;
				MLE_ASSERT(registryPtr >= actorRegistry);
				attach->parentIndex = registryPtr - actorRegistry + 1 + out->actorCount;
				MleDwpActor* actor = *(MleDwpActor**) bsearch(childName, actors, numActors,
					sizeof(actors[0]), FindActors);
				
				// !!! Add check to make sure same actor is not returned more than once.
				MLE_ASSERT(actor != NULL);
				*actorRegistryPtr++ = actor;	
				actor->write(out);
	
	            // Write out delegate attachments that were not contained in actors.
				if (attach->parentIndex > 0)
					attach->write(out);
				index++;
			}
		}

	    // Write out a text header to associate actor names with group entries at runtime.
		if (out->actorid) {
			MleDwpActor **registryPtr = actorRegistry;
			while (registryPtr < actorRegistryPtr) {
				fprintf(out->actorid, "#define FwPPActor_%s %d\n", (*registryPtr)->getName(),
					registryPtr - actorRegistry + out->actorCount);
				registryPtr++;
			}
			out->actorCount += registryPtr - actorRegistry;
			delete actorRegistry;
		}
	}
	return 0;
}
#endif /* 0 */
