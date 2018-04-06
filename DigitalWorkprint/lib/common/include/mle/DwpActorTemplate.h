/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpActorTemplate.h
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


#ifndef __MLE_DWP_ACTORTEMPLATE_H_
#define __MLE_DWP_ACTORTEMPLATE_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This defines an actor template, which can be referenced (by an MleDwpActor
 * item) in place of an actor class.  The template itself must refer to
 * an actor class.  The difference between an actor template and an actor
 * class is that the template is a model for an actor item.  That is, it
 * contains default subitems for property values and/or delegate binding.
 * These values may be overridden in an actor that references the template.
 *
 * WORKPRINT SYNTAX
 *
 * EXAMPLE
 * Syntax:		(ActorTemplate <name> <type>)
 *
 * <name>	actor name (char *)
 * <type>	actor class name (char *)
 *
 * Contained by: none
 *
 * Contains: MleDwpProperty, MleDwpDelegateBinding
 *
 * END
 *
 * MleDwpActor, MleDwpActorDef
 *
 * BUGS
 * It should be possible for an actor template to reference another actor
 * template as long as the chain ends at an actor def.  Code in the player
 * doesn't quite support this yet.
 *
 * Detection of overriding subitems is done when an actor subitem has the
 * same type and name as an actor template subitem.  This process can be
 * confused by subhierarchies, so it is best not to "conceal" subitems
 * of a template by putting them below other subitems.
 */
class MLE_DWP_API MleDwpActorTemplate : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpActorTemplate);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpActorTemplate(void);

	/**
	 * The destructor.
	 */
	virtual ~MleDwpActorTemplate();

	/**
	 * Sets the name of the actor class this template
	 * references.
	 */
	void setActorClass(const char *);

	/**
	 * Retrieves the name of the actor class this template
	 * references.  NULL is returned if no class has been set.
	 */
	const char *getActorClass(void) const;

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

  protected:

	char *m_actorClass;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);
};

#endif /* __MLE_DWP_ACTORTEMPLATE_H_ */
