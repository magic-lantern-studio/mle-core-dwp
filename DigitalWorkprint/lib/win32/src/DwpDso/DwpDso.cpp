/** @defgroup MleDWPDso Magic Lantern Digital Workprint Library API - DSO */

/**
 * @file DwpDso.cpp
 * @ingroup MleDWPDso
 *
 * Defines the entry point for the DLL application.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include header files.
#include "stdafx.h"
#include "DwpDso.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}


// This is an example of an exported variable
DWPDSO_API int nDwpDso=0;

// This is an example of an exported function.
DWPDSO_API int fnDwpDso(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see DwpDso.h for the class definition
CDwpDso::CDwpDso()
{ 
    return; 
}

