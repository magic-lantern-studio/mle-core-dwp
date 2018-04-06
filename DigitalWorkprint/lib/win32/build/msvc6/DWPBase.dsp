# Microsoft Developer Studio Project File - Name="DWPBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DWPBASE - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DWPBase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DWPBase.mak" CFG="DWPBASE - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DWPBase - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DWPBase - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DWPBase - Win32 Release"

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
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\tools	copy Release\DWPBase.lib $(MLE_HOME)\lib\tools\DWPBase.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DWPBase - Win32 Debug"

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
# ADD LIB32 /nologo /out:"Debug\DWPBased.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\tools	copy Debug\DWPBased.lib $(MLE_HOME)\lib\tools\DWPBased.lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "DWPBase - Win32 Release"
# Name "DWPBase - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpContainer.cpp
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

SOURCE=..\..\..\common\src\DWPBase\DwpFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpFilename.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpFinder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpInclude.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpItem.cpp
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

SOURCE=..\..\..\common\src\DWPBase\DwpStrKeyDict.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpTagAllDiscriminator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpTagDiscriminator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPBase\DwpUnknown.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\Dwp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpContainer.h
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

SOURCE=..\..\..\common\include\mle\DwpFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFilename.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpFinder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpInclude.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpInput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpItem.h
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

SOURCE=..\..\..\common\include\mle\DwpStrKeyDict.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpTagAllDiscriminator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpTagDiscriminator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpUnknown.h
# End Source File
# End Group
# End Target
# End Project
