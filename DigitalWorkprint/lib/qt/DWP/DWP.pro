QT -= gui

TEMPLATE = lib
DEFINES += DWP_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += MLE_NOT_DLL DEBUG

INCLUDEPATH += $$PWD/../../common/include $$PWD/../../linux/include /opt/MagicLantern/include

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/../../common/src/DWP/Dwp.cpp

SOURCES += \
    $$PWD/../../common/src/DWPBase/DwpContainer.cpp \
    $$PWD/../../common/src/DWPBase/DwpDict.cpp \
    $$PWD/../../common/src/DWPBase/DwpDiscriminator.cpp \
    $$PWD/../../common/src/DWPBase/DwpDSOFile.cpp \
    $$PWD/../../common/src/DWPBase/DwpFile.cpp \
    $$PWD/../../common/src/DWPBase/DwpFilename.cpp \
    $$PWD/../../common/src/DWPBase/DwpFinder.cpp \
    $$PWD/../../common/src/DWPBase/DwpInclude.cpp \
    $$PWD/../../common/src/DWPBase/DwpInput.cpp \
    $$PWD/../../common/src/DWPBase/DwpItem.cpp \
    $$PWD/../../common/src/DWPBase/DwpLoad.cpp \
    $$PWD/../../common/src/DWPBase/DwpMonitor.cpp \
    $$PWD/../../common/src/DWPBase/DwpNotifier.cpp \
    $$PWD/../../common/src/DWPBase/DwpOutput.cpp \
    $$PWD/../../common/src/DWPBase/DwpStrKeyDict.cpp \
    $$PWD/../../common/src/DWPBase/DwpTagAllDiscriminator.cpp \
    $$PWD/../../common/src/DWPBase/DwpTagDiscriminator.cpp \
    $$PWD/../../common/src/DWPBase/DwpUnknown.cpp

SOURCES += \
    $$PWD/../../common/src/DWPModel/DwpActor.cpp \
    $$PWD/../../common/src/DWPModel/DwpActorDef.cpp \
    $$PWD/../../common/src/DWPModel/DwpActorSet.cpp \
    $$PWD/../../common/src/DWPModel/DwpActorTemplate.cpp \
    $$PWD/../../common/src/DWPModel/DwpBoot.cpp \
    $$PWD/../../common/src/DWPModel/DwpCast.cpp \
    $$PWD/../../common/src/DWPModel/DwpCastRef.cpp \
    $$PWD/../../common/src/DWPModel/DwpChecker.cpp \
    $$PWD/../../common/src/DWPModel/DwpConverter.cpp \
    $$PWD/../../common/src/DWPModel/DwpDelegateAttachment.cpp \
    $$PWD/../../common/src/DWPModel/DwpDelegateBinding.cpp \
    $$PWD/../../common/src/DWPModel/DwpDelegateDef.cpp \
    $$PWD/../../common/src/DWPModel/DwpDelegateForumMapping.cpp \
    $$PWD/../../common/src/DWPModel/DwpFiletype.cpp \
    $$PWD/../../common/src/DWPModel/DwpForum.cpp \
    $$PWD/../../common/src/DWPModel/DwpForumDef.cpp \
    $$PWD/../../common/src/DWPModel/DwpGroup.cpp \
    $$PWD/../../common/src/DWPModel/DwpGroupRef.cpp \
    $$PWD/../../common/src/DWPModel/DwpHeaderFile.cpp \
    $$PWD/../../common/src/DWPModel/DwpLimits.cpp \
    $$PWD/../../common/src/DWPModel/DwpMedia.cpp \
    $$PWD/../../common/src/DWPModel/DwpMediaRefClass.cpp \
    $$PWD/../../common/src/DWPModel/DwpMediaRef.cpp \
    $$PWD/../../common/src/DWPModel/DwpMediaRefSource.cpp \
    $$PWD/../../common/src/DWPModel/DwpMediaRefTarget.cpp \
    $$PWD/../../common/src/DWPModel/DwpPackage.cpp \
    $$PWD/../../common/src/DWPModel/DwpProperty.cpp \
    $$PWD/../../common/src/DWPModel/DwpPropertyDef.cpp \
    $$PWD/../../common/src/DWPModel/DwpRoleAttachment.cpp \
    $$PWD/../../common/src/DWPModel/DwpRoleBinding.cpp \
    $$PWD/../../common/src/DWPModel/DwpRoleDef.cpp \
    $$PWD/../../common/src/DWPModel/DwpRoleSetMapping.cpp \
    $$PWD/../../common/src/DWPModel/DwpScene.cpp \
    $$PWD/../../common/src/DWPModel/DwpSet.cpp \
    $$PWD/../../common/src/DWPModel/DwpSetDef.cpp \
    $$PWD/../../common/src/DWPModel/DwpSourceFile.cpp \
    $$PWD/../../common/src/DWPModel/DwpStage.cpp \
    $$PWD/../../common/src/DWPModel/DwpStageDef.cpp

