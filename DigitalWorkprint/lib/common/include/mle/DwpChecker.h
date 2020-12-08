/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpChecker.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Wizzer Works
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


#ifndef __MLE_DWP_CHECKER_H_
#define __MLE_DWP_CHECKER_H_


// Include Magic Lantern header files.
#include "mle/mlErrno.h"

// Include Digital Workprint header files.
#include "mle/DwpItem.h"

// Forward declarations.
class MleDwpStage;
class MleDwpActorTemplate;


/**
 * This class checks an in-memory workprint for problems.
 */
class MLE_DWP_API MleDwpChecker
{
  public:

	/**
	 * The default constructor.
	 */
	MleDwpChecker(void);

	/**
	 * A constructor that specifies the root of the Digital
	 * Workprint that will be analyzed.
	 *
	 * @param top A pointer to the root of the Digital
	 * Workprint.
	 */
    MleDwpChecker(MleDwpItem *top);

	/**
	 * The destructor.
	 */
    virtual ~MleDwpChecker(void);

	/**
	 * Set the root of the Digital Workprint that will
	 * be analyzed for errors.
	 *
	 * @param top A pointer to the root of the Digital
	 * Workprint.
	 */
    void setWorkprint(MleDwpItem *top)
	{ m_dwp = top; }

	/**
	 * Analyze the Digital Workprint for problems.
	 *
	 * @return An error code is returned.
	 */
    virtual MlErr check(void);

    char * getErrorMsg(void)
	{ return m_errorMsg; }

    static MlBoolean isTopLevelOnlyWpItem(MleDwpItem *item);

    /**
	 * Get conditional flags.
	 *
	 * @return An unsigned integer containing the flag bits is returned.
	 */
    virtual unsigned int getFlags(void);

    /**
	 * Set conditional flags.
	 *
	 * @param value The value to set the flags to.
	 * @param mask The bits in the flag to set.
	 */
    virtual void setFlags(unsigned int value,unsigned int mask);

    /**
	 * Clear conditional flags.
	 *
	 * @param mask The bits in the flag to clear.
	 */
    virtual void clearFlags(unsigned int mask);

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

    virtual MlErr actorsInGroups(void);

    virtual MlErr actorsHaveActorDefs(void);

    //virtual MlErr actorForumTypeMatches(void);
	virtual MlErr actorSetTypeMatches(void);

    //virtual MlErr oneForumExists(MleDwpStage *stage);
	virtual MlErr oneSetExists(MleDwpStage *stage);

	//virtual MlErr stagesHaveAForum(void);
    virtual MlErr stagesHaveASet(void);

    //virtual MlErr forumsHaveForumDefs(void);
	virtual MlErr setsHaveSetDefs(void);

    virtual MlErr hasScenes(void);

    virtual MlErr uniqueGroupNames(void);

    /**
	 * Tests that all objects of one type are contained within objects of
     * another type (e.g., groups within scenes).
	 *
	 * @param The child's Digital Workprint type to test for.
	 * @param The ancestor's Digital Workprint type to test against.
	 *
     * @return The first problem child found is returned.
     */
    MleDwpItem * containershipTest(MleDwpType childType, 
				 MleDwpType ancestorType);

    virtual void setErrorMsg(const char *s);

  private:

    MleDwpItem *m_dwp;
    char * m_errorMsg;
    int m_flags;

    const char *getActorClassFromTemplate(MleDwpActorTemplate *templt);

    MleDwpActorTemplate *getTemplate(MleDwpActorTemplate *templt);
};


/**
 * Macro constant.
 * <p>
 * Defines valid values for the m_flags member variable,
 * </p>
 */
#define MLE_DWP_CHECK_NOFLAGS 0x00000000


/**
 * Macro constant.
 * <p>
 * Reserved flag for tools and rehearsal player,
 * </p>
 */
#define MLE_DWP_CHECK_TOOLSMASK 0x0000ffff


/**
 * Macro constant
 * <p>
 * Reserved flag for mastering targets.
 * </p>
 */
#define MLE_DWP_CHECK_TARGETMASK 0x00ff0000


/**
 * Macro constant.
 * <p>
 * Reserved flag for mastering targets.
 * </p>
 */
#define MLE_DWP_CHECK_TARGET_SGI 0x00010000


/**
 * Macro constant
 * <p>
 * Reserved flag for mastering targets
 * </p>
 */
#define MLE_DWP_CHECK_TARGET_PC 0x00020000

 /**
 * Macro constant
 * <p>
 * Reserved flag for mastering targets
 * </p>
 */
#define MLE_DWP_CHECK_TARGET_LINUX 0x00040000


/**
 * Macro function.
 * <p>
 * Check if the mastering target is set.
 * </p>
 */

#define MleDwpChecker_TargetIsSet(checker) \
    ((checker)->getFlags() & MLE_DWP_CHECK_TARGETMASK)

/**
 * Macro function
 * <p>
 * Check if the mastering target is the SGI platform.
 * </p>
 */
#define MleDwpChecker_TargetIsSGI(checker) \
    (((checker)->getFlags() & MLE_DWP_CHECK_TARGETMASK) == MLE_DWP_CHECK_TARGET_SGI)

/**
 * Macro function.
 * <p>
 * Check if the mastering target is the PC platform.
 * </p>
 */
#define MleDwpChecker_TargetIsPC(checker) \
    (((checker)->getFlags() & MLE_DWP_CHECK_TARGETMASK) == MLE_DWP_CHECK_TARGET_PC)

 /**
 * Macro function.
 * <p>
 * Check if the mastering target is the Linux platform.
 * </p>
 */
#define MleDwpChecker_TargetIsLINUX(checker) \
    (((checker)->getFlags() & MLE_DWP_CHECK_TARGETMASK) == MLE_DWP_CHECK_TARGET_LINUX)

/**
 * Macro function.
 * <p>
 * Set the mastering target to the SGI platform.
 * </p>
 */
#define MleDwpChecker_SetTargetToSGI(checker) \
    ((checker)->setFlags(MLE_DWP_CHECK_TARGET_SGI,MLE_DWP_CHECK_TARGETMASK))

/**
 * Macro function.
 * <p>
 * Set the mastering target to the PC platform.
 * </p>
 */
#define MleDwpChecker_SetTargetToPC(checker) \
    ((checker)->setFlags(MLE_DWP_CHECK_TARGET_PC,MLE_DWP_CHECK_TARGETMASK))

/**
 * Macro function.
 * <p>
 * Set the mastering target to the Linux platform.
 * </p>
 */
#define MleDwpChecker_SetTargetToLINUX(checker) \
    ((checker)->setFlags(MLE_DWP_CHECK_TARGET_LINUX,MLE_DWP_CHECK_TARGETMASK))

#endif /* __MLE_DWP_CHECKER_H_ */
