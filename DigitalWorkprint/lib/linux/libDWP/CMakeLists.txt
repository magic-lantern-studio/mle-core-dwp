# COPYRIGHT_BEGIN
#
# The MIT License (MIT)
#
# Copyright (c) 2025 Wizzer Works
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#  For information concerning this header file, contact Mark S. Millard,
#  of Wizzer Works at msm@wizzerworks.com.
#
#  More information concerning Wizzer Works may be found at
#
#      http://www.wizzerworks.com
#
# COPYRIGHT_END

# Stop if cmake version is below 3.0
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Project name and enable C++ support
project(libdwp VERSION 1.0.0 LANGUAGES C CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify where to find CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Turn on verbose mode.
set(CMAKE_VERBOSE_MAKEFILE ON)

# Specify Debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

message(STATUS "***** Processing libDWP *****")

# Fail early if one of the required packages cannot be found
include(FindMLUTIL)
find_package(MLUTIL REQUIRED)
include(FindMLMATH)
find_package(MLMATH REQUIRED)
find_package(PkgConfig REQUIRED)

# Check for Magic Lantern environment variables
if (DEFINED ENV{MLE_ROOT})
  message(STATUS "MLE_ROOT=" $ENV{MLE_ROOT})
else()
  message(STATUS "MLE_ROOT NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern")
  message(STATUS "Using " $ENV{MLE_ROOT})
endif()

# Specify the library

# Specify the include directories for the header files
include_directories(
  ../include
  ../../common/include
  $ENV{MLE_ROOT}/include)

# Specify the compiler macro definitions
add_compile_definitions(
  MLE_NOT_UTIL_DLL
  MLE_NOT_MATH_DLL
  MLE_NOT_DWP_DLL)

# Specify the shared library
add_library(
  dwpShared SHARED
    ../../common/src/DWP/Dwp.cpp
    ../../common/src/DWPBase/DwpDict.cpp
    ../../common/src/DWPBase/DwpStrKeyDict.cpp
    ../../common/src/DWPBase/DwpMonitor.cpp
    ../../common/src/DWPBase/DwpDiscriminator.cpp
    ../../common/src/DWPBase/DwpTagDiscriminator.cpp
    ../../common/src/DWPBase/DwpTagAllDiscriminator.cpp
    ../../common/src/DWPBase/DwpNotifier.cpp
    ../../common/src/DWPBase/DwpContainer.cpp
    ../../common/src/DWPBase/DwpInclude.cpp
    ../../common/src/DWPBase/DwpFile.cpp
    ../../common/src/DWPBase/DwpFilename.cpp
    ../../common/src/DWPBase/DwpDSOFile.cpp
    ../../common/src/DWPBase/DwpUnknown.cpp
    ../../common/src/DWPBase/DwpFinder.cpp
    ../../common/src/DWPBase/DwpInput.cpp
    ../../common/src/DWPBase/DwpOutput.cpp
    ../../common/src/DWPBase/DwpItem.cpp
    ../../common/src/DWPBase/DwpLoad.cpp
    ../../common/src/DWPType/DwpArray.cpp
    ../../common/src/DWPType/DwpDatatype.cpp
    ../../common/src/DWPType/DwpDataUnion.cpp
    ../../common/src/DWPType/DwpEnum.cpp
    ../../common/src/DWPType/DwpEnumDef.cpp
    ../../common/src/DWPType/DwpFloat.cpp
    ../../common/src/DWPType/DwpFloat3.cpp
    ../../common/src/DWPType/DwpFloat4.cpp
    ../../common/src/DWPType/DwpFloatArray.cpp
    ../../common/src/DWPType/DwpInt.cpp
    ../../common/src/DWPType/DwpIntArray.cpp
    ../../common/src/DWPType/DwpMatrix34.cpp
    ../../common/src/DWPType/DwpRotation.cpp
    ../../common/src/DWPType/DwpScalar.cpp
    ../../common/src/DWPType/DwpScalarArray.cpp
    ../../common/src/DWPType/DwpString.cpp
    ../../common/src/DWPType/DwpTransform.cpp
    ../../common/src/DWPType/DwpVector2.cpp
    ../../common/src/DWPType/DwpVector3.cpp
    ../../common/src/DWPType/DwpVector3Array.cpp
    ../../common/src/DWPType/DwpVector4.cpp
    ../../common/src/DWPType/DwpVoidStarType.cpp
    ../../common/src/DWPModel/DwpActor.cpp
    ../../common/src/DWPModel/DwpActorDef.cpp
    ../../common/src/DWPModel/DwpActorSet.cpp
    ../../common/src/DWPModel/DwpActorTemplate.cpp
    ../../common/src/DWPModel/DwpBoot.cpp
    ../../common/src/DWPModel/DwpCast.cpp
    ../../common/src/DWPModel/DwpCastRef.cpp
    ../../common/src/DWPModel/DwpChecker.cpp
    ../../common/src/DWPModel/DwpConverter.cpp
    ../../common/src/DWPModel/DwpDelegateAttachment.cpp
    ../../common/src/DWPModel/DwpDelegateBinding.cpp
    ../../common/src/DWPModel/DwpDelegateDef.cpp
    ../../common/src/DWPModel/DwpDelegateForumMapping.cpp
    ../../common/src/DWPModel/DwpFiletype.cpp
    ../../common/src/DWPModel/DwpForum.cpp
    ../../common/src/DWPModel/DwpForumDef.cpp
    ../../common/src/DWPModel/DwpGroup.cpp
    ../../common/src/DWPModel/DwpGroupRef.cpp
    ../../common/src/DWPModel/DwpHeaderFile.cpp
    ../../common/src/DWPModel/DwpLimits.cpp
    ../../common/src/DWPModel/DwpMedia.cpp
    ../../common/src/DWPModel/DwpMediaRef.cpp
    ../../common/src/DWPModel/DwpMediaRefSource.cpp
    ../../common/src/DWPModel/DwpMediaRefTarget.cpp
    ../../common/src/DWPModel/DwpMediaRefClass.cpp
    ../../common/src/DWPModel/DwpPackage.cpp
    ../../common/src/DWPModel/DwpProperty.cpp
    ../../common/src/DWPModel/DwpPropertyDef.cpp
    ../../common/src/DWPModel/DwpRoleAttachment.cpp
    ../../common/src/DWPModel/DwpRoleBinding.cpp
    ../../common/src/DWPModel/DwpRoleDef.cpp
    ../../common/src/DWPModel/DwpRoleSetMapping.cpp
    ../../common/src/DWPModel/DwpScene.cpp
    ../../common/src/DWPModel/DwpSet.cpp
    ../../common/src/DWPModel/DwpSetDef.cpp
    ../../common/src/DWPModel/DwpSourceFile.cpp
    ../../common/src/DWPModel/DwpStage.cpp
    ../../common/src/DWPModel/DwpStageDef.cpp)

# Specify the static library
add_library(
  dwpStatic STATIC
    ../../common/src/DWP/Dwp.cpp
    ../../common/src/DWPBase/DwpDict.cpp
    ../../common/src/DWPBase/DwpStrKeyDict.cpp
    ../../common/src/DWPBase/DwpMonitor.cpp
    ../../common/src/DWPBase/DwpDiscriminator.cpp
    ../../common/src/DWPBase/DwpTagDiscriminator.cpp
    ../../common/src/DWPBase/DwpTagAllDiscriminator.cpp
    ../../common/src/DWPBase/DwpNotifier.cpp
    ../../common/src/DWPBase/DwpContainer.cpp
    ../../common/src/DWPBase/DwpInclude.cpp
    ../../common/src/DWPBase/DwpFile.cpp
    ../../common/src/DWPBase/DwpFilename.cpp
    ../../common/src/DWPBase/DwpDSOFile.cpp
    ../../common/src/DWPBase/DwpUnknown.cpp
    ../../common/src/DWPBase/DwpFinder.cpp
    ../../common/src/DWPBase/DwpInput.cpp
    ../../common/src/DWPBase/DwpOutput.cpp
    ../../common/src/DWPBase/DwpItem.cpp
    ../../common/src/DWPBase/DwpLoad.cpp
    ../../common/src/DWPType/DwpArray.cpp
    ../../common/src/DWPType/DwpDatatype.cpp
    ../../common/src/DWPType/DwpDataUnion.cpp
    ../../common/src/DWPType/DwpEnum.cpp
    ../../common/src/DWPType/DwpEnumDef.cpp
    ../../common/src/DWPType/DwpFloat.cpp
    ../../common/src/DWPType/DwpFloat3.cpp
    ../../common/src/DWPType/DwpFloat4.cpp
    ../../common/src/DWPType/DwpFloatArray.cpp
    ../../common/src/DWPType/DwpInt.cpp
    ../../common/src/DWPType/DwpIntArray.cpp
    ../../common/src/DWPType/DwpMatrix34.cpp
    ../../common/src/DWPType/DwpRotation.cpp
    ../../common/src/DWPType/DwpScalar.cpp
    ../../common/src/DWPType/DwpScalarArray.cpp
    ../../common/src/DWPType/DwpString.cpp
    ../../common/src/DWPType/DwpTransform.cpp
    ../../common/src/DWPType/DwpVector2.cpp
    ../../common/src/DWPType/DwpVector3.cpp
    ../../common/src/DWPType/DwpVector3Array.cpp
    ../../common/src/DWPType/DwpVector4.cpp
    ../../common/src/DWPType/DwpVoidStarType.cpp
    ../../common/src/DWPModel/DwpActor.cpp
    ../../common/src/DWPModel/DwpActorDef.cpp
    ../../common/src/DWPModel/DwpActorSet.cpp
    ../../common/src/DWPModel/DwpActorTemplate.cpp
    ../../common/src/DWPModel/DwpBoot.cpp
    ../../common/src/DWPModel/DwpCast.cpp
    ../../common/src/DWPModel/DwpCastRef.cpp
    ../../common/src/DWPModel/DwpChecker.cpp
    ../../common/src/DWPModel/DwpConverter.cpp
    ../../common/src/DWPModel/DwpDelegateAttachment.cpp
    ../../common/src/DWPModel/DwpDelegateBinding.cpp
    ../../common/src/DWPModel/DwpDelegateDef.cpp
    ../../common/src/DWPModel/DwpDelegateForumMapping.cpp
    ../../common/src/DWPModel/DwpFiletype.cpp
    ../../common/src/DWPModel/DwpForum.cpp
    ../../common/src/DWPModel/DwpForumDef.cpp
    ../../common/src/DWPModel/DwpGroup.cpp
    ../../common/src/DWPModel/DwpGroupRef.cpp
    ../../common/src/DWPModel/DwpHeaderFile.cpp
    ../../common/src/DWPModel/DwpLimits.cpp
    ../../common/src/DWPModel/DwpMedia.cpp
    ../../common/src/DWPModel/DwpMediaRef.cpp
    ../../common/src/DWPModel/DwpMediaRefSource.cpp
    ../../common/src/DWPModel/DwpMediaRefTarget.cpp
    ../../common/src/DWPModel/DwpMediaRefClass.cpp
    ../../common/src/DWPModel/DwpPackage.cpp
    ../../common/src/DWPModel/DwpProperty.cpp
    ../../common/src/DWPModel/DwpPropertyDef.cpp
    ../../common/src/DWPModel/DwpRoleAttachment.cpp
    ../../common/src/DWPModel/DwpRoleBinding.cpp
    ../../common/src/DWPModel/DwpRoleDef.cpp
    ../../common/src/DWPModel/DwpRoleSetMapping.cpp
    ../../common/src/DWPModel/DwpScene.cpp
    ../../common/src/DWPModel/DwpSet.cpp
    ../../common/src/DWPModel/DwpSetDef.cpp
    ../../common/src/DWPModel/DwpSourceFile.cpp
    ../../common/src/DWPModel/DwpStage.cpp
    ../../common/src/DWPModel/DwpStageDef.cpp)

  # Specify the shared library properties
  set_target_properties(dwpShared PROPERTIES
    OUTPUT_NAME dwp
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

  target_compile_options(dwpShared
    PRIVATE
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Release>:>)

  target_compile_definitions(dwpShared
    PRIVATE
      $<$<CONFIG:Debug>: MLE_DEBUG DEBUG>
      $<$<CONFIG:Release>:>)

  # Specify the static library properties
  set_target_properties(dwpStatic PROPERTIES
    OUTPUT_NAME dwp
    VERSION ${PROJECT_VERSION})

  target_compile_options(dwpStatic
    PRIVATE
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Release>:>)

  target_compile_definitions(dwpStatic
    PRIVATE
      $<$<CONFIG:Debug>: MLE_DEBUG>
      $<$<CONFIG:Release>:>)

  # Install the libraries
  install(
    TARGETS
      dwpShared
      dwpStatic
    DESTINATION
      lib)

  # Install the header files
  install(
    FILES
      ../include/mle/DwpPlatformDefs.h
      ../include/mle/DwpPlatformTypes.h
      ../../common/include/mle/DwpActorDef.h
      ../../common/include/mle/DwpActor.h
      ../../common/include/mle/DwpActorSet.h
      ../../common/include/mle/DwpActorTemplate.h
      ../../common/include/mle/DwpArray.h
      ../../common/include/mle/DwpBoot.h
      ../../common/include/mle/DwpCast.h
      ../../common/include/mle/DwpCastRef.h
      ../../common/include/mle/DwpChecker.h
      ../../common/include/mle/DwpCommonDefs.h
      ../../common/include/mle/DwpCommonTypes.h
      ../../common/include/mle/DwpContainer.h
      ../../common/include/mle/DwpConverter.h
      ../../common/include/mle/DwpDatatype.h
      ../../common/include/mle/DwpDataUnion.h
      ../../common/include/mle/DwpDelegateAttachment.h
      ../../common/include/mle/DwpDelegateBinding.h
      ../../common/include/mle/DwpDelegateDef.h
      ../../common/include/mle/DwpDelegateForumMapping.h
      ../../common/include/mle/DwpDict.h
      ../../common/include/mle/DwpDiscriminator.h
      ../../common/include/mle/DwpDSOFile.h
      ../../common/include/mle/DwpEnumDef.h
      ../../common/include/mle/DwpEnum.h
      ../../common/include/mle/DwpFile.h
      ../../common/include/mle/DwpFilename.h
      ../../common/include/mle/DwpFiletype.h
      ../../common/include/mle/DwpFinder.h
      ../../common/include/mle/DwpFloat3.h
      ../../common/include/mle/DwpFloat4.h
      ../../common/include/mle/DwpFloatArray.h
      ../../common/include/mle/DwpFloat.h
      ../../common/include/mle/DwpForumDef.h
      ../../common/include/mle/DwpForum.h
      ../../common/include/mle/DwpGroup.h
      ../../common/include/mle/DwpGroupRef.h
      ../../common/include/mle/Dwp.h
      ../../common/include/mle/DwpHeaderFile.h
      ../../common/include/mle/DwpInclude.h
      ../../common/include/mle/DwpInput.h
      ../../common/include/mle/DwpIntArray.h
      ../../common/include/mle/DwpInt.h
      ../../common/include/mle/DwpItem.h
      ../../common/include/mle/DwpLimits.h
      ../../common/include/mle/DwpLoad.h
      ../../common/include/mle/DwpMatrix34.h
      ../../common/include/mle/DwpMedia.h
      ../../common/include/mle/DwpMediaRefClass.h
      ../../common/include/mle/DwpMediaRef.h
      ../../common/include/mle/DwpMediaRefSource.h
      ../../common/include/mle/DwpMediaRefTarget.h
      ../../common/include/mle/DwpMonitor.h
      ../../common/include/mle/DwpNotifier.h
      ../../common/include/mle/DwpOutput.h
      ../../common/include/mle/DwpPackage.h
      ../../common/include/mle/DwpPropertyDef.h
      ../../common/include/mle/DwpProperty.h
      ../../common/include/mle/DwpRoleAttachment.h
      ../../common/include/mle/DwpRoleBinding.h
      ../../common/include/mle/DwpRoleDef.h
      ../../common/include/mle/DwpRoleSetMapping.h
      ../../common/include/mle/DwpRotation.h
      ../../common/include/mle/DwpScalarArray.h
      ../../common/include/mle/DwpScalar.h
      ../../common/include/mle/DwpScene.h
      ../../common/include/mle/DwpSetDef.h
      ../../common/include/mle/DwpSet.h
      ../../common/include/mle/DwpSourceFile.h
      ../../common/include/mle/DwpStageDef.h
      ../../common/include/mle/DwpStage.h
      ../../common/include/mle/DwpString.h
      ../../common/include/mle/DwpStrKeyDict.h
      ../../common/include/mle/DwpTagAllDiscriminator.h
      ../../common/include/mle/DwpTagDiscriminator.h
      ../../common/include/mle/DwpTblMgr.h
      ../../common/include/mle/DwpTransform.h
      ../../common/include/mle/DwpUnknown.h
      ../../common/include/mle/DwpVector2.h
      ../../common/include/mle/DwpVector3Array.h
      ../../common/include/mle/DwpVector3.h
      ../../common/include/mle/DwpVector4.h
      ../../common/include/mle/DwpVoidStarType.h
    DESTINATION
      include/mle)
