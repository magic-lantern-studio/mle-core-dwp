/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpItem.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the base digital workprint item used by the Magic Lantern Digital
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

// Include system header files.
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/DwpItem.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
#include "mle/DwpStrKeyDict.h"
#include "mle/DwpUnknown.h"
#include "mle/DwpInclude.h"
#include "mle/DwpDiscriminator.h"

#include "mle/mlAssert.h"
#include "mle/mlMalloc.h"


// Define the special container/include item for readAll().
class _MleDwpTop : public MleDwpInclude
{
	MLE_DWP_HEADER(_MleDwpTop);

  public:

	// This item is always transparent, i.e. it never writes itself
	// out, only its children, so it doesn't need its own readContents()
	// function.

	// write() just writes the children of this item
	virtual int write(MleDwpOutput *out,int writeChildren = 1);
};


// Implement the special container/include item.
MLE_DWP_SOURCE(_MleDwpTop,MleDwpInclude);


// This write function just writes the children, making the item transparent.
int
_MleDwpTop::write(MleDwpOutput *out,int writeChildren)
{
	int status = 0;

	if ( writeChildren )
	{
		MleDwpItem *child = getFirstChild();
		while ( child )
		{
			status |= child->write(out,writeChildren);
			child = child->getNext();
		}
	}

	return status;
}


// define the linked list item for item tags
struct _MleDwpTag {
	char *tag;
	_MleDwpTag *next;
};


// define the static member registry for item names and creation functions
MleDwpStrKeyDict MleDwpItem::typeRegistry;


// implement the DSO loader object
MleDSOLoader MleDwpItem::g_loader;


MleDwpItem::MleDwpItem(void)
{
	/* initialize variables */
	m_name = NULL;

	m_parent = NULL;
	m_first = NULL;
	m_last = NULL;
	m_prev = NULL;
	m_next = NULL;

	m_tagFirst = m_tagLast = NULL;

	m_discriminator = NULL;
}


MleDwpItem::~MleDwpItem()
{
    // call the notification callbacks for this specific wp item
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// call the notification callbacks for those who registered
	// general interest on all wp items (NULL key)
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	// check if we have a parent
	if ( m_parent )
		m_parent->removeChild(this);

	/* recursively delete children */
	/*   Don't use the functions getFirstChild(), etc. here because */
	/*   we really do want to delete all the children */
	while ( m_first )
	{
		// first will be updated by the removeChild() call, so
		//   we need a temporary and no updating of the loop variable
		MleDwpItem *tmp = m_first;

		// remove the child
		removeChild(tmp);

		// then delete it
		delete tmp;
	}

	// remove any tags
	while ( m_tagFirst )
	{
		_MleDwpTag *tmp = m_tagFirst;
		m_tagFirst = m_tagFirst->next;

		delete tmp->tag;
		delete tmp;
	}

	/* free allocated storage */
	delete m_name;
}


/*
	This function returns a copy of an item.  It works by using
	in-memory transcription.  This is potentially slow, but copying
	is probably not a performance bottleneck.
*/
MleDwpItem *
MleDwpItem::copy(int copyChildren,MleDwpItem *parent)
{
	/* create an in-memory output object */
	MleDwpOutput out;
	out.setBuffer(NULL,0);		// let the object allocate memory

	/* transcribe the object without children */
	write(&out,0);

	/* get the buffer back */
	char *buffer = out.getBuffer();

	/* create an in-memory input object */
	MleDwpInput in;
	in.setBuffer(buffer);

	/* transcribe the object in */
	MleDwpItem *item = MleDwpItem::read(&in,parent);

	/* free the buffer */
	mlFree(buffer);

	/* copy the children if requested */
	if ( copyChildren )
	{
		MleDwpItem *child = getFirstChild();
		while ( child )
		{
			child->copy(copyChildren,item);
			child = child->getNext();
		}
	}

	// Copy the discriminator if item not being added to a parent
	// This will also set discrim on all children
	if (!parent)
	    item->setDiscriminator(this->getDiscriminator());

	// call the change notification callbacks for the modified wp item
	item->notify(MleDwpChangeNotification);

	return item;
}


