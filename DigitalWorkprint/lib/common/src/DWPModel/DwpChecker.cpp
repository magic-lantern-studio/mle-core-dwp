/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpChecker.cpp
 * @ingroup MleDWPModel
 *
 * Checks an in-memory workprint for problems
 *
 * @author Mark S. Millard
 * @date November 7, 2017
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Wizzer Works
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
#include "string.h"

// Include Digital Workprint header files.
#include "mle/mlTypes.h"
#include "mle/mlUnique.h"
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"


// Include Digital Workprint header files.
#include "mle/DwpItem.h"
#include "mle/DwpActor.h"
#include "mle/DwpActorDef.h"
#include "mle/DwpActorTemplate.h"
#include "mle/DwpRoleSetMapping.h"
//#include "mle/DwpDelegateForumMapping.h"
#include "mle/DwpRoleBinding.h"
//#include "mle/DwpDelegateBinding.h"
#include "mle/DwpProperty.h"
#include "mle/DwpHeaderFile.h"
#include "mle/DwpSourceFile.h"
#include "mle/DwpPropertyDef.h"
#include "mle/DwpGroup.h"
#include "mle/DwpScene.h"
#include "mle/DwpSet.h"
//#include "mle/DwpForum.h"
#include "mle/DwpSetDef.h"
//#include "mle/DwpForumDef.h"
#include "mle/DwpStage.h"
#include "mle/DwpFinder.h"

// Include DWP Checker header files.
#include "mle/DwpChecker.h"


MleDwpChecker::MleDwpChecker()
    :m_dwp(NULL),
     m_errorMsg(NULL),
     m_flags(MLE_DWP_CHECK_NOFLAGS)
{ 
    // Do nothing extra.
}


MleDwpChecker::MleDwpChecker(MleDwpItem *top)
    :m_dwp(top),
     m_errorMsg(NULL),
     m_flags(MLE_DWP_CHECK_NOFLAGS)
{ 
    // Do nothing extra.
}


MleDwpChecker::~MleDwpChecker()
{ 
    if (m_errorMsg)
	    mlFree(m_errorMsg);
}


MlErr MleDwpChecker::check()
{
    MLE_ASSERT(m_dwp);

    MlErr err;
    
	// Basic workprint semantics.
    if ((err = actorsInGroups()) != MlNoErr)
	    return err;
    if ((err = actorsHaveActorDefs()) != MlNoErr)
	    return err;
//  if ((err = actorForumTypeMatches()) != MlNoErr)
	if ((err = actorSetTypeMatches()) != MlNoErr)
        return err;
//  if ((err = stagesHaveAForum()) != MlNoErr)
	if ((err = stagesHaveASet()) != MlNoErr)
	    return err;
//  if ((err = forumsHaveForumDefs()) != MlNoErr)
	if ((err = setsHaveSetDefs()) != MlNoErr)
        return err;
    if ((err = hasScenes()) != MlNoErr)
	    return err;
    if ((err = uniqueGroupNames()) != MlNoErr)
        return err;

    // Mastering checks.
    if (MleDwpChecker_TargetIsSet(this))
	{
        ;
    }

    return MlNoErr;
}


// This rule checks that all actors are declared inside a group.

MlErr MleDwpChecker::actorsInGroups()
{
    MleDwpItem *problemActor = 
	    containershipTest(MleDwpActor::typeId, MleDwpGroup::typeId);

    if (problemActor != NULL)
    {
		char s[200];
		sprintf(s, "Actor '%s' is not inside any Group", 
			problemActor->getName());
		setErrorMsg(s);
		return 1;
    }

    return MlNoErr;
}


// This rule checks that all actors have a valid definition
// given an actor template, find its class name

const char *
MleDwpChecker::getActorClassFromTemplate(MleDwpActorTemplate *templt)
{
    if (! templt)
        return(NULL);

    // Get actor class/template name.
    const char *actorClass = templt->getActorClass();
    if (! actorClass)
        return(NULL);

    // Traverse upwards through any/all ancestor tempaltes.
    MleDwpActorTemplate *t = getTemplate(templt);
    while (t)
    {
        actorClass = t->getActorClass();
        t = getTemplate(t);
    }

    return(actorClass);
}


