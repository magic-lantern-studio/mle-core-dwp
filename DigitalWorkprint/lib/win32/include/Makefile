# -*- Makefile -*-
#
# Copyright (C) 2000-2006  Wizzer Works
#
# Makefile for installing header files.
#
# $Id$
#
###########################################################################

ISM_DEPTH = ..
include $(ISM_DEPTH)/ismdefs

VPATH = mle:$(ISM_DEPTH)/../common/include/mle

BASE_COMMON_HEADERS = \
	Dwp.h \
	DwpContainer.h \
	DwpDict.h \
	DwpDiscriminator.h \
	DwpDSOFile.h \
	DwpFile.h \
	DwpFilename.h \
	DwpFinder.h \
	DwpInclude.h \
	DwpInput.h \
	DwpItem.h \
	DwpMonitor.h \
	DwpNotifier.h \
	DwpOutput.h \
	DwpStrKeyDict.h \
	DwpTagAllDiscriminator.h \
	DwpTagDiscriminator.h \
	DwpUnknown.h \
	DwpCommonDefs.h \
	DwpCommonTypes.h \
	$(NULL)

BASE_WIN32_HEADERS = \
	DwpPlatformDefs.h \
	DwpPlatformTypes.h \
	$(NULL)

TYPE_COMMON_HEADERS = \
	DwpArray.h \
	DwpDatatype.h \
	DwpDataUnion.h \
	DwpEnum.h \
	DwpEnumDef.h \
	DwpFloat.h \
	DwpFloat3.h \
	DwpFloat4.h \
	DwpFloatArray.h \
	DwpInt.h \
	DwpIntArray.h \
	DwpMatrix34.h \
	DwpRotation.h \
	DwpScalar.h \
	DwpScalarArray.h \
	DwpString.h \
	DwpTransform.h \
	DwpVector2.h \
	DwpVector3.h \
	DwpVector3Array.h \
	DwpVector4.h \
	DwpVoidStarType.h \
	$(NULL)

MODEL_COMMON_HEADERS = \
	DwpActor.h \
	DwpActorDef.h \
	DwpActorSet.h \
	DwpActorTemplate.h \
	DwpBoot.h \
	DwpCast.h \
	DwpCastRef.h \
	DwpChecker.h \
	DwpConverter.h \
	DwpDelegateAttachment.h \
	DwpDelegateBinding.h \
	DwpDelegateDef.h \
	DwpDelegateForumMapping.h \
	DwpFiletype.h \
	DwpForum.h \
	DwpForumDef.h \
	DwpGroup.h \
	DwpGroupRef.h \
	DwpHeaderFile.h \
	DwpLimits.h \
	DwpMedia.h \
	DwpMediaRef.h \
	DwpMediaRefClass.h \
	DwpPackage.h \
	DwpProperty.h \
	DwpPropertyDef.h \
	DwpRoleAttachment.h \
	DwpRoleBinding.h \
	DwpRoleDef.h \
	DwpRoleSetMapping.h \
	DwpScene.h \
	DwpSet.h \
	DwpSetDef.h \
	DwpSourceFile.h \
	DwpStage.h \
	DwpStageDef.h \
	$(NULL)

DWP_COMMON_HEADERS = \
	Dwp.h \
	$(NULL)

COMMON_HEADERS = \
	$(BASE_COMMON_HEADERS) \
	$(TYPE_COMMON_HEADERS) \
	$(MODEL_COMMON_HEADERS) \
	$(DWP_COMMON_HEADERS) \
	$(NULL)

WIN32_HEADERS = \
	$(BASE_WIN32_HEADERS) \
	$(NULL)

HEADERS = \
	$(COMMON_HEADERS) \
	$(WIN32_HEADERS) \
	$(NULL)

install: $(HEADERS)
	$(INSTALL) -d $(MLE_ROOT)/include/mle
	$(INSTALL) $^ $(MLE_ROOT)/include/mle

export: $(_FORCE)

include $(ISMRULES)

collect:
	$(IBCOLLECT) -baseDir $(ISM_DEPTH)/.. \
                     -log ../build/collect.raw \
                     -srcDir $(ISM_DEPTH)/../common/include/mle \
                     -destDir "include/mle" \
                     -tags "INCLUDE" \
                     $(COMMON_HEADERS)
	$(IBCOLLECT) -baseDir $(ISM_DEPTH)/.. \
                     -log ../build/collect.raw \
                     -srcDir mle \
                     -destDir "include/mle" \
                     -tags "INCLUDE" \
                     $(WIN32_HEADERS)
