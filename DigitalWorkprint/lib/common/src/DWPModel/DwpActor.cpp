/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpActor.h
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
#include <stdio.h>
#include <string.h>

// Import Digital Workprint header files.
#include "mle/mlMalloc.h"
#include "mle/DwpActor.h"
//#include "mle/DwpActorTemplate.h"
//#include "mle/DwpFinder.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
//#include "mle/DppActorGroupOutput.h"
//#include "mle/DwpTblMgr.h"

// Import Runtime Player header files.
//#include "mle/actor.h"
//#include "mle/actorclass.h"


MLE_DWP_SOURCE(MleDwpActor,MleDwpItem);

MleDwpActor::MleDwpActor(void)
{
	m_actorClass = NULL;
	m_playerError = DWP_NO_ERROR;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpActor::~MleDwpActor(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all dwp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	if ( m_actorClass )
		mlFree(m_actorClass);
}

void
MleDwpActor::setActorClass(const char *ac)
{
	/* Get rid of the old actor class name. */
	if ( m_actorClass )
		mlFree(m_actorClass);
	
	/* set the new actor class. */
	m_actorClass = ac ? strdup(ac) : NULL;

	/* Load actor class if unknown. */
#if 0	// THIS IS NOW OBSOLETE
	if (fLoadDsosOnStartup && m_actorClass && 
	    MLeDwpActorClass::find(m_actorClass) == NULL )
	{

		MleDwpItem::loader.loadClass(actorClass);
	}
#endif

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

const char *
MleDwpActor::getActorClass(void) const
{
	return m_actorClass;
}

int
MleDwpActor::readContents(MleDwpInput *in)
{
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	if ( in->readString(buffer) )
	{
		in->reportError("error reading Actor name");
		in->skip();
		return 1;
	}
	setName(buffer);

	if ( in->readString(buffer) )
	{
		in->reportError("error reading Actor class");
		in->skip();
		return 1;
	}
	setActorClass(buffer);

	return 0;
}

int
MleDwpActor::writeContents(MleDwpOutput *out)
{
	out->writeString(getName());
	out->writeString(getActorClass());

	return 0;
}

void *
MleDwpActor::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void*
MleDwpActor::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpActor::operator delete(void *p)
{
	mlFree(p);
}

void
MleDwpActor::operator delete[](void* p)
{
	mlFree(p);
}


#if 0
int
MleDwpActor::writeContents(MleDppActorGroupOutput *out)
{
	out->writeOpcode(createActorOpcode);
.
	// Find the actor class
	//   We must find out if we have a template, because a template
	//   is not a class.  A template refers to a class.
	const char *className = getActorClass();

	// Find the root.
	MleDwpItem *root = this;
	while ( root->getParent() )
		root = root->getParent();
	
	// Look for an actor template.
	MleDwpFinder tmplFinder(MleDwpActorTemplate::typeId,className);
	MleDwpItem *tmpl;
	while ( tmpl = tmplFinder.find(root) )
	{
		className = ((MleDwpActorTemplate *)tmpl)->getActorClass();
		tmplFinder.setName(className);
	}

	int actorClassNumber = out->table->actorClassRuntimeName(className); 
	out->writeIndex(actorClassNumber);
	out->currentActorClass = className;
	return 0;
}


// This function overrides the base class write().  It controls what
//   subitems are written out.  Subitems might come from some other
//   hierarchy entirely, specifically from an MleDwpActorTemplate.
//
//   A cheap way to do properties is to write everything from a template
//   and then write everything from the actor.  The fact that the property
//   settings from the actor occur in the Playprint after property settings
//   in from the template will make things come out right.  This is a little
//   wasteful, however, and other kinds of items will not overwrite (e.g.
//   MleDwpDelegateBinding.
//
//   A better way to do things is to register items by type and name, and
//   only take the latest one.  This will work most of the time, but will
//   have some trouble with nonleaf items.  For example, if the property
//   settings come from under an MleDwpInclude item, it is more difficult to
//   register them.  This can be solved with a discriminator in many cases,
//   but we'll ignore this problem for now.
int
MleDwpActor::write(MleDppActorGroupOutput *out)
{
	// Write the actor contents.
	writeContents(out);

	// Create a dictionary of subitems.
	//   We are going to manufacture names for subitems by concatenating
	//   types and names.  For example, a property named "position" will
	//   be indexed by MleDwpProperty:position.
	MleDwpStrKeyDict subItem;

	// Initialize the dictionary with all the local subitems.
	MleDwpItem *child = getFirstChild();
	while ( child )
	{
		// Register in the dictionary.
		//   create a name
		char *key = (char *)mlMalloc(2 +
			strlen(child->getTypeName()) +
			strlen(child->getName()));
		sprintf(key,"%s:%s",
			child->getTypeName(),child->getName());
		.
		// Add to the dictionary
		subItem.set(key,child);

		// Free key space.
		mlFree(key);

		// Warn about subhierarchies.
		if ( child->getFirstChild() )
			printf("warning: hierarchies under MleDwpActor may not work properly with MleDwpActorTemplates\n");

		// Go on to the next child.
		child = child->getNext();
	}
		
	// Search for the template, if any.
	//   Find the root.
	MleDwpItem *root = this;
	while ( root->getParent() )
		root = root->getParent();
	
	// Look for an actor template.
	MleDwpFinder tmplFinder(MleDwpActorTemplate::typeId,getActorClass());
	MleDwpItem *tmpl;
	while ( tmpl = tmplFinder.find(root) )
	{
		// Got one, iterate over the subitems.
		MleDwpItem *child = tmpl->getFirstChild();
		while ( child )
		{
			// Register in the dictionary.
			//   Create a name.
			char *key = (char *)mlMalloc(2 +
				strlen(child->getTypeName()) +
				strlen(child->getName()));
			sprintf(key,"%s:%s",
				child->getTypeName(),child->getName());
			
			// Check if one is already present.
			if ( subItem.find(key) == NULL )
				// Add to the dictionary.
				subItem.set(key,child);
			else
				printf("template subitem %s overriden in actor.\n",
					key);

			// Free key space.
			mlFree(key);

			// warn about subhierarchies.
			if ( child->getFirstChild() )
				printf("warning: hierarchies under MleDwpActorTemplate may not work properly\n");

			// Go on to the next child.
			child = child->getNext();
		}

		// See if there is another template.
		tmplFinder.setName(((MleDwpActorTemplate *)tmpl)->getActorClass());
	}

	// Now loop through the dictionary and write subitems.
	MleDwpDictIter subItemIter(subItem);
	while ( subItemIter.getKey() )
	{
		MleDwpItem *item = (MleDwpItem *)subItemIter.getValue();

		// Write out the item.
		item->write(out);

		subItemIter.next();
	}

	return 0;
}
#endif /* 0 */