/*
    Does a full copy of wp item, by turning off any discriminator. Without
    doing this, (if you call copy() directly), child objs 
    that don't match discrim won't be copied.
*/

MleDwpItem *
MleDwpItem::copyWithoutDiscrim(int copyChildren, MleDwpItem *parent)
{
	// turn off discrim during our copy, so we copy everything
	short oldEnabled;
	MleDwpDiscriminator *discrim = getDiscriminator();
	if (discrim)
		oldEnabled = discrim->setEnabled(0);
    
	// copy the item
	MleDwpItem *copiedItem = copy(copyChildren, parent);
    
	if (discrim) 
		discrim->setEnabled(oldEnabled);
    
	return copiedItem;
}


/*
	This function returns the optional name of an item.  At present,
	it returns the string "NULL" when no name has been defined.  In
	the future, it may generate a string from the item pointer for
	use as a generated name.  This will require recognizing these
	generated names after a write/read cycle.
*/
const char *
MleDwpItem::getName(void) const
{
	// if the name hasn't been set then use the class name
	if ( ! m_name )
	    return "NULL";
	else return m_name;
}


/*
	This function sets the name of an item.  The name is unset by
	passing a null pointer.
*/
void
MleDwpItem::setName(const char *n)
{
	/* free the old name */
	if ( m_name )
		delete m_name;
	
	/* check to see if the pointer is non-null */
	if ( n )
	{
		/* set the new name */
		m_name = new char[strlen(n) + 1];
		strcpy(m_name,n);
	}
	else
		m_name = NULL;

	// call the change notification callbacks for this wp item
	notify(MleDwpChangeNotification);
}


/*
	This function is a virtual that returns the item type name.
	It is implemented by macro in subclasses.
*/
const char *
MleDwpItem::getTypeName(void) const
{
	return "MleDwpItem";
}


/*
	This function is a virtual that returns the item type identifier.
        It is implemented by macro in subclasses.
 */
MleDwpType MleDwpItem::getTypeId(void) const
{
        return(typeId);
}


/*
	Create and return a new MleDwpItem.  Should never really be called
        since MleDwpItem is an abstract class.
*/
MleDwpItem *MleDwpItem::typeId(void)
{
	abort();
	return(NULL);
}


/*
	This function is a virtual that tests to see if this item
	is descended from the type passed in as an argument.  This
	is really the terminal test.  In subclasses, this is implemented
	using a macro:

		int MyClass::isa(const char *type) const
		{
			if ( !strcmp(type,"MyClass") )
				return 1;
			else
				return MySuperClass::isa(type);
		}
	
	Note that this potentially makes calls all the way up the class
	hierarchy.
*/
int
MleDwpItem::isa(const char *type) const
{
	MLE_ASSERT(type);

	return !strcmp(type,"MleDwpItem");
}


/*
	This function is a virtual that tests to see if this item
	is descended from the type passed in as an argument.  This
	is really the terminal test.  In subclasses, this is implemented
	using a macro (see previous isa() implementation).
*/
int MleDwpItem::isa(MleDwpType type) const
{
	return type == typeId;
}


/*
	This function returns the parent of this item in the containership
	hierarchy.  In the future, this function will be augmented to
	do selective traversal based on flags such as delivery platform
	type or to make file container objects transparent.
*/
MleDwpItem *
MleDwpItem::getParent(void) const
{
	if ( m_discriminator )
	{
		if ( m_parent == NULL )
			return m_parent;
		
		// try the discriminator on where we would ordinarly go
		switch ( m_discriminator->discriminate(m_parent) )
		{
		    case MleDwpDiscriminator::USE:
			// do the normal thing
			return m_parent;
		    default:
			// it might be considered an error to be here,
			//   since you shouldn't be at an item not being
			//   used.  However, because discrimination can
			//   be reconfigured, we won't complain.
			return m_parent->getParent();
		}
	}
	else
		return m_parent;
}


/*
    This function returns the first ancestor of the given type in
    the containership hierarchy.
*/
MleDwpItem *
MleDwpItem::getAncestor(MleDwpType type)
{
    MleDwpItem *parent = this;
    while ((parent = parent->getParent()) != NULL)
    {
	if (parent->getTypeId() == type)
	    return parent;
    }

    return NULL;
}


