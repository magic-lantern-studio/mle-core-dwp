/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file Dwp.cpp
 * @ingroup MleDWPAccess
 *
 * This file contains the implementation for initializing the Digital Workprint
 * library.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2022 Wizzer Works
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

// Include system header files
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlAssert.h"

//Include Digital Workprint header files.
#include "mle/Dwp.h"

#include "mle/DwpActor.h"
#include "mle/DwpActorDef.h"
#include "mle/DwpActorTemplate.h"
#include "mle/DwpForum.h"				// deprecated
#include "mle/DwpForumDef.h"			// deprecated
#include "mle/DwpDelegateAttachment.h"	// deprecated
#include "mle/DwpDelegateBinding.h"		// deprecated
#include "mle/DwpDelegateDef.h"			// deprecated
#include "mle/DwpSet.h"
#include "mle/DwpSetDef.h"
#include "mle/DwpRoleAttachment.h"
#include "mle/DwpRoleBinding.h"
#include "mle/DwpRoleDef.h"
#include "mle/DwpDSOFile.h"
#include "mle/DwpGroup.h"
#include "mle/DwpScene.h"
#include "mle/DwpBoot.h"
#include "mle/DwpHeaderFile.h"
#include "mle/DwpInclude.h"
#include "mle/DwpMediaRef.h"
#include "mle/DwpMediaRefSource.h"
#include "mle/DwpMediaRefTarget.h"
#include "mle/DwpPackage.h"
#include "mle/DwpProperty.h"
#include "mle/DwpPropertyDef.h"
#include "mle/DwpSourceFile.h"
#include "mle/DwpStage.h"
#include "mle/DwpStageDef.h"
#include "mle/DwpContainer.h"

#include "mle/DwpInt.h"
#include "mle/DwpFloat.h"
#include "mle/DwpScalar.h"
#include "mle/DwpVector2.h"
#include "mle/DwpVector3.h"
#include "mle/DwpVector4.h"
#include "mle/DwpRotation.h"
#include "mle/DwpTransform.h"
#include "mle/DwpString.h"
#include "mle/DwpIntArray.h"
#include "mle/DwpFloatArray.h"
#include "mle/DwpVoidStarType.h"
#include "mle/DwpEnum.h"
#include "mle/DwpLimits.h"
#include "mle/DwpDelegateForumMapping.h"	// deprecated
#include "mle/DwpRoleSetMapping.h"
#include "mle/DwpConverter.h"
#include "mle/DwpMedia.h"
#include "mle/DwpMediaRefClass.h"
#include "mle/DwpFiletype.h"

#if defined(__linux__)
#if defined(HAVE_LOG4CXX)
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

LoggerPtr g_dwpLogger(Logger::getLogger("DWP"));
static MlBoolean g_dwpLoggerInitialized = FALSE;
#endif /* HAVE_LOG4CXX */
#endif

/*
 * This function initializes Magic Lantern system types.  It is possible
 * to link these in on demand using a DSO, but since they will be
 * commonly used it is simpler not to use that mechanism.
 */
void mleDwpInit(void)
{
	static int called = 0;

	// Don't initialize more than once.
	//   Datatypes should be unique.  If they are created more
	//   than once, datatype comparison may fail.
	MLE_ASSERT(called == 0);
	called = 1;

	// initClass workprint items.
	MleDwpActor::initClass();
	MleDwpActorDef::initClass();
	MleDwpActorTemplate::initClass();
	MleDwpBoot::initClass();
	MleDwpContainer::initClass();
	MleDwpDelegateAttachment::initClass();	// deprecated
	MleDwpRoleAttachment::initClass();
	MleDwpDelegateBinding::initClass();		// deprecated
	MleDwpRoleBinding::initClass();
	MleDwpDelegateDef::initClass();			// deprecated
	MleDwpRoleDef::initClass();
	MleDwpDSOFile::initClass();
	MleDwpForum::initClass();				// deprecated
	MleDwpSet::initClass();
	MleDwpForumDef::initClass();			// deprecated
	MleDwpSetDef::initClass();
	MleDwpGroup::initClass();
	MleDwpScene::initClass();
	MleDwpHeaderFile::initClass();
	MleDwpInclude::initClass();
	MleDwpMediaRef::initClass();
	MleDwpSourceFile::initClass();
	MleDwpPackage::initClass();
	MleDwpMediaRefTarget::initClass();
	MleDwpMediaRefSource::initClass();
	MleDwpProperty::initClass();
	MleDwpPropertyDef::initClass();
	MleDwpStage::initClass();
	MleDwpStageDef::initClass();
	MleDwpRoleSetMapping::initClass();

	// initClass property data types.
	MleDwpInt::initClass();
	MleDwpFloat::initClass();
	MleDwpScalar::initClass();
	MleDwpVector2::initClass();
	MleDwpVector3::initClass();
	MleDwpVector4::initClass();
	MleDwpRotation::initClass();
	MleDwpTransform::initClass();
	MleDwpIntArray::initClass();
	MleDwpFloatArray::initClass();
	MleDwpString::initClass();
	MleDwpMediaRefType::initClass();

	MleDwpEnum::initClass();
	MleDwpLimits::initClass();
	MleDwpDelegateForumMapping::initClass();
	MleDwpConverter::initClass();
	MleDwpMedia::initClass();
	MleDwpMediaRefClass::initClass();
	MleDwpFiletype::initClass();

#if defined(__linux__)
#if defined(HAVE_LOG4CXX)
	if (!g_dwpLoggerInitialized) {
		/// Set up a simple configuration that logs to the console.
	    BasicConfigurator::configure();
	    //LoggerPtr logger(Logger::getLogger("main"));
	    LOG4CXX_DEBUG(g_dwpLogger, "log4cxx Initialized");
	    g_dwpLoggerInitialized = TRUE;
	}
#endif /* HAVE_LOG4CXX */
#endif
}