MleDwpActorTemplate * 
MleDwpChecker::getTemplate(MleDwpActorTemplate *templt)
{
    if (! templt)
        return(NULL);

    // Get actor class/template name.
    const char *actorClass = templt->getActorClass();
    if (! actorClass)
        return(NULL);

    // Look for actor template in DWP.
    MleDwpFinder  finder(MleDwpActorTemplate::typeId, actorClass);
    MleDwpActorTemplate *t = (MleDwpActorTemplate *)finder.find(m_dwp);

    return(t);
}


MlErr MleDwpChecker::actorsHaveActorDefs()
{
    unsigned int actorCount,actordefCount;
    MleDwpActor **actorList;
    MleDwpFinder actorFinder(MleDwpActor::typeId);
    MleDwpActorDef **actordefList;
    MleDwpFinder actordefFinder(MleDwpActorDef::typeId);
    MlBoolean actordefFound = TRUE;

    // Find Actor workprint items.
    actorFinder.setFindAll(1);
    actorFinder.find(m_dwp);
    
    actorCount = actorFinder.getNumItems();
    actorList = (MleDwpActor **) actorFinder.getItems();

    // Find ActorDef workprint items.
    actordefFinder.setFindAll(1);
    actordefFinder.find(m_dwp);

    actordefCount = actordefFinder.getNumItems();
    actordefList = (MleDwpActorDef **) actordefFinder.getItems();

    // For each Actor found ...
	int i;
    for (i = 0; i < (int)actorCount; i++)
    {
        const char *actorType = actorList[i]->getActorClass();

        // ... resolve ActorTemplate hierarchy ...
        MleDwpFinder actortemplateFinder(MleDwpActorTemplate::typeId, actorType);
        MleDwpActorTemplate *t = (MleDwpActorTemplate *)actortemplateFinder.find(m_dwp);
	if (t)
            actorType = getActorClassFromTemplate(t);

        // ... and check against each ActorDef found
        actordefFound = FALSE;
        for (int j= 0; j < (int)actordefCount; j++)
		{
            if (! strcmp(actorType,actordefList[j]->getName()))
			{
                actordefFound = TRUE;
                break;
            }
        }

        if (! actordefFound)
            break;
    }

    if (actordefFound == TRUE)
	{
        return MlNoErr;
    } else
	{
        char errorMessage[1024];

		sprintf(errorMessage, "Actor %s doesn't have a ActorDef.",
			actorList[i]->getName());
		setErrorMsg(errorMessage);
			return 1;
    }
}

