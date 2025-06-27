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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


#ifndef __MLE_DWP_ACTOR_H_
#define __MLE_DWP_ACTOR_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This defines an actor instance.  The actor class may either refer to an
 * actual class name or to an ActorTemplate item.  Actor names should be unique
 * within their group.
 *
 * WORKPRINT SYNTAX
 *
 * EXAMPLE
 * Syntax:	(Actor <name> <type>)
 *
 * <name> ::= actor name (char *)
 * <type> ::= actor class name (char *)
 *
 * Contained by: MleDwpGroup
 *
 * Contains: MleDwpProperty, MleDwpDelegateBinding
 *
 * END
 *
 * @see MleDwpActorDef, MleDwpActorTemplate
 */
class MLE_DWP_API MleDwpActor : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpActor);

  public:

	/**
	 * The default constructor.
	 */
	MleDwpActor(void);

	/**
	 * The destructor.
	  */
	virtual ~MleDwpActor(void);

	/**
	 * Sets the actor type name.  This is either the
	 * name of a C++ class or the name of an ActorTemplate item.
	 * If it is the name of a C++ class, there is usually also an
	 * ActorDef of the same name in the workprint.
	 */
	void setActorClass(const char *);

    /**
	 * Returns the actor type name.  NULL is returned
	 * if no actor type has been set.
	 */
	const char *getActorClass(void) const;

	//virtual int write(MleDppActorGroupOutput *out);

	enum PlayerError
	{
	    DWP_NO_ERROR, 
	    DWP_ACTOR_NOT_LOADED
	};

    /**
	 * Get player error.
	 */
	PlayerError getPlayerError()
	{ return m_playerError; }

    /**
	 * Set player error.
	 */
	void setPlayerError(PlayerError err)
	{ m_playerError = err; }
	
	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
	 * Override operator new array.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new[](size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void operator delete(void *p);

	/**
	 * Override operator delete array.
	 *
	 * @param p A pointer to the memory to delete.
	 */
	void operator delete[](void* p);

  protected:

	char *m_actorClass;
	enum PlayerError m_playerError;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	//virtual int writeContents(MleDppActorGroupOutput *out);
};

#endif /* __MLE_DWP_ACTOR_H_ */
