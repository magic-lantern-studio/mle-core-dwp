/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API */

/**
 * @file DwpTblMgr.h
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

#ifndef __MLE_DWP_TBLMGR_H_
#define __MLE_DWP_TBLMGR_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Digial Workprint heaeder files.
#include "mle/DwpTagAllDiscriminator.h"


class MleDwpItem;

// Support for building tables (i.e. strings, structs, ...).
typedef struct 
{
    int used;
    int allocated;
    void **items;
} TblMgrItems;

typedef struct {
    char *name;
    int numProperties;
    char *headerFile;
} ActorClassTableItem;

typedef struct {
    char *name;
    char *type;
} ForumTableItem;

typedef struct {
    char *name;
    int numProperties;
    char *headerFile;
} ForumClassTableItem;

typedef struct {
    char *name;
    char *headerFile;
} GroupClassTableItem;

typedef struct {
    char *name;
} GroupInstanceTableItem;

typedef struct {
    char *name;
    char *headerFile;
} SceneClassTableItem;


class MleDwpTblMgr
{
  // Declare member variables.
  protected:

    TblMgrItems m_actorClassTable;
    TblMgrItems m_delegateClassTable;
    TblMgrItems m_forumClassTable;
    TblMgrItems m_actorPropTable;
    TblMgrItems m_forumPropTable;
    TblMgrItems m_forumTable;
    TblMgrItems m_groupClassTable;
    TblMgrItems m_groupInstanceTable;
    TblMgrItems m_sceneClassTable;
    TblMgrItems m_mediarefTable;
    
  private:

    MleDwpTagAllDiscriminator *m_discriminator;
    
   // Declare member functions.
  public:

    MleDwpTblMgr(void);

    virtual ~MleDwpTblMgr(void);
    
    MlBoolean setDiscriminators(MleDwpItem *tree, char *tags);

    MlBoolean buildIndexTables(MleDwpItem *root);

    TblMgrItems *getActorClassTable(void) { return(&m_actorClassTable); };

    TblMgrItems *getDelegateClassTable(void) { return(&m_delegateClassTable); };

    TblMgrItems *getForumClassTable(void) { return(&m_forumClassTable); };

    TblMgrItems *getActorPropTable(void) { return(&m_actorPropTable); };

    TblMgrItems *getForumPropTable(void) { return(&m_forumPropTable); };

    TblMgrItems *getForumTable(void) { return(&m_forumTable); };

    TblMgrItems *getGroupClassTable(void) { return(&m_groupClassTable); };

    TblMgrItems *getGroupInstanceTable(void) { return(&m_groupInstanceTable); };

    TblMgrItems *getSceneClassTable(void) { return(&m_sceneClassTable); };

    TblMgrItems *getMediaRefTable(void) { return(&m_mediarefTable); };

    int getActorClassTableSize(void) { return(m_actorClassTable.used); };

    int getDelegateClassTableSize(void) { return(m_delegateClassTable.used); };

    int getForumClassTableSize(void) { return(m_forumClassTable.used); }

    int getActorPropTableSize(void) { return(m_actorPropTable.used); };

    int getForumPropTableSize(void) { return(m_forumPropTable.used); };

    int getForumTableSize(void) { return(m_forumTable.used); };

    int getGroupClassTableSize(void) { return(m_groupClassTable.used); };

    int getGroupInstanceTableSize(void) { return(m_groupInstanceTable.used); };

    int getSceneClassTableSize(void) { return(m_sceneClassTable.used); };

    int getMediaRefTableSize(void) { return(m_mediarefTable.used); };
    

    int actorClassRuntimeName(const char *actorClass);

    int delegateClassRuntimeName(const char *delegateClass);

    int forumClassRuntimeName(const char *forumClass);

    int actorPropRuntimeName(const char *actorClass,const char *propertyName);

    int forumPropRuntimeName(const char *forumClass,const char *propertyName);

    int forumRuntimeName(const char *forumName);

    int groupClassRuntimeName(const char *groupClass);

    int groupInstanceRuntimeName(const char *groupInstance);

    int sceneClassRuntimeName(const char *sceneClass);

    int mediarefClassRuntimeName(const char *mediarefClass);
    
  private:

    void saveString(const char *pstr,TblMgrItems *strTable);

    void saveActorClass(char *name,int numProperties,char *headerFile,
			TblMgrItems *actorTable);

    void saveForumClass(char *name,int numProperties,char *headerFile,
			TblMgrItems *forumTable);

    void saveForum(char *name,char *type,TblMgrItems *forumTable);

    void saveGroupClass(char *name,char *headerFile,TblMgrItems *groupTable);

    void saveGroupInstance(char *name,TblMgrItems *groupTable);

    void saveSceneClass(char *name,char *headerFile,TblMgrItems *sceneTable);

    int mapStringToIndex(const char *name,TblMgrItems *strTable);

    int mapActorClassToIndex(const char *name,TblMgrItems *actorTable);

    int mapForumClassToIndex(const char *name,TblMgrItems *forumTable);

    int mapForumToIndex(const char *name,TblMgrItems *forumTable);

    int mapGroupClassToIndex(const char *name,TblMgrItems *groupTable);

    int mapGroupInstanceToIndex(const char *name,TblMgrItems *groupTable);

    int mapSceneClassToIndex(const char *name,TblMgrItems *sceneTable);
};


#define MLE_DWP_BADTBL_INDEX -1


#endif /* __MLE_DWP_TBLMGR_H_ */