#if 0
MlErr MleDwpChecker::actorForumTypeMatches(void)
{
    MlErr error = MlNoErr;
    MleDwpActor **actorList;
    unsigned int i, actorCount;
    MleDwpFinder *dbFinder;
    MleDwpDelegateBinding *binding;
    const char *forumName;
    MleDwpFinder *forumFinder;
    MleDwpForum *forum;
    MleDwpFinder *defFinder;
    MleDwpActorDef *actorDef;
    MleDwpFinder *mappingFinder;
    MleDwpDelegateForumMapping *mapping;

    MleDwpFinder actorFinder(MleDwpActor::typeId, NULL, 1);
    actorFinder.find(m_dwp);
    actorCount = actorFinder.getNumItems();

    if (actorCount > 0)
	{
		// Iterate and check each actor.
		actorList = (MleDwpActor **) actorFinder.getItems();
		for (i = 0; i < actorCount; i++)
		{
			// Find this actor's delegate binding.
			dbFinder = new MleDwpFinder(MleDwpDelegateBinding::typeId, NULL);
			binding = (MleDwpDelegateBinding *) dbFinder->find(actorList[i]);
			if (binding != NULL)
			{
				forumName = binding->getForum();

				// Find forum in workprint.
				forumFinder = new MleDwpFinder(MleDwpForum::typeId, forumName);
				forumFinder->find(m_dwp);
				if (forumFinder->getNumItems() != 1)
				{
					char errorBuffer[1024];

					sprintf(errorBuffer, "Actor %s doesn't have forum %s.",
						actorList[i]->getName(), forumName);
					setErrorMsg(errorBuffer);
					error = 1;
					break;
				}

				forum = (MleDwpForum *) forumFinder->getItem();
				MLE_ASSERT(forum != NULL);
			} else
			{
				// Not all actors define a delegate binding.
				delete dbFinder;
				continue;
			}

			// Find actor definition's delegate forum mapping, if any.
			defFinder = new MleDwpFinder(MleDwpActorDef::typeId,
						   actorList[i]->getActorClass());
			defFinder->find(m_dwp);
			if (defFinder->getNumItems() > 0)
			{
				actorDef = (MleDwpActorDef *) defFinder->getItem();
				MLE_ASSERT(actorDef != NULL);
			} else
			{
				// Must be defined from an actor template.
				const char * className;
				MleDwpFinder *atFinder;
				MleDwpActorTemplate *actorTemplate;

				className = actorList[i]->getActorClass();
				MLE_ASSERT(className);

				// Look for actor template by that name.
				atFinder = new MleDwpFinder(MleDwpActorTemplate::typeId, className);
				atFinder->find(m_dwp);
				actorTemplate = (MleDwpActorTemplate *) atFinder->getItem();
				delete atFinder;

				if (actorTemplate == NULL)
				{
					char errorBuffer[1024];

					sprintf(errorBuffer, "Actor %s doesn't have either an"
						"ActorDef or an ActorTemplate.");
					setErrorMsg(errorBuffer);
					error = 1;
					break;
				}
				MLE_ASSERT(actorTemplate->isa(MleDwpActorTemplate::typeId));

				// Search up containment for top level template.
				while (actorTemplate != NULL)
				{
					className = actorTemplate->getActorClass();
					MLE_ASSERT(className != NULL);

					// Look for actor template by that name.
					atFinder = new MleDwpFinder(MleDwpActorTemplate::typeId,
								  className);
					atFinder->find(m_dwp);
					actorTemplate = (MleDwpActorTemplate *) atFinder->getItem();
					delete atFinder;
				}

				// Top actorDef found, parent should be ActorDef
				MleDwpFinder adFinder(MleDwpActorDef::typeId, className);
				adFinder.find(m_dwp);

				if (adFinder.getNumItems() < 1 ||
					(actorDef = (MleDwpActorDef *) adFinder.getItem()) == NULL)
				{
					char errorBuffer[1024];

					sprintf(errorBuffer, "Can't find top actor definition for "
						"actor %s.  Encountered dead end at %s.",
						actorList[i]->getName(),
						className);
					setErrorMsg(errorBuffer);
					error = 1;
					break;
				}
			}

			// Find DelegateForumMapping in actor definition.
			mappingFinder = new MleDwpFinder(MleDwpDelegateForumMapping::typeId);
			mappingFinder->find(actorDef);
			if (mappingFinder->getNumItems() > 0)
			{
				// Found a delegate forum mapping, compare forum types.
				mapping = (MleDwpDelegateForumMapping *)
				mappingFinder->getItem();
				MLE_ASSERT(mapping != NULL);

				if (strcmp(forum->getType(), mapping->getForum()) != 0)
				{
					char errorBuffer[1024];

					sprintf(errorBuffer,
						"The DelegateBinding for Actor %s specifies "
						"forum %s of type %s,\nwhich doesn't match the "
						"forum type %s specified in the "
						"DelegateForumMapping in the ActorDef.",
						actorList[i]->getName(), forumName,
						forum->getType(), mapping->getForum());
					setErrorMsg(errorBuffer);
					error = 1;
					break;
				}
			}

			delete mappingFinder;
			delete defFinder;
			delete dbFinder;
		} // For each actor.
    }
    
    return error;
}
#else
MlErr MleDwpChecker::actorSetTypeMatches(void)
{
    MlErr error = MlNoErr;
    MleDwpActor **actorList;
    unsigned int i, actorCount;
    MleDwpFinder *dbFinder;
    MleDwpRoleBinding *binding;
    const char *setName;
    MleDwpFinder *setFinder;
    MleDwpSet *set;
    MleDwpFinder *defFinder;
    MleDwpActorDef *actorDef;
    MleDwpFinder *mappingFinder;
    MleDwpRoleSetMapping *mapping;

    MleDwpFinder actorFinder(MleDwpActor::typeId, NULL, 1);
    actorFinder.find(m_dwp);
    actorCount = actorFinder.getNumItems();

    if (actorCount > 0)
	{
		// Iterate and check each actor.
		actorList = (MleDwpActor **) actorFinder.getItems();
		for (i = 0; i < actorCount; i++)
		{
			// Find this actor's role binding.
			dbFinder = new MleDwpFinder(MleDwpRoleBinding::typeId, NULL);
			binding = (MleDwpRoleBinding *) dbFinder->find(actorList[i]);
			if (binding != NULL)
			{
				setName = binding->getSet();

				// Find set in workprint.
				setFinder = new MleDwpFinder(MleDwpSet::typeId, setName);
				setFinder->find(m_dwp);
				if (setFinder->getNumItems() != 1)
				{
					char errorBuffer[1024];

					sprintf(errorBuffer, "Actor %s doesn't have set %s.",
						actorList[i]->getName(), setName);
					setErrorMsg(errorBuffer);
					error = 1;
					break;
				}

				set = (MleDwpSet *) setFinder->getItem();
				MLE_ASSERT(set != NULL);
			} else
			{
				// Not all actors define a role binding.
				delete dbFinder;
				continue;
			}

			// Find actor definition's role set mapping, if any.
			defFinder = new MleDwpFinder(MleDwpActorDef::typeId,
						   actorList[i]->getActorClass());
			defFinder->find(m_dwp);
			if (defFinder->getNumItems() > 0)
			{
				actorDef = (MleDwpActorDef *) defFinder->getItem();
				MLE_ASSERT(actorDef != NULL);
			} else
			{
				// Must be defined from an actor template.
				const char * className;
				MleDwpFinder *atFinder;
				MleDwpActorTemplate *actorTemplate;

				className = actorList[i]->getActorClass();
				MLE_ASSERT(className);

				// Look for actor template by that name.
				atFinder = new MleDwpFinder(MleDwpActorTemplate::typeId, className);
				atFinder->find(m_dwp);
				actorTemplate = (MleDwpActorTemplate *) atFinder->getItem();
				delete atFinder;

				if (actorTemplate == NULL)
				{
					char errorBuffer[1024];

					sprintf(errorBuffer, "Actor %s doesn't have either an"
						"ActorDef or an ActorTemplate.");
					setErrorMsg(errorBuffer);
					error = 1;
					break;
				}
				MLE_ASSERT(actorTemplate->isa(MleDwpActorTemplate::typeId));

				// Search up containment for top level template.
				while (actorTemplate != NULL)
				{
					className = actorTemplate->getActorClass();
					MLE_ASSERT(className != NULL);

					// Look for actor template by that name.
					atFinder = new MleDwpFinder(MleDwpActorTemplate::typeId,
								  className);
					atFinder->find(m_dwp);
					actorTemplate = (MleDwpActorTemplate *) atFinder->getItem();
					delete atFinder;
				}

				// Top actorDef found, parent should be ActorDef
				MleDwpFinder adFinder(MleDwpActorDef::typeId, className);
				adFinder.find(m_dwp);

				if (adFinder.getNumItems() < 1 ||
					(actorDef = (MleDwpActorDef *) adFinder.getItem()) == NULL)
				{
					char errorBuffer[1024];

					sprintf(errorBuffer, "Can't find top actor definition for "
						"actor %s.  Encountered dead end at %s.",
						actorList[i]->getName(),
						className);
					setErrorMsg(errorBuffer);
					error = 1;
					break;
				}
			}

			// Find RoleSetMapping in actor definition.
			mappingFinder = new MleDwpFinder(MleDwpRoleSetMapping::typeId);
			mappingFinder->find(actorDef);
			if (mappingFinder->getNumItems() > 0)
			{
				// Found a role set mapping, compare forum types.
				mapping = (MleDwpRoleSetMapping *)
				mappingFinder->getItem();
				MLE_ASSERT(mapping != NULL);

				if (strcmp(set->getType(), mapping->getSet()) != 0)
				{
					char errorBuffer[1024];

					sprintf(errorBuffer,
						"The RoleBinding for Actor %s specifies "
						"set %s of type %s,\nwhich doesn't match the "
						"set type %s specified in the "
						"RoleSetMapping in the ActorDef.",
						actorList[i]->getName(), setName,
						set->getType(), mapping->getSet());
					setErrorMsg(errorBuffer);
					error = 1;
					break;
				}
			}

			delete mappingFinder;
			delete defFinder;
			delete dbFinder;
		} // For each actor.
    }
    
    return error;
}
#endif /* 0 */


