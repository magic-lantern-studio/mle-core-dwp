/** @defgroup MleDWPDso Magic Lantern Digital Workprint Library API - DSO */

/**
 * @file DwpDso.h
 * @ingroup MleDWPDso
 *
 * @author Mark S. Millard
 * @date February 6, 2004
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

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DWPDSO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DWPDSO_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef DWPDSO_EXPORTS
#define DWPDSO_API __declspec(dllexport)
#else
#define DWPDSO_API __declspec(dllimport)
#endif

// This class is exported from the DwpDso.dll
class DWPDSO_API CDwpDso
{
  public:
	CDwpDso(void);
	// TODO: add your methods here.
};

extern DWPDSO_API int nDwpDso;

DWPDSO_API int fnDwpDso(void);