/*
	This function returns the first child of this item in the containership
	hierarchy.  In the future, this function will be augmented to
	do selective traversal based on flags such as delivery platform
	type or to make file container objects transparent.
*/
MleDwpItem *
MleDwpItem::getFirstChild(void) const
{
	if ( m_discriminator )
	{
		if ( m_first == NULL )
		{
			// if we are at a PHANTOM, return next
			if ( m_discriminator->discriminate(this) ==
			    MleDwpDiscriminator::PHANTOM )
				return getNext();
			else
				return m_first;
		}

		// try the discriminator on where we would ordinarly go
		switch ( m_discriminator->discriminate(m_first) )
		{
		    case MleDwpDiscriminator::USE:
			// do nothing special
			return m_first;
		    case MleDwpDiscriminator::PHANTOM:
			// put children at this level
			return m_first->getFirstChild();
		    case MleDwpDiscriminator::SKIP:
			// the next item becomes first
			return m_first->getNext();
		}

		// shouldn't get here but the compiler complains
		MLE_ASSERT(0);
		return NULL;
	}
	else
		return m_first;
}


/*
	This function returns the last child of this item in the containership
	hierarchy.  In the future, this function will be augmented to
	do selective traversal based on flags such as delivery platform
	type or to make file container objects transparent.
*/
MleDwpItem *
MleDwpItem::getLastChild(void) const
{
	if ( m_discriminator )
	{
		if ( m_last == NULL )
		{
			// if we are at a PHANTOM, return prev
			if ( m_discriminator->discriminate(this) ==
			    MleDwpDiscriminator::PHANTOM )
				return getPrev();
			else
				return m_last;
		}

		// try the discriminator on where we would ordinarly go
		switch ( m_discriminator->discriminate(m_last) )
		{
		    case MleDwpDiscriminator::USE:
			// do nothing special
			return m_last;
		    case MleDwpDiscriminator::PHANTOM:
		    	// put children at this level
			return m_last->getLastChild();
		    case MleDwpDiscriminator::SKIP:
			// just make the previous one last
			return m_last->getPrev();
		}

		// shouldn't get here but the compiler complains
		MLE_ASSERT(0);
		return NULL;
	}
	else
		return m_last;
}


/*
	This function returns the previous sibling of this item in the
	containership hierarchy.  In the future, this function will be
	augmented to do selective traversal based on flags such as
	delivery platform type or to make file container objects transparent.
*/
MleDwpItem *
MleDwpItem::getPrev(void) const
{
	if ( m_discriminator )
	{
		if ( m_prev == NULL )
		{
			// if parent is a PHANTOM, return parent prev
			if ( m_parent && m_discriminator->discriminate(m_parent) ==
			    MleDwpDiscriminator::PHANTOM )
				return m_parent->getPrev();
			else
				return m_prev;
		}

		// try the discriminator on where we would ordinarly go
		switch ( m_discriminator->discriminate(m_prev) )
		{
		    case MleDwpDiscriminator::USE:
			// do nothing special
			return m_prev;
		    case MleDwpDiscriminator::PHANTOM:
			// use the children of the PHANTOM
			return m_prev->getLastChild();
		    case MleDwpDiscriminator::SKIP:
			// just skip over it
			return m_prev->getPrev();
		}

		// shouldn't get here but the compiler complains
		MLE_ASSERT(0);
		return NULL;
	}
	else
		return m_prev;
}


/*
	This function returns the next sibling of this item in the
	containership hierarchy.  In the future, this function will
	be augmented to do selective traversal based on flags such as
	delivery platform type or to make file container objects transparent.
*/
MleDwpItem *
MleDwpItem::getNext(void) const
{
	if ( m_discriminator )
	{
		if ( m_next == NULL )
		{
			// if parent is a PHANTOM, return parent prev
			if ( m_parent && m_discriminator->discriminate(m_parent) ==
			    MleDwpDiscriminator::PHANTOM )
				return m_parent->getNext();
			else
				return m_next;
		}

		// try the discriminator on where we would ordinarly go
		switch ( m_discriminator->discriminate(m_next) )
		{
		    case MleDwpDiscriminator::USE:
			// do nothing special
			return m_next;
		    case MleDwpDiscriminator::PHANTOM:
			// use the children of the PHANTOM
			return m_next->getFirstChild();
		    case MleDwpDiscriminator::SKIP:
			// just skip over it
			return m_next->getNext();
		}

		// shouldn't get here but the compiler complains
		MLE_ASSERT(0);
		return NULL;
	}
	else
		return m_next;
}