SOURCES += \
    $$PWD/../../common/src/DWPType/DwpArray.cpp \
    $$PWD/../../common/src/DWPType/DwpDatatype.cpp \
    $$PWD/../../common/src/DWPType/DwpDataUnion.cpp \
    $$PWD/../../common/src/DWPType/DwpEnum.cpp \
    $$PWD/../../common/src/DWPType/DwpEnumDef.cpp \
    $$PWD/../../common/src/DWPType/DwpFloat3.cpp \
    $$PWD/../../common/src/DWPType/DwpFloat4.cpp \
    $$PWD/../../common/src/DWPType/DwpFloatArray.cpp \
    $$PWD/../../common/src/DWPType/DwpFloat.cpp \
    $$PWD/../../common/src/DWPType/DwpIntArray.cpp \
    $$PWD/../../common/src/DWPType/DwpInt.cpp \
    $$PWD/../../common/src/DWPType/DwpMatrix34.cpp \
    $$PWD/../../common/src/DWPType/DwpRotation.cpp \
    $$PWD/../../common/src/DWPType/DwpScalarArray.cpp \
    $$PWD/../../common/src/DWPType/DwpScalar.cpp \
    $$PWD/../../common/src/DWPType/DwpString.cpp \
    $$PWD/../../common/src/DWPType/DwpTransform.cpp \
    $$PWD/../../common/src/DWPType/DwpVector2.cpp \
    $$PWD/../../common/src/DWPType/DwpVector3Array.cpp \
    $$PWD/../../common/src/DWPType/DwpVector3.cpp \
    $$PWD/../../common/src/DWPType/DwpVector4.cpp \
    $$PWD/../../common/src/DWPType/DwpVoidStarType.cpp

