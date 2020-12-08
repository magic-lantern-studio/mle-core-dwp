/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpGroup.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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


#ifndef __MLE_DWP_GROUP_H_
#define __MLE_DWP_GROUP_H_

//Include Digital Workprint header files.
#include "mle/DwpItem.h"

/**
 * This is the container that defines the minimum unit of actors to
 * be loaded at run time.  The analogous run time object is MlGroup.
 *
 * WORKPRINT SYNTAX:
 * 
 * EXAMPLE
 * Syntax: (Group <name> <type>), where
 * 
 * <name> is the name of the group (char *), and where
 * <type> is the run time class type of the group (char *) 
 * 
 * Contained by: MleDwpScene
 * 
 * Contains: MleDwpActor, MleDwpDelegateAttachment, MleDwpHeaderFile	
 * END
 * 
 * @see MleDwpScene, MleDwpGroupRef, MleDwpActor, MleDwpItem, MleGroup
 */
class MLE_DWP_API MleDwpGroup : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpGroup);

  public:

	/**
	 * A constructor that specifies the class name.
	 *
	 * @param className A reasonable default value for the runtime class type of the group.
	 * Any subclass should override this.
	 */
	MleDwpGroup(char *className = const_cast<char*>("MleGroup"));

	/**
	 * The destructor.
	 */
	virtual ~MleDwpGroup();

    /**
	 * Set the runtime class type of the group.
	 */
	void setGroupClass(const char *);

    /**
	 * Determine the runtime class type of the group.
	 */
	const char *getGroupClass(void) const;

	/**
	 * Callback that returns true on MleDwpGroup or MleDwpGroupRef.
	 */
	static int groupOrGroupRefFinderCB( MleDwpItem *item, void* );

	/**
	 * Writes a group to the digital workprint.
	 */
	virtual int write(MleDwpOutput *out,int writeChildren = 1);

    // Local enumeration for catching player errors in rehearsal mode.
	enum PlayerError
	{
	    DWP_NO_ERROR, 
	    DWP_GROUP_NOT_LOADED
	};

	/**
	 * Get player error.  Used by tools to keep track of any
	 * problems that have occurred with this group in the
	 * player.
	 */
	PlayerError getPlayerError() { return m_playerError; }

	/**
	 * Set pruntime player error.
	 */
	void setPlayerError(PlayerError err) { m_playerError = err; }

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

	char *m_groupClass;

	enum PlayerError m_playerError;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	//virtual int writeContents(MleDppActorGroupOutput *out);

	//virtual int write(MleDppActorGroupOutput *out);
};

#endif /* __MLE_DWP_GROUP_H_ */
