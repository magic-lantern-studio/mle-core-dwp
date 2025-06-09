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
project(mlert VERSION 1.0.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify where to find CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Specify Debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

# Fail early if one of the required packages cannot be found
include(FindMLUTIL)
find_package(MLUTIL REQUIRED)
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
  include
  ../common/include
  $ENV{MLE_ROOT}/include)

# Specify the compiler macro definitions
add_compile_definitions(
  MLE_NOT_UTIL_DLL
  MLE_NOT_MATH_DLL
  MLE_NOT_DWP_DLL)

# Specify the shared library
add_library(
  dwpShared SHARED
    ../common/src/DWP/Dwp.cpp
    ../common/src/DWPBase/DwpDict.cpp
    ../common/src/DWPBase/DwpStrKeyDict.cpp
    ../common/src/DWPBase/DwpMonitor.cpp
    ../common/src/DWPBase/DwpDiscriminator.cpp
    ../common/src/DWPBase/DwpTagDiscriminator.cpp
    ../common/src/DWPBase/DwpTagAllDiscriminator.cpp
    ../common/src/DWPBase/DwpNotifier.cpp
    ../common/src/DWPBase/DwpContainer.cpp
    ../common/src/DWPBase/DwpInclude.cpp
    ../common/src/DWPBase/DwpFile.cpp
    ../common/src/DWPBase/DwpFilename.cpp
    ../common/src/DWPBase/DwpDSOFile.cpp
    ../common/src/DWPBase/DwpUnknown.cpp
    ../common/src/DWPBase/DwpFinder.cpp
    ../common/src/DWPBase/DwpInput.cpp
    ../common/src/DWPBase/DwpOutput.cpp
    ../common/src/DWPBase/DwpItem.cpp
    ../common/src/DWPBase/DwpLoad.cpp
    ../common/src/DWPType/DwpArray.cpp
    ../common/src/DWPType/DwpDatatype.cpp
    ../common/src/DWPType/DwpDataUnion.cpp
    ../common/src/DWPType/DwpEnum.cpp
    ../common/src/DWPType/DwpEnumDef.cpp
    ../common/src/DWPType/DwpFloat.cpp
    ../common/src/DWPType/DwpFloat3.cpp
    ../common/src/DWPType/DwpFloat4.cpp
    ../common/src/DWPType/DwpFloatArray.cpp
    ../common/src/DWPType/DwpInt.cpp
    ../common/src/DWPType/DwpIntArray.cpp
    ../common/src/DWPType/DwpMatrix34.cpp
    ../common/src/DWPType/DwpRotation.cpp
    ../common/src/DWPType/DwpScalar.cpp
    ../common/src/DWPType/DwpScalarArray.cpp
    ../common/src/DWPType/DwpString.cpp
    ../common/src/DWPType/DwpTransform.cpp
    ../common/src/DWPType/DwpVector2.cpp
    ../common/src/DWPType/DwpVector3.cpp
    ../common/src/DWPType/DwpVector3Array.cpp
    ../common/src/DWPType/DwpVector4.cpp
    ../common/src/DWPType/DwpVoidStarType.cpp
    ../common/src/DWPModel/DwpActor.cpp
    ../common/src/DWPModel/DwpActorDef.cpp
    ../common/src/DWPModel/DwpActorSet.cpp
    ../common/src/DWPModel/DwpActorTemplate.cpp
    ../common/src/DWPModel/DwpBoot.cpp
    ../common/src/DWPModel/DwpCast.cpp
    ../common/src/DWPModel/DwpCastRef.cpp
    ../common/src/DWPModel/DwpChecker.cpp
    ../common/src/DWPModel/DwpConverter.cpp
    ../common/src/DWPModel/DwpDelegateAttachment.cpp
    ../common/src/DWPModel/DwpDelegateBinding.cpp
    ../common/src/DWPModel/DwpDelegateDef.cpp
    ../common/src/DWPModel/DwpDelegateForumMapping.cpp
    ../common/src/DWPModel/DwpFiletype.cpp
    ../common/src/DWPModel/DwpForum.cpp
    ../common/src/DWPModel/DwpForumDef.cpp
    ../common/src/DWPModel/DwpGroup.cpp
    ../common/src/DWPModel/DwpGroupRef.cpp
    ../common/src/DWPModel/DwpHeaderFile.cpp
    ../common/src/DWPModel/DwpLimits.cpp
    ../common/src/DWPModel/DwpMedia.cpp
    ../common/src/DWPModel/DwpMediaRef.cpp
    ../common/src/DWPModel/DwpMediaRefSource.cpp
    ../common/src/DWPModel/DwpMediaRefTarget.cpp
    ../common/src/DWPModel/DwpMediaRefClass.cpp
    ../common/src/DWPModel/DwpPackage.cpp
    ../common/src/DWPModel/DwpProperty.cpp
    ../common/src/DWPModel/DwpPropertyDef.cpp
    ../common/src/DWPModel/DwpRoleAttachment.cpp
    ../common/src/DWPModel/DwpRoleBinding.cpp
    ../common/src/DWPModel/DwpRoleDef.cpp
    ../common/src/DWPModel/DwpRoleSetMapping.cpp
    ../common/src/DWPModel/DwpScene.cpp
    ../common/src/DWPModel/DwpSet.cpp
    ../common/src/DWPModel/DwpSetDef.cpp
    ../common/src/DWPModel/DwpSourceFile.cpp
    ../common/src/DWPModel/DwpStage.cpp
    ../common/src/DWPModel/DwpStageDef.cpp)