#if 0
MlErr MleDwpChecker::oneForumExists(MleDwpStage *stage)
{
    MleDwpFinder f(MleDwpForum::typeId, NULL);
    MleDwpForum *forum = (MleDwpForum*) f.find(stage);
    if (! forum)
    {
        char	errorMessage[1024];

		sprintf(errorMessage, "Stage %s doesn't have at least one forum.",
			stage->getName());
		setErrorMsg(errorMessage);
		return 1;
    }
    
    return MlNoErr;
}
#else
MlErr MleDwpChecker::oneSetExists(MleDwpStage *stage)
{
    MleDwpFinder f(MleDwpSet::typeId, NULL);
    MleDwpSet *set = (MleDwpSet *) f.find(stage);
    if (! set)
    {
        char	errorMessage[1024];

		sprintf(errorMessage, "Stage %s doesn't have at least one set.",
			stage->getName());
		setErrorMsg(errorMessage);
		return 1;
    }
    
    return MlNoErr;
}
#endif /* 0 */


#if 0
MlErr MleDwpChecker::stagesHaveAForum(void)
{
    unsigned int	stageCount, i;
    MleDwpStage **	stageList;

    MleDwpFinder f(MleDwpStage::typeId, NULL);
    f.find(m_dwp);
    
    stageCount = f.getNumItems();
    stageList = (MleDwpStage **) f.getItems();

    for (i = 0; i < stageCount; i++)
	{
        if (oneForumExists(stageList[i]) != MlNoErr)
		{
			return 1;
		}
    }
    
    return MlNoErr;
}
#else
MlErr MleDwpChecker::stagesHaveASet(void)
{
    unsigned int	stageCount, i;
    MleDwpStage **	stageList;

    MleDwpFinder f(MleDwpStage::typeId, NULL);
    f.find(m_dwp);
    
    stageCount = f.getNumItems();
    stageList = (MleDwpStage **) f.getItems();

    for (i = 0; i < stageCount; i++)
	{
        if (oneSetExists(stageList[i]) != MlNoErr)
		{
			return 1;
		}
    }
    
    return MlNoErr;
}
#endif /* 0 */