/*
	This function sets the current global discriminator.  To disable
	discrimination, set this with NULL.
*/
void
MleDwpItem::setDiscriminator(MleDwpDiscriminator *d)
{
	MleDwpItem *child;

	m_discriminator = d;

	// propagate new discriminator to all descendents
	for (child = m_first; child != NULL; child = child->m_next) {
		child->setDiscriminator(d);
	}
}


/*
	This function returns the current global discriminator.
*/
MleDwpDiscriminator *
MleDwpItem::getDiscriminator()
{
	return m_discriminator;
}


/*
	This function inserts a new item into the containership hierarchy
	before the item beforeChild.  If beforeChild is NULL, the new
	item becomes the last child.
*/
void
MleDwpItem::insertChild(MleDwpItem *child,MleDwpItem *beforeChild)
{
	MLE_ASSERT(child);

	// don't steal a child from another parent
	MLE_ASSERT(child->m_parent == NULL);

	// if beforeChild is non-NULL, its parent might not be this item
	//   iff there are PHANTOM items between this and beforeChild.
	//   If that is not the case, then this call is an error.  If
	//   that is the case, we can call insertChild() with the real
	//   parent.
	if ( beforeChild && beforeChild->m_parent != this )
	{
#if defined(MLE_DEBUG)
		// verify that everything in between is PHANTOM
		MleDwpItem *p = beforeChild->m_parent;
		while ( p )
		{
			// see if this is a match
			if ( p == this )
				break;
			
			// otherwise, it should be a PHANTOM node
			//   If the assertion here is failed, it means
			//   that the parent and child arguments do not
			//   have that relation.
			MLE_ASSERT(m_discriminator);
			MLE_ASSERT(m_discriminator->discriminate(p) ==
				MleDwpDiscriminator::PHANTOM);
				
			p = p->m_parent;
		}
#endif
		
		// use the beforeChild parent
		beforeChild->m_parent->insertChild(child,beforeChild);
	}
	else
	{
		/* set the child parent */
		child->m_parent = this;
	
		/* if beforeChild is NULL, make the child the last child */
		if ( beforeChild == NULL )
		{
			if ( m_last == NULL )
				m_last = m_first = child;
			else
			{
				m_last->m_next = child;
				child->m_prev = m_last;
				m_last = child;
			}
		}
		else
		{
			/* make sure beforeChild is a child of this item */
			MLE_ASSERT(beforeChild->m_parent == this);
	
			/* insert the child in the list before beforeChild */
			if ( beforeChild->m_prev )
				beforeChild->m_prev->m_next = child;
			else
				m_first = child;
	
			child->m_prev = beforeChild->m_prev;
			child->m_next = beforeChild;
	
			beforeChild->m_prev = child;
		}
	}

	// set child's discriminator to same
	child->setDiscriminator(m_discriminator);

	// call the child added notification callbacks
	notify(MleDwpChildInsertedNotification);

	// notify my top level container of descendant addition
	MleDwpItem *	top;
	for (top = this; top->m_parent != NULL; top = top->m_parent)
	    ;
	top->notify(MleDwpDescendentInsertedNotification);
}


