/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpScene.pp
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
#include <string.h>
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Digital Workprit header files.
#include "mle/DwpFinder.h"
#include "mle/DwpGroup.h"
#include "mle/DwpGroupRef.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
#include "mle/DwpScene.h"
//#include "mle/DppSceneOutput.h"

MLE_DWP_SOURCE(MleDwpScene,MleDwpItem);


MleDwpScene::MleDwpScene(char *sc)
{
	// Default value of sc is non-null, so constructor must have valid sc.
	MLE_ASSERT(NULL != sc);
	m_sceneClass = strdup(sc);

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpScene::~MleDwpScene(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key)
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	if ( m_sceneClass )
		mlFree(m_sceneClass);
}

void
MleDwpScene::setSceneClass(const char *sc)
{
	/* get rid of the old scene class name. */
	if ( m_sceneClass )
		mlFree(m_sceneClass);
	
	/* Set the new scene class. */
	m_sceneClass = sc ? strdup(sc) : NULL;
}

const char *
MleDwpScene::getSceneClass(void) const
{
	return m_sceneClass;
}

int
MleDwpScene::readContents(MleDwpInput *in)
{
    char buffer[MLE_DWP_INPUT_BUFSIZE];

    if ( in->readString(buffer) )
    {
        in->reportError("error reading Scene name");
        in->skip();
        return 1;
    }
    setName(buffer);

    if ( in->readString(buffer) )
    {
	    in->reportError("error reading Scene class");
	    in->skip();
	    return 1;
    }
    setSceneClass(buffer);

    return 0;
}

int
MleDwpScene :: writeContents(MleDwpOutput *out)
{
    out->writeString(getName());
    out->writeString(getSceneClass());
    return 0;
}

#if 0
int
MleDwpScene::writeContents(MleDppSceneOutput *out)
{
	// find the scene class
	const char *className = getSceneClass();

	int sceneClassNumber = out->table->sceneClassRuntimeName(className); 
	out->writeInt(sceneClassNumber);
	out->currentSceneClass = className;

	return 0;
}
#endif /* 0 */

int
MleDwpScene::write(MleDwpOutput *out,int writeChildren)
{
    return(MleDwpItem::write(out, writeChildren));
}

void *
MleDwpScene::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpScene::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpScene::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
MleDwpScene::operator delete[](void* p)
{
    mlFree(p);
}

#if 0
int
MleDwpScene::write(MleDpSceneOutput *out)
{
    int i;	// loop variables

    // open output file
    char sceneChunkFile[FILENAME_MAX];
    sprintf(sceneChunkFile, "%s.chk", getName());
    out->openFile(sceneChunkFile);

    // find all the groups and group references in this scene
    MleDwpFinder groupFinder;
    groupFinder.setCallback(&MleDwpGroup::groupOrGroupRefFinderCB, NULL);
    groupFinder.setFindAll(1);
    groupFinder.find(this);
    const int numGroups = groupFinder.getNumItems();
    MleDwpItem **group = (MleDwpItem **)groupFinder.getItems();

    // Write out the index of this scene\'s class in the runtime
    // table of constructors.
    MleDwpItem *root = this;
    while ( root->getParent() )
	    root = root->getParent();

    FwTblMgr tbl;
    FW_ASSERT(tbl.buildIndexTables(root) == TRUE);
    int index = tbl.sceneClassRuntimeName( this->getSceneClass() );
    out->writeInt(index);

    // write out the number of groups in the scene to the chunk
    out->writeInt(numGroups);

    // output each group
    //   Groups are written out in the same order they were created.
    for ( i = 0; i < numGroups; i++ ) {

	    // If this item is a groupRef, then we ask for its name,
	    // which is the unique name of a group instance.  Then we
	    // look up the group instance in the TblMgr and get the 
	    // group\'s index from there.
	    // It\'s pretty handy that if this item is a group, then
	    // it\'s name is the same as that of a valid group ref,
	    // so we can use the same code for both types of items.
	    int index = tbl.groupInstanceRuntimeName(group[i]->getName());
	    
	    // Write the group\'s global index to the chunk.
	    out->writeInt(index); 

    }

    // The scene\'s part of the sceneid.h file.
    // We don\'t know our number, so instead count on the sceneCount
    // member to hold it for us, and on out to traverse the set of 
    // scenes in order. XXX
    if (out->sceneid) {
	    fprintf(out->sceneid,"\n/*\n * Global scene index. \n */\n");
	    fprintf(out->sceneid, "#define FwPPSceneID_%s %d\n", 
		    getName(), out->sceneCount);

	    // The group\'s relative index in the scene.
	    fprintf(out->sceneid,"\n/*\n * Group indices within the scene. \n */\n");
	    for ( i = 0; i < numGroups; i++ ) {
		    fprintf(out->sceneid,"#define FwPPSceneGroupID_%s_%s %d\n",
			    getName(), group[i]->getName(), i);
	    }
    }
    out->sceneCount++;

    if (group) {
	    delete group;
    }
    return 0;
}
#endif /* 0 */

