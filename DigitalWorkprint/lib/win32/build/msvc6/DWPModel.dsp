# Microsoft Developer Studio Project File - Name="DWPModel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DWPMODEL - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DWPModel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DWPModel.mak" CFG="DWPMODEL - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DWPModel - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DWPModel - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DWPModel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../common/include" /I "../../include" /I "$(MLE_HOME)/Core/util/common/include" /I "$(MLE_HOME)/Core/util/win32/include" /I "$(MLE_HOME)/Core/math/common/include" /I "$(MLE_HOME)/Core/math/win32/include" /D "ML_CDECL" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\tools	copy Release\DWPModel.lib $(MLE_HOME)\lib\tools\DWPModel.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DWPModel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../common/include" /I "../../include" /I "$(MLE_HOME)/Core/util/common/include" /I "$(MLE_HOME)/Core/util/win32/include" /I "$(MLE_HOME)/Core/math/common/include" /I "$(MLE_HOME)/Core/math/win32/include" /D "MLE_DEBUG" /D "ML_CDECL" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\DWPModeld.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\tools	copy Debug\DWPModeld.lib $(MLE_HOME)\lib\tools\DWPModeld.lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "DWPModel - Win32 Release"
# Name "DWPModel - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpActor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpActorDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpActorSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpActorTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpBoot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpCast.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpCastRef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpChecker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpDelegateAttachment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpDelegateBinding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpDelegateDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpDelegateForumMapping.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpFiletype.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpForum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpForumDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpGroupRef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpHeaderFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpLimits.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpMedia.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpMediaRef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpMediaRefClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpPackage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpPropertyDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpRoleAttachment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpRoleBinding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpRoleDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpRoleSetMapping.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpScene.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpSetDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpSourceFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpStage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpStageDef.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpActor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpActorDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpActorSet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpActorTemplate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpBoot.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpCast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpCastRef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpChecker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpConverter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDelegateAttachment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDelegateBinding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDelegateDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDelegateForumMapping.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFiletype.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpForum.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpForumDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpGroupRef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpHeaderFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpLimits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpMedia.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpMediaRef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpMediaRefClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpPackage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpProperty.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpPropertyDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpRoleAttachment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpRoleBinding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpRoleDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpRoleSetMapping.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpScene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpSet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpSetDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpSourceFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpStage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpStageDef.h
# End Source File
# End Group
# End Target
# End Project