/*
	This function removes an item from the child list.
*/
void
MleDwpItem::removeChild(MleDwpItem *child)
{
	MLE_ASSERT(child);

	// child might not be a real child of this item if discriminators
	//   are being used.  If there are only PHANTOM nodes between this
	//   item and child, this is a legal removal.
	MleDwpItem *p = child->m_parent;

#if defined(MLE_DEBUG)
	// start with the direct parent
	while ( p )
	{
		// see if this is a match
		if ( p == this )
			break;
		
		// otherwise, it should be a PHANTOM node
		//   If there is an assertion failure here, it means that
		//   the child argument is not a proper child of this item.
		MLE_ASSERT(m_discriminator);
		MLE_ASSERT(m_discriminator->discriminate(p) ==
			MleDwpDiscriminator::PHANTOM);
			
		p = p->m_parent;
	}

	// if no match, it's an error
	MLE_ASSERT(p);
#endif

	if ( child->m_prev )
		child->m_prev->m_next = child->m_next;
	else
		child->m_parent->m_first = child->m_next;
	
	if ( child->m_next )
		child->m_next->m_prev = child->m_prev;
	else
		child->m_parent->m_last = child->m_prev;

	child->m_prev = child->m_next = child->m_parent = NULL;

	// call the child added notification callbacks - note we
	// are doing notification on the parent selected in while-loop
	// above; we might want to send notification to immed. parent
	// (which, if p!=child->parent, is a phantom)
	p->notify(MleDwpChildRemovedNotification);

	// notify my top level container of descendant removal
	MleDwpItem *	top;
	for (top = this; top->m_parent != NULL; top = top->m_parent)
	    ;
	top->notify(MleDwpDescendentRemovedNotification);
}


/*
	This function sets a string tag on an item.  There is a linked
	list of tag structures.  The list is searched for a matching
	tag and if one is not found, then a new list entry is created
	and prepended.
*/
void
MleDwpItem::addTag(const char *tag)
{
	MLE_ASSERT(tag);

	// check to see if this tag is already here
	if ( hasTag(tag) )
		return;
	
	// otherwise, append a new list entry
	_MleDwpTag *entry = new _MleDwpTag;
	entry->tag = strcpy(new char[strlen(tag) + 1],tag);
	entry->next = NULL;

	if ( m_tagLast )
		m_tagLast = m_tagLast->next = entry;
	else
		m_tagFirst = m_tagLast = entry;
}


/*
	This function removes a tag from the tag list.
*/
void
MleDwpItem::removeTag(const char *tag)
{
	MLE_ASSERT(tag);

	_MleDwpTag **entry = &m_tagFirst;
	_MleDwpTag *last = NULL;		// remembers last tag
	while ( *entry )
	{
		// compare for a match
		if ( !strcmp((*entry)->tag,tag) )
		{
			// save the pointer
			_MleDwpTag *tmp = *entry;

			// do list removal
			*entry = (*entry)->next;

			// check for removal of last list element
			if ( tmp == m_tagLast )
				m_tagLast = last;	// fix last pointer

			// do teardown and deletion
			delete tmp->tag;
			delete tmp;

			break;
		}

		last = *entry;
		entry = &(*entry)->next;
	}
}

		
/*
	This function tests if a tag is present on this item.  It
	returns nonzero if true.  If NULL is passed as the tag, the
	item will return nonzero if it has no tags and zero if it
	does have tags.
*/
int
MleDwpItem::hasTag(const char *tag) const
{
	MLE_ASSERT(tag);
	
	// traverse the members of the list
	_MleDwpTag *entry = m_tagFirst;
	while ( entry )
	{
		// compare the entry tag
		if ( !strcmp(entry->tag,tag) )
			return 1;

		entry = entry->next;
	}

	return 0;
}


int
MleDwpItem::hasAnyTag(void) const
{
	return (m_tagFirst != NULL);
}


// returns the Nth tag; NULL if it doesn't exist
const char *
MleDwpItem::getNthTag(int n) const
{
    _MleDwpTag *entry = m_tagFirst;

    while (n-- > 0 && entry)
	entry = entry->next;

    return (entry ? entry->tag : NULL);
}


// notify registered listeners of item creation, modification, or
// deletion
void MleDwpItem::notify(MleDwpNotifierType notifyType, void *key)
{
    switch (notifyType) {

	case MleDwpCreateNotification:

	    // call the creation notification callbacks for MleDwpItem, since we
	    // know we're a WpItem subclass
	    m_notifier.notify((const void *)MleDwpItem::typeId, this, MleDwpCreateNotification);

	    m_notifier.notify(key == NULL ? (const void *)getTypeId() : key, this,
			    notifyType);
	    break;
	
	default:
	    m_notifier.notify(key == NULL ? this : key, this, notifyType);
	    break;
    }
}