// This rule checks that all sets have a valid definition.

#if 0
MlErr MleDwpChecker::forumsHaveForumDefs()
{
    unsigned int forumCount,forumdefCount;
    MleDwpForum **forumList;
    MleDwpFinder forumFinder(MleDwpForum::typeId);
    MleDwpForumDef **forumdefList;
    MleDwpFinder forumdefFinder(MleDwpForumDef::typeId);
    MlBoolean forumdefFound;

    // Find Forum workprint items.
    forumFinder.setFindAll(1);
    forumFinder.find(m_dwp);
    
    forumCount = forumFinder.getNumItems();
    forumList = (MleDwpForum **) forumFinder.getItems();

    // Find ForumDef workprint items.
    forumdefFinder.setFindAll(1);
    forumdefFinder.find(m_dwp);

    forumdefCount = forumdefFinder.getNumItems();
    forumdefList = (MleDwpForumDef **) forumdefFinder.getItems();

    // Check each Forum found against each ForumDef found.
	int i;
    for (i = 0; i < (int)forumCount; i++)
    {
        const char *forumType = forumList[i]->getType();

        forumdefFound = FALSE;
        for (int j= 0; j < (int)forumdefCount; j++)
		{
            if (! strcmp(forumType,forumdefList[j]->getName()))
			{
                forumdefFound = TRUE;
                break;
            }
        }

        if (! forumdefFound)
            break;
    }

    if (forumdefFound == TRUE)
	{
        return MlNoErr;
    } else {
        char errorMessage[1024];

		sprintf(errorMessage, "Forum %s doesn't have a ForumDef.",
			forumList[i]->getName());
		setErrorMsg(errorMessage);
        return 1;
    }
}
#else
MlErr MleDwpChecker::setsHaveSetDefs()
{
    unsigned int setCount,setdefCount;
    MleDwpSet **setList;
    MleDwpFinder setFinder(MleDwpSet::typeId);
    MleDwpSetDef **setdefList;
    MleDwpFinder setdefFinder(MleDwpSetDef::typeId);
    MlBoolean setdefFound = FALSE;

    // Find Set workprint items.
    setFinder.setFindAll(1);
    setFinder.find(m_dwp);
    
    setCount = setFinder.getNumItems();
    setList = (MleDwpSet **) setFinder.getItems();

    // Find SetDef workprint items.
    setdefFinder.setFindAll(1);
    setdefFinder.find(m_dwp);

    setdefCount = setdefFinder.getNumItems();
    setdefList = (MleDwpSetDef **) setdefFinder.getItems();

    // Check each Set found against each SetDef found.
	int i;
    for (i = 0; i < (int)setCount; i++)
    {
        const char *setType = setList[i]->getType();

        setdefFound = FALSE;
        for (int j= 0; j < (int)setdefCount; j++)
		{
            if (! strcmp(setType,setdefList[j]->getName()))
			{
                setdefFound = TRUE;
                break;
            }
        }

        if (! setdefFound)
            break;
    }

    if (setdefFound == TRUE)
	{
        return MlNoErr;
    } else {
        char errorMessage[1024];

		if (setList == NULL)
		{
			sprintf(errorMessage, "SetDef or Set not found.");
		} else {
		    sprintf(errorMessage, "Set %s doesn't have a SetDef.",
			        setList[i]->getName());
		}
		setErrorMsg(errorMessage);
        return 1;
    }
}
#endif /* 0 */