HEADERS += \
    $$PWD/../../common/include/mle/DwpActorDef.h \
    $$PWD/../../common/include/mle/DwpActor.h \
    $$PWD/../../common/include/mle/DwpActorSet.h \
    $$PWD/../../common/include/mle/DwpActorTemplate.h \
    $$PWD/../../common/include/mle/DwpArray.h \
    $$PWD/../../common/include/mle/DwpBoot.h \
    $$PWD/../../common/include/mle/DwpCast.h \
    $$PWD/../../common/include/mle/DwpCastRef.h \
    $$PWD/../../common/include/mle/DwpChecker.h \
    $$PWD/../../common/include/mle/DwpCommonDefs.h \
    $$PWD/../../common/include/mle/DwpCommonTypes.h \
    $$PWD/../../common/include/mle/DwpContainer.h \
    $$PWD/../../common/include/mle/DwpConverter.h \
    $$PWD/../../common/include/mle/DwpDatatype.h \
    $$PWD/../../common/include/mle/DwpDataUnion.h \
    $$PWD/../../common/include/mle/DwpDelegateAttachment.h \
    $$PWD/../../common/include/mle/DwpDelegateBinding.h \
    $$PWD/../../common/include/mle/DwpDelegateDef.h \
    $$PWD/../../common/include/mle/DwpDelegateForumMapping.h \
    $$PWD/../../common/include/mle/DwpDict.h \
    $$PWD/../../common/include/mle/DwpDiscriminator.h \
    $$PWD/../../common/include/mle/DwpDSOFile.h \
    $$PWD/../../common/include/mle/DwpEnumDef.h \
    $$PWD/../../common/include/mle/DwpEnum.h \
    $$PWD/../../common/include/mle/DwpFile.h \
    $$PWD/../../common/include/mle/DwpFilename.h \
    $$PWD/../../common/include/mle/DwpFiletype.h \
    $$PWD/../../common/include/mle/DwpFinder.h \
    $$PWD/../../common/include/mle/DwpFloat3.h \
    $$PWD/../../common/include/mle/DwpFloat4.h \
    $$PWD/../../common/include/mle/DwpFloatArray.h \
    $$PWD/../../common/include/mle/DwpFloat.h \
    $$PWD/../../common/include/mle/DwpForumDef.h \
    $$PWD/../../common/include/mle/DwpForum.h \
    $$PWD/../../common/include/mle/DwpGroup.h \
    $$PWD/../../common/include/mle/DwpGroupRef.h \
    $$PWD/../../common/include/mle/Dwp.h \
    $$PWD/../../common/include/mle/DwpHeaderFile.h \
    $$PWD/../../common/include/mle/DwpInclude.h \
    $$PWD/../../common/include/mle/DwpInput.h \
    $$PWD/../../common/include/mle/DwpIntArray.h \
    $$PWD/../../common/include/mle/DwpInt.h \
    $$PWD/../../common/include/mle/DwpItem.h \
    $$PWD/../../common/include/mle/DwpLimits.h \
    $$PWD/../../common/include/mle/DwpLoad.h \
    $$PWD/../../common/include/mle/DwpMatrix34.h \
    $$PWD/../../common/include/mle/DwpMedia.h \
    $$PWD/../../common/include/mle/DwpMediaRefClass.h \
    $$PWD/../../common/include/mle/DwpMediaRef.h \
    $$PWD/../../common/include/mle/DwpMediaRefSource.h \
    $$PWD/../../common/include/mle/DwpMediaRefTarget.h \
    $$PWD/../../common/include/mle/DwpMonitor.h \
    $$PWD/../../common/include/mle/DwpNotifier.h \
    $$PWD/../../common/include/mle/DwpOutput.h \
    $$PWD/../../common/include/mle/DwpPackage.h \
    $$PWD/../../common/include/mle/DwpPropertyDef.h \
    $$PWD/../../common/include/mle/DwpProperty.h \
    $$PWD/../../common/include/mle/DwpRoleAttachment.h \
    $$PWD/../../common/include/mle/DwpRoleBinding.h \
    $$PWD/../../common/include/mle/DwpRoleDef.h \
    $$PWD/../../common/include/mle/DwpRoleSetMapping.h \
    $$PWD/../../common/include/mle/DwpRotation.h \
    $$PWD/../../common/include/mle/DwpScalarArray.h \
    $$PWD/../../common/include/mle/DwpScalar.h \
    $$PWD/../../common/include/mle/DwpScene.h \
    $$PWD/../../common/include/mle/DwpSetDef.h \
    $$PWD/../../common/include/mle/DwpSet.h \
    $$PWD/../../common/include/mle/DwpSourceFile.h \
    $$PWD/../../common/include/mle/DwpStageDef.h \
    $$PWD/../../common/include/mle/DwpStage.h \
    $$PWD/../../common/include/mle/DwpString.h \
    $$PWD/../../common/include/mle/DwpStrKeyDict.h \
    $$PWD/../../common/include/mle/DwpTagAllDiscriminator.h \
    $$PWD/../../common/include/mle/DwpTagDiscriminator.h \
    $$PWD/../../common/include/mle/DwpTblMgr.h \
    $$PWD/../../common/include/mle/DwpTransform.h \
    $$PWD/../../common/include/mle/DwpUnknown.h \
    $$PWD/../../common/include/mle/DwpVector2.h \
    $$PWD/../../common/include/mle/DwpVector3Array.h \
    $$PWD/../../common/include/mle/DwpVector3.h \
    $$PWD/../../common/include/mle/DwpVector4.h \
    $$PWD/../../common/include/mle/DwpVoidStarType.h

HEADERS += \
    $$PWD/../../linux/include/mle/DwpPlatformDefs.h \
    $$PWD/../../linux/include/mle/DwpPlatformTypes.h

# Default rules for deployment.
unix {
    target.path = /opt/MagicLantern/lib
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    INSTALLS += target headers
}
!isEmpty(target.path): INSTALLS += target