/*
	This is a virtual function that reads the item-specific
	contents from a workprint.  It should read in what the virtual
	writeContents() writes out.  It is called from a higher level
	read function, and is generally not called by applications.
	This default implementation reads nothing, so any item that
	does not override the default will support only pure containership.
*/
int
MleDwpItem::readContents(MleDwpInput *)
{
	return 0;
}


/*
	This is a virtual function that writes the item-specific
	contents to a workprint.  It should write out what the virtual
	readContents() reads in.  It is called from a higher level
	write function, and is generally not called by applications.
	This default implementation writes nothing, so any item that
	does not override the default will support only pure containership.
*/
int MleDwpItem::writeContents(MleDwpOutput *)
{
	return 0;
}


/*
	This function is intended to be called by applications to
	write out an item.  It writes out the item the function was
	invoked on, writes the item-specific information by calling
	the virtual writeContents(), and then writes out the item
	children by calling write() recursively on them.
*/
int MleDwpItem::write(MleDwpOutput *out,int writeChildren)
{
	MLE_ASSERT(out);

	// accumulate return status
	int status = 0;

	/* write the opening paren and item type */
	/*   There is a little fudging of names here.  If the type name */
	/*   begins with MleDwp, it is considered to be a system name, and */
	/*   the prefix is stripped from the workprint item name. */
	status |= out->writeString("(");
	if (( ! strncmp(getTypeName(),"MleDwp",6) ) ||
		( ! strncmp(getTypeName(),"MleDpp",6) ))
		status |= out->writeString(getTypeName() + 6);
	else
		status |= out->writeString(getTypeName());

	// write out any tags
	_MleDwpTag *tag = m_tagFirst;
	while ( tag )
	{
		// write out the tag specifier (+) followed by the tag string
		status |= out->writeChar('+');
		status |= out->writeFancyString(tag->tag);

		tag = tag->next;
	}

	/* write the item contents */
	out->adjustIndent(1);
	status |= writeContents(out);

	if ( writeChildren )
	{
		/* write any children */
		MleDwpItem *child = getFirstChild();
		if ( child )
			status |= out->writeTerminal();
		while ( child )
		{
			status |= child->write(out);
			child = child->getNext();
		}
	}

	/* write the closing paren */
	out->adjustIndent(-1);
	status |= out->writeString(")");
	status |= out->writeTerminal();

	return status;
}


