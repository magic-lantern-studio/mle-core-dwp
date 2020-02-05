# Microsoft Developer Studio Project File - Name="DwpDso" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DwpDso - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DwpDso.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DwpDso.mak" CFG="DwpDso - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DwpDso - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DwpDso - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DwpDso - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DWPDSO_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../common/include" /I "../../include" /I "$(MLE_HOME)/Core/util/common/include" /I "$(MLE_HOME)/Core/util/win32/include" /I "$(MLE_HOME)/Core/math/common/include" /I "$(MLE_HOME)/Core/math/win32/include" /D "ML_CDECL" /D "MLE_INTERNAL" /D "MLE_MAKE_DLL" /D "MLE_DLL" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DWPDSO_EXPORTS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 mlmath.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib psapi.lib /nologo /dll /map /machine:I386 /out:"Release/DWP.dll" /libpath:"$(MLE_HOME)\lib\rehearsal" /libpath:"$(MLE_HOME)\Core\util\win32\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\rehearsal	copy Release\DWP.lib $(MLE_HOME)\lib\rehearsal\DWP.lib	mkdir $(MLE_HOME)\bin\rehearsal	copy Release\DWP.dll $(MLE_HOME)\bin\rehearsal\DWP.dll	mkdir $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\DigitalWorkprint\lib\common\include\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\DigitalWorkprint\lib\win32\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DwpDso - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DWPDSO_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../common/include" /I "../../include" /I "$(MLE_HOME)/Core/util/common/include" /I "$(MLE_HOME)/Core/util/win32/include" /I "$(MLE_HOME)/Core/math/common/include" /I "$(MLE_HOME)/Core/math/win32/include" /D "MLE_DEBUG" /D "ML_CDECL" /D "MLE_INTERNAL" /D "MLE_MAKE_DLL" /D "MLE_DLL" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DWPDSO_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mlmathd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib psapi.lib /nologo /dll /map /debug /machine:I386 /out:"Debug/DWPd.dll" /pdbtype:sept /libpath:"$(MLE_HOME)\lib\rehearsal" /libpath:"$(MLE_HOME)\Core\util\win32\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\rehearsal	copy Debug\DWPd.lib $(MLE_HOME)\lib\rehearsal\DWPd.lib	mkdir $(MLE_HOME)\bin\rehearsal	copy Debug\DWPd.dll $(MLE_HOME)\bin\rehearsal\DWPd.dll	mkdir $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\DigitalWorkprint\lib\common\include\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\DigitalWorkprint\lib\win32\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "DwpDso - Win32 Release"
# Name "DwpDso - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\common\src\DWP\Dwp.cpp
# End Source File
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

SOURCE=..\..\..\common\src\DWPType\DwpArray.cpp
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

SOURCE=..\..\..\common\src\DWPBase\DwpContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpDatatype.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpDataUnion.cpp
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

SOURCE=..\..\..\common\src\DWPBase\DwpDict.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpDiscriminator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpDSOFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpEnum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpEnumDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpFilename.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpFiletype.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpFinder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpFloat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpFloat3.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpFloat4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpFloatArray.cpp
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

SOURCE=..\..\..\common\src\DWPBase\DwpInclude.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpInt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpIntArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPModel\DwpLimits.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpMatrix34.cpp
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

SOURCE=..\..\..\common\src\DWPBase\DwpMonitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpNotifier.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpOutput.cpp
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

SOURCE=..\..\..\common\src\DWPType\DwpRotation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpScalar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpScalarArray.cpp
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
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpStrKeyDict.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpTagAllDiscriminator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpTagDiscriminator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpUnknown.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpVector2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpVector3.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpVector3Array.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpVector4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpVoidStarType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Core\util\common\src\MleDsoLoader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Core\util\common\src\MleMemoryManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Core\util\win32\src\MleWin32MemoryManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Core\util\win32\src\MleWin32Path.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Core\util\common\src\mlExpandFilename.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Core\util\common\src\mlUnique.c
# End Source File
# Begin Source File

SOURCE=..\..\src\DwpDso\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\Dwp.h
# End Source File
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

SOURCE=..\..\..\common\include\mle\DwpArray.h
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

SOURCE=..\..\..\common\include\mle\DwpCommonDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpCommonTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpConverter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDatatype.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDataUnion.h
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

SOURCE=..\..\..\common\include\mle\DwpDict.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDiscriminator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDSOFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpEnum.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpEnumDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFilename.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFiletype.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFinder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFloat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFloat3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFloat4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFloatArray.h
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

SOURCE=..\..\..\common\include\mle\DwpInclude.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpInput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpInt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpIntArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpLimits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpMatrix34.h
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

SOURCE=..\..\..\common\include\mle\DwpMonitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpNotifier.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpOutput.h
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

SOURCE=..\..\..\common\include\mle\DwpRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpScalar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpScalarArray.h
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
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpStrKeyDict.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpTagAllDiscriminator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpTagDiscriminator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpTblMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpUnknown.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpVector2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpVector3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpVector3Array.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpVector4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpVoidStarType.h
# End Source File
# Begin Source File

SOURCE=..\..\src\DwpDso\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\src\DwpDso\ReadMe.txt
# End Source File
# End Target
# End Project
