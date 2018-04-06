/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpScene.h
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


#ifndef __MLE_DWP_SCENE_H_
#define __MLE_DWP_SCENE_H_

// Include Digital Workprint header files.
#include "mle/DwpItem.h"
//#include "mle/DppSceneOutput.h"

/**
 * This is the container that defines the maximum unit of actors to
 * be loaded at run time.  A scene contains a number of groups, all
 * of which are loaded when the scene is loaded at run time.  The
 * analogous run time object is MlScene.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 *  Syntax: (Scene <name> <type>), where
 * 
 * <name> is the name of the scene (char *), and where
 * <type> is the run time class type of the scene (char *) 
 * 
 * Contained by: none
 * 
 * Contains: MleDwpGroup, MleDwpGroupRef, MleDwpHeaderFile
 * END
 * 
 * @see MleDwpForum, MleDwpItem, FwScene
 */
class MLE_DWP_API MleDwpScene : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpScene);

  public:

	/**
	 * A constructor that specifies the class name.
	 *
	 * @param className A reasonable default value for the runtime class type of the scene.
	 * Any subclass should override this.
	 */
	MleDwpScene(char *className = "MleScene");

	/**
	 * The destructor.
	 */
	virtual ~MleDwpScene(void);

    /**
	 * Set the runtime class type of the scene.
	 */
	void setSceneClass(const char *);

    /**
	 * Determine the runtime class type of the scene.
	 */
	const char *getSceneClass(void) const;

	/**
	 * Writes a scene to workprint.
	 */
	virtual int write(MleDwpOutput *out,int writeChildren = 1);

    // Local enumeration for catching player errors in rehearsal mode.
	enum PlayerError
	{
	    DWP_NO_ERROR, 
	    DWP_SCENE_NOT_LOADED
	};

	/**
	 * Get player error.  Used by tools to keep track of any
	 * problems that have occurred with this scene in the
	 * player.
	 */
	PlayerError getPlayerError() { return m_playerError; }
    /**
	 * Set runtime player error.
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

	char *m_sceneClass;
	enum PlayerError m_playerError;

	virtual int readContents(MleDwpInput *in);

	virtual int writeContents(MleDwpOutput *out);

	//virtual int writeContents(MleDppSceneOutput *out);

	//virtual int write(MleDppSceneOutput *out);
};

#endif /* __MLE_DWP_SCENE_H_ */