/*
	This static member function is called by applications to read
	from a workprint.  It returns a single item read in and optionally
	attaches it to a parent item.  To read all the items from a workprint,
	this function should be called on the input object until it returns
	NULL.

	Reading takes place by first reading the item type, then consulting
	the registry to see if the item class has been registered.  If it
	has, then the creation function registered with it is run to make
	an item instance.  After that the virtual readContents() is invoked
	to read any item-specific data, and then read() is recursively
	invoked to read in any children.

	If the class has not been registered, a special MleDwpUnknown item
	is instanced in its place.
*/
MleDwpItem *
MleDwpItem::read(MleDwpInput *in,MleDwpItem *parent)
{
	MLE_ASSERT(in);

	char c;
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	/* try to read a left paren */
	if ( in->readChar(&c) )
		return NULL;	// end of file

	/* verify this is a left paren */
	if ( c != '(' )
	{
		/* a right paren just means the end of children */
		if ( c != ')' )
			in->reportError("bad token: expected left paren");
		else
			in->putBackByte(')');
		return NULL;
	}

	/* read in item type */
	if ( in->readString(buffer) )
	{
		in->reportError("unexpected end of file");
		return NULL;
	}

#ifdef DEBUG
#if defined(__linux__)
#if defined(HAVE_LOG4CXX)
	LOG4CXX_DEBUG(g_dwpLogger, "DwpItem: reading " << buffer);
#else
	printf("DwpItem: reading %s.\n", buffer);
	fflush(stdout);
#endif /* HAVE_LOG4CXX */
#else
    printf("DwpItem: reading %s.\n", buffer);
    fflush(stdout);
#endif /* __linux__ */
#endif /* DEBUG */

	/* look up creation function in type registry */
	MleDwpItem *(*create)(void) =
		(MleDwpItem *(*)(void))typeRegistry.find(buffer);

	// if unsuccessful, try a DSO load
	if ( create == NULL && !g_loader.loadClass(buffer,"MleDwp") )
		create = (MleDwpItem *(*)(void))typeRegistry.find(buffer);

	/* create item */
	MleDwpItem *item;
	if ( create )
		item = (*create)();
	else
		item = new MleDwpUnknown(buffer);

	/* add to parent */
	if ( parent )
		parent->insertChild(item);

	// look for any tags
	while ( 1 )
	{
		if ( in->readChar(&c) )
		{
			in->reportError("unexpected end of file");
			return item;
		}

		// check for special tag marker
		if ( c == '+' )
		{
			if ( in->readString(buffer) )
			{
				in->reportError("couldn't read tag");
				return item;
			}

			item->addTag(buffer);
		}
		else
		{
			// not found, so put the character back and go on
			in->putBackByte(c);
			break;
		}
	}

	/* read item-specific information */
	if ( item->readContents(in) )
	{
		// An error has occurred in reading the contents of
		//   this node.  However, it may still be possible
		//   to continue reading the rest of the workprint,
		//   including any children of the node.  For now,
		//   do nothing and count on the item-specific read
		//   contents to report an error.
	}
	
	/* read children */
	while ( read(in,item) )
		;
	
	/* read terminal right paren */
	if ( in->readChar(&c) )
	{
		in->reportError("unexpected end of file");
		return item;
	}

	/* verify this is a right paren */
	if ( c != ')' )
	{
		in->reportError("bad token: expected right paren");

		// try to skip to end of the item
		in->skip();
		in->readChar(&c);	// skip puts the right paren back

		return item;
	}

	return item;
}


/*
	This function reads all the workprint items from an input
	object and returns a pointer to a single containing item.
	This is a special item, a _MleDwpTop, that does not
	write itself (only its children) into a workprint.
*/
MleDwpItem *
MleDwpItem::readAll(MleDwpInput *in)
{
	MLE_ASSERT(in);

	MleDwpItem *root = new _MleDwpTop;
	((_MleDwpTop *)root)->setName(in->getPath());
	while ( read(in,root) )
		;
	
	// check to make sure buffer is empty
	char c;
	if ( in->readChar(&c) == 0 && c == ')' )
		in->reportError("extra right paren at end of input (usually a previous item was terminated twice)");

// This code was inserted to behave like Inventor.  Unfortunately, this
//   causes problems when editing the structure because the existence of
//   the top transparent item is undetermined.
#ifdef NOTRANSPARENT
	// if there is zero or one item, don't bother with the root
	if ( root->first == root->last )
	{
		MleDwpItem *item = root->first;

		// remove the single item
		if ( item )
			root->removeChild(item);

		delete root;
		return item;
	}
	else
#endif
		return root;
}


/*
	This function reads all the workprint items from a file
	and returns a pointer to a single containing item.  This
	is a special item, a MleDwpContainer, that does not write
	itself (only its children) into a workprint.
*/
MleDwpItem *
MleDwpItem::readAll(const char *filename)
{
	MLE_ASSERT(filename);

	MleDwpInput in;
	if ( in.openFile(filename) )
		return NULL;
	else
		return readAll(&in);
}


/*
	This function registers a new class and creation function
	with the item system.
*/
void
MleDwpItem::registerType(const char *name,MleDwpItem *(*func)(void))
{
	MLE_ASSERT(name);
	MLE_ASSERT(func);

	/* If the name starts with MleDwp, this is a system name. */
	/* If the name starts with MleDpp, this is a system name. */
	if (( ! strncmp(name,"MleDwp",6) ) ||
		( ! strncmp(name,"MleDpp",6) ))
		name += 6;

	typeRegistry.set(name,(const void *)func);
}

void *
MleDwpItem::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpItem::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpItem::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpItem::operator delete[](void* p)
{
	mlFree(p);
}