# Specify the static library
add_library(
  dwpStatic STATIC
    ../common/src/DWP/Dwp.cpp
    ../common/src/DWPBase/DwpDict.cpp
    ../common/src/DWPBase/DwpStrKeyDict.cpp
    ../common/src/DWPBase/DwpMonitor.cpp
    ../common/src/DWPBase/DwpDiscriminator.cpp
    ../common/src/DWPBase/DwpTagDiscriminator.cpp
    ../common/src/DWPBase/DwpTagAllDiscriminator.cpp
    ../common/src/DWPBase/DwpNotifier.cpp
    ../common/src/DWPBase/DwpContainer.cpp
    ../common/src/DWPBase/DwpInclude.cpp
    ../common/src/DWPBase/DwpFile.cpp
    ../common/src/DWPBase/DwpFilename.cpp
    ../common/src/DWPBase/DwpDSOFile.cpp
    ../common/src/DWPBase/DwpUnknown.cpp
    ../common/src/DWPBase/DwpFinder.cpp
    ../common/src/DWPBase/DwpInput.cpp
    ../common/src/DWPBase/DwpOutput.cpp
    ../common/src/DWPBase/DwpItem.cpp
    ../common/src/DWPBase/DwpLoad.cpp
    ../common/src/DWPType/DwpArray.cpp
    ../common/src/DWPType/DwpDatatype.cpp
    ../common/src/DWPType/DwpDataUnion.cpp
    ../common/src/DWPType/DwpEnum.cpp
    ../common/src/DWPType/DwpEnumDef.cpp
    ../common/src/DWPType/DwpFloat.cpp
    ../common/src/DWPType/DwpFloat3.cpp
    ../common/src/DWPType/DwpFloat4.cpp
    ../common/src/DWPType/DwpFloatArray.cpp
    ../common/src/DWPType/DwpInt.cpp
    ../common/src/DWPType/DwpIntArray.cpp
    ../common/src/DWPType/DwpMatrix34.cpp
    ../common/src/DWPType/DwpRotation.cpp
    ../common/src/DWPType/DwpScalar.cpp
    ../common/src/DWPType/DwpScalarArray.cpp
    ../common/src/DWPType/DwpString.cpp
    ../common/src/DWPType/DwpTransform.cpp
    ../common/src/DWPType/DwpVector2.cpp
    ../common/src/DWPType/DwpVector3.cpp
    ../common/src/DWPType/DwpVector3Array.cpp
    ../common/src/DWPType/DwpVector4.cpp
    ../common/src/DWPType/DwpVoidStarType.cpp
    ../common/src/DWPModel/DwpActor.cpp
    ../common/src/DWPModel/DwpActorDef.cpp
    ../common/src/DWPModel/DwpActorSet.cpp
    ../common/src/DWPModel/DwpActorTemplate.cpp
    ../common/src/DWPModel/DwpBoot.cpp
    ../common/src/DWPModel/DwpCast.cpp
    ../common/src/DWPModel/DwpCastRef.cpp
    ../common/src/DWPModel/DwpChecker.cpp
    ../common/src/DWPModel/DwpConverter.cpp
    ../common/src/DWPModel/DwpDelegateAttachment.cpp
    ../common/src/DWPModel/DwpDelegateBinding.cpp
    ../common/src/DWPModel/DwpDelegateDef.cpp
    ../common/src/DWPModel/DwpDelegateForumMapping.cpp
    ../common/src/DWPModel/DwpFiletype.cpp
    ../common/src/DWPModel/DwpForum.cpp
    ../common/src/DWPModel/DwpForumDef.cpp
    ../common/src/DWPModel/DwpGroup.cpp
    ../common/src/DWPModel/DwpGroupRef.cpp
    ../common/src/DWPModel/DwpHeaderFile.cpp
    ../common/src/DWPModel/DwpLimits.cpp
    ../common/src/DWPModel/DwpMedia.cpp
    ../common/src/DWPModel/DwpMediaRef.cpp
    ../common/src/DWPModel/DwpMediaRefSource.cpp
    ../common/src/DWPModel/DwpMediaRefTarget.cpp
    ../common/src/DWPModel/DwpMediaRefClass.cpp
    ../common/src/DWPModel/DwpPackage.cpp
    ../common/src/DWPModel/DwpProperty.cpp
    ../common/src/DWPModel/DwpPropertyDef.cpp
    ../common/src/DWPModel/DwpRoleAttachment.cpp
    ../common/src/DWPModel/DwpRoleBinding.cpp
    ../common/src/DWPModel/DwpRoleDef.cpp
    ../common/src/DWPModel/DwpRoleSetMapping.cpp
    ../common/src/DWPModel/DwpScene.cpp
    ../common/src/DWPModel/DwpSet.cpp
    ../common/src/DWPModel/DwpSetDef.cpp
    ../common/src/DWPModel/DwpSourceFile.cpp
    ../common/src/DWPModel/DwpStage.cpp
    ../common/src/DWPModel/DwpStageDef.cpp)

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
      ../common/include/mle/playprnt.h
      ../common/include/mle/ppinput.h
      ../common/include/mle/pp.h
      ../common/include/mle/chunk.h
     DESTINATION
      include/mle)

  # Uninstall libraries and header files
  add_custom_target("uninstall" COMMENT "Uninstall installed files")
  add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!)
