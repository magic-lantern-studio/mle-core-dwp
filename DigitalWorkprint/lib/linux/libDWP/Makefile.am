#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libDWP.la

#######################################
# Build information for each library

# Sources for libDWP
libDWP_la_SOURCES = \
	../../common/src/DWP/Dwp.cpp \
	../../common/src/DWPBase/DwpDict.cpp \
	../../common/src/DWPBase/DwpStrKeyDict.cpp \
	../../common/src/DWPBase/DwpMonitor.cpp \
	../../common/src/DWPBase/DwpDiscriminator.cpp \
	../../common/src/DWPBase/DwpTagDiscriminator.cpp \
	../../common/src/DWPBase/DwpTagAllDiscriminator.cpp \
	../../common/src/DWPBase/DwpNotifier.cpp \
	../../common/src/DWPBase/DwpContainer.cpp \
	../../common/src/DWPBase/DwpInclude.cpp \
	../../common/src/DWPBase/DwpFile.cpp \
	../../common/src/DWPBase/DwpFilename.cpp \
	../../common/src/DWPBase/DwpDSOFile.cpp \
	../../common/src/DWPBase/DwpUnknown.cpp \
	../../common/src/DWPBase/DwpFinder.cpp \
	../../common/src/DWPBase/DwpInput.cpp \
	../../common/src/DWPBase/DwpOutput.cpp \
	../../common/src/DWPBase/DwpItem.cpp \
	../../common/src/DWPBase/DwpLoad.cpp \
	../../common/src/DWPType/DwpArray.cpp \
	../../common/src/DWPType/DwpDatatype.cpp \
	../../common/src/DWPType/DwpDataUnion.cpp \
	../../common/src/DWPType/DwpEnum.cpp \
	../../common/src/DWPType/DwpEnumDef.cpp \
	../../common/src/DWPType/DwpFloat.cpp \
	../../common/src/DWPType/DwpFloat3.cpp \
	../../common/src/DWPType/DwpFloat4.cpp \
	../../common/src/DWPType/DwpFloatArray.cpp \
	../../common/src/DWPType/DwpInt.cpp \
	../../common/src/DWPType/DwpIntArray.cpp \
	../../common/src/DWPType/DwpMatrix34.cpp \
	../../common/src/DWPType/DwpRotation.cpp \
	../../common/src/DWPType/DwpScalar.cpp \
	../../common/src/DWPType/DwpScalarArray.cpp \
	../../common/src/DWPType/DwpString.cpp \
	../../common/src/DWPType/DwpTransform.cpp \
	../../common/src/DWPType/DwpVector2.cpp \
	../../common/src/DWPType/DwpVector3.cpp \
	../../common/src/DWPType/DwpVector3Array.cpp \
	../../common/src/DWPType/DwpVector4.cpp \
	../../common/src/DWPType/DwpVoidStarType.cpp \
	../../common/src/DWPModel/DwpActor.cpp \
	../../common/src/DWPModel/DwpActorDef.cpp \
	../../common/src/DWPModel/DwpActorSet.cpp \
	../../common/src/DWPModel/DwpActorTemplate.cpp \
	../../common/src/DWPModel/DwpBoot.cpp \
	../../common/src/DWPModel/DwpCast.cpp \
	../../common/src/DWPModel/DwpCastRef.cpp \
	../../common/src/DWPModel/DwpChecker.cpp \
	../../common/src/DWPModel/DwpConverter.cpp \
	../../common/src/DWPModel/DwpDelegateAttachment.cpp \
	../../common/src/DWPModel/DwpDelegateBinding.cpp \
	../../common/src/DWPModel/DwpDelegateDef.cpp \
	../../common/src/DWPModel/DwpDelegateForumMapping.cpp \
	../../common/src/DWPModel/DwpFiletype.cpp \
	../../common/src/DWPModel/DwpForum.cpp \
	../../common/src/DWPModel/DwpForumDef.cpp \
	../../common/src/DWPModel/DwpGroup.cpp \
	../../common/src/DWPModel/DwpGroupRef.cpp \
	../../common/src/DWPModel/DwpHeaderFile.cpp \
	../../common/src/DWPModel/DwpLimits.cpp \
	../../common/src/DWPModel/DwpMedia.cpp \
	../../common/src/DWPModel/DwpMediaRef.cpp \
	../../common/src/DWPModel/DwpMediaRefSource.cpp \
	../../common/src/DWPModel/DwpMediaRefTarget.cpp \
	../../common/src/DWPModel/DwpMediaRefClass.cpp \
	../../common/src/DWPModel/DwpPackage.cpp \
	../../common/src/DWPModel/DwpProperty.cpp \
	../../common/src/DWPModel/DwpPropertyDef.cpp \
	../../common/src/DWPModel/DwpRoleAttachment.cpp \
	../../common/src/DWPModel/DwpRoleBinding.cpp \
	../../common/src/DWPModel/DwpRoleDef.cpp \
	../../common/src/DWPModel/DwpRoleSetMapping.cpp \
	../../common/src/DWPModel/DwpScene.cpp \
	../../common/src/DWPModel/DwpSet.cpp \
	../../common/src/DWPModel/DwpSetDef.cpp \
	../../common/src/DWPModel/DwpSourceFile.cpp \
	../../common/src/DWPModel/DwpStage.cpp \
	../../common/src/DWPModel/DwpStageDef.cpp

# Linker options libTestProgram
libDWP_la_LDFLAGS = -version-info 1:0:0 -L/opt/MagicLantern/lib -lmlmath -ldl

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libDWP_la_CPPFLAGS = \
	-DMLE_NOT_UTIL_DLL \
	-DMLE_NOT_MATH_DLL \
	-DMLE_NOT_DWP_DLL \
	-DDEBUG \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../common/include \
	-I$(MLE_ROOT)/include \
	-std=c++17

if HAVE_LOG4CXX
#libDWP_la_SOURCES += \
#    audio_random_portaudio.h \
#    audio_random_portaudio.cpp
	
libDWP_la_CPPFLAGS += $(LOG4CXX_CFLAGS)

#libDWP_la_LDFLAGS += $(LOG4CXX_LIBS)
endif