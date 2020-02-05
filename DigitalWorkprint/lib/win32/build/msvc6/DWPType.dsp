# Microsoft Developer Studio Project File - Name="DWPType" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DWPTYPE - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DWPType.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DWPType.mak" CFG="DWPTYPE - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DWPType - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DWPType - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DWPType - Win32 Release"

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
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\tools	copy Release\DWPType.lib $(MLE_HOME)\lib\tools\DWPType.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DWPType - Win32 Debug"

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
# ADD LIB32 /nologo /out:"Debug\DWPTyped.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\tools	copy Debug\DWPTyped.lib $(MLE_HOME)\lib\tools\DWPTyped.lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "DWPType - Win32 Release"
# Name "DWPType - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpDatatype.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpDataUnion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpEnum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpEnumDef.cpp
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

SOURCE=..\..\..\common\src\DWPType\DwpInt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpIntArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpMatrix34.cpp
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

SOURCE=..\..\..\common\src\DWPType\DwpString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\DWPType\DwpTransform.cpp
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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDatatype.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpDataUnion.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpEnum.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpEnumDef.h
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

SOURCE=..\..\..\common\include\mle\DwpInt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpIntArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpMatrix34.h
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

SOURCE=..\..\..\common\include\mle\DwpString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\DwpTransform.h
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
# End Group
# End Target
# End Project