MlErr MleDwpChecker::hasScenes()
{
    return MlNoErr;
}


// This rule checks that all group names are unique.

static int _groupNameCompare( const void *s1, const void *s2 )
{
    return strcmp((*(MleDwpGroup **)s1)->getName(),(*(MleDwpGroup **)s2)->getName());
}


MlErr MleDwpChecker::uniqueGroupNames()
{
    unsigned int groupCount,i;
    MleDwpGroup **groupList;

    MleDwpFinder f(MleDwpGroup::typeId, NULL);
    f.setFindAll(1);
    f.find(m_dwp);
    
    groupCount = f.getNumItems();
    groupList = (MleDwpGroup **) f.getItems();

    i = mlUnique((void *)groupList,groupCount,sizeof(MleDwpGroup *),
             _groupNameCompare);
    if (i != groupCount)
	{
        char errorMessage[1024];

		sprintf(errorMessage,
					"%s\n\t%s\n",
					"Workprint contains duplicate group entries:",
					"Group names must be unique.");
		setErrorMsg(errorMessage);
			return 1;
    }

    return MlNoErr;
}


//////////////////////////////////////////////////////////////////////
//////////////////   helper test routines      ///////////////////////
//////////////////////////////////////////////////////////////////////

//
// Tests that all objects of one type are contained within objects of
// another type (e.g., groups within scenes).
//
// returns first problem child found
//

MleDwpItem * MleDwpChecker::containershipTest(MleDwpType childType, 
    MleDwpType ancestorType)
{
    MleDwpFinder f(childType, NULL, 1);
    f.find(m_dwp);
    for (int i = 0; i < f.getNumItems(); i++)
    {
		MleDwpItem *ancestor = f.getItems()[i];
		while ((ancestor = ancestor->getParent()) != NULL)
		{
			if (ancestor->isa(ancestorType))
			break;
		}
		
		// If we didn't find ancestor of right type, ancestor will be NULL.
		if (ancestor == NULL)
			return f.getItems()[i];
    }

    // No problems found.
    return NULL;
}


void MleDwpChecker :: setErrorMsg(const char *s)
{
    if (m_errorMsg)
	    mlFree(m_errorMsg);

    m_errorMsg = strdup(s);
}

//  Check if this workprint item is legal only at the top level

MlBoolean MleDwpChecker::isTopLevelOnlyWpItem(MleDwpItem *item)
{
    if ( item->isa( MleDwpGroup::typeId )) return FALSE;
    //if ( item->isa( MleDwpForum::typeId )) return FALSE;
	if ( item->isa( MleDwpSet::typeId )) return FALSE;
    if ( item->isa( MleDwpActor::typeId )) return FALSE;
    if ( item->isa( MleDwpProperty::typeId )) return FALSE;
    if ( item->isa( MleDwpPropertyDef::typeId )) return FALSE;
    // XXX allow adding these at the top level
    //if ( item->isa( MleDwpHeaderFile::typeId )) return FALSE;
    //if ( item->isa( MleDwpSourceFile::typeId )) return FALSE;
	
    return TRUE;
}


unsigned int MleDwpChecker::getFlags(void)
{
    return(m_flags);
}


void MleDwpChecker::setFlags(unsigned int value,unsigned int mask)
{
    m_flags |= (value & mask);
}


void MleDwpChecker::clearFlags(unsigned int mask)
{
    m_flags &= ~mask;
}

void *
MleDwpChecker::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpChecker::operator delete(void *p)
{
	mlFree(p);
}
