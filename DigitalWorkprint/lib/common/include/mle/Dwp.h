/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file Dwp.h
 * @ingroup MleDWPAccess
 *
 * This file contains the definitions for initializing the Digital Workprint
 * library.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 *
 * \mainpage Magic Lantern Digital Workprint Library API
 *
 * \section overview Overview
 * <p>
 * The Magic Lantern Digital Workprint Library is a component of Magic Lantern,
 * an interactive, title development platform for authoring highly interactive titles,
 * such as interactive TV applications, games, educational programs, and
 * point-of-information/sales kiosks. The DWP library provides utilities for
 * constructing and maintaining a Digital Workprint.
 * </p>
 * <p>
 * The Magic Lantern DWP Library is distributed as two static libraries and two
 * dynamic link libraries. The build environment distributed with the source for the
 * DWP library can be used to build any or all of these libraries.
 * The libraries are
 * <ul>
 * <li>tools/DWP.lib       - Release Static Library</li>
 * <li>tools/DWPd.lib      - Debug Static Library</li>
 * <li>rehearsal/DWP.dll   - Release Dynamic Link Library</li>
 * <li>rehearsal/DWPd.dll  - Debug Dynamic Link Library</li>
 * </ul>
 * </p>
 * <p>
 * The tools version of the libraries are used with Magic Lantern
 * tools and can be found under <b>MLE_HOME/lib/tools</b>. The rehearsal version
 * of the libraries are distributed as a DLL/DSO and are used with the Magic Lantern
 * Rehearsal Player. They can be found under <b>MLE_HOME/bin/rehearsal</b>.
 * </p>
 *
 * \section ms_build Building for Microsoft Windows
 * <p>
 * On Microsoft Windows platforms, one of these defines must always be set when
 * building application programs:
 * <ul>
 * <li><b>MLE_DLL</b>, when the application programmer is using the
 *     library in the form of a dynamic link library (DLL)
 * </li>
 * <li><b>MLE_NOT_DLL</b>, when the application programmer is using the
 *     library in the form of a static object library (LIB)
 * </ul></p>
 * <p>
 * Note that either MLE_DLL or MLE_NOT_DLL <b><i>must</i></b> be defined by
 * the application programmer on MSWindows platforms, or else the
 * #error statement will be processed (see DWP.h). Set up one or the other of these two
 * defines in your compiler environment according to how the library
 * was built -- as a DLL (use "MLE_DLL") or as a LIB (use
 * "MLE_NOT_DLL").
 * </p><p>
 * Setting up defines for the compiler is typically done by either
 * adding something like "/DMLE_DLL" to the compiler's argument
 * line (for command-line build processes), or by adding the define to
 * the list of preprocessor symbols in your IDE GUI (in the MSVC IDE,
 * this is done from the "Project->Settings" menu; choose the "C/C++"
 * tab, then "Preprocessor" from the dropdown box and add the
 * appropriate define)).
 * </p><p>
 * It is extremely important that the application programmer uses the
 * correct define, as using "MLE_NOT_DLL" when "MLE_DLL" is
 * correct is likely to cause mysterious crashes.
 * </p>
 *
 * \section linux_build Building for Linux
 * <p>
 * Instructions for building on this platform are forthcoming.
 * </p>
 */


// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_DWP_H_
#define __MLE_DWP_H_

//
// DwpPlatformDefs.h defines the PLATFORM_<foobar> name for each
// platform should there arise a need for platform specific code
// in the common tree, which there won't.
//
#include "mle/DwpPlatformDefs.h"

//
// DwpPlatformTypes.h should be defined in each platforms
// include/mle directory.  It includes type information
// necessary to bring that platform up to the common level.
//
#include "mle/DwpPlatformTypes.h"

//
// DwpCommonDefs.h defines a number of nice macros and other definitions
// useful for code across all platforms.
//
#include "mle/DwpCommonDefs.h"

//
// DwpCommonTypes.h defines a number of common types.
//
#include "mle/DwpCommonTypes.h"

/* Precaution to avoid an error easily made by the application programmer. */
#ifdef MLE_DWP_API
#error Leave the internal MLE_DWP_API define alone.
#endif /* MLE_DWP_API */

// Magic Lantern Digital Workprint export rules.
# ifdef MLE_INTERNAL
#  ifdef MLE_MAKE_DLL
#   define MLE_DWP_API MLE_DWP_EXPORT
#  endif /* MLE_MAKE_DLL */
# else /* !MLE_INTERNAL */
#  ifdef MLE_DLL
#   define MLE_DWP_API MLE_DWP_IMPORT
#  else /* !MLE_DLL */
#   ifndef MLE_NOT_DLL
#    error Define either MLE_DLL or MLE_NOT_DLL as appropriate for your linkage! See mle/Dwp.h for further instructions.
#   endif /* MLE_NOT_DLL */
#  endif /* !MLE_DLL */
# endif /* !MLE_INTERNAL */

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef MLE_DWP_API
# define MLE_DWP_API
#endif /* !MLE_DWP_API */

//
// The MLE_ASSERT macro replacement for assert().
//
//#include "mle/mlAssert.h"

//
// The MLE_TRACE macro/function.
//
//#include "mle/mlTrace.h"

//#define DEBUG_MEMORY_FAILURE 1

//
// The memory override calls.
//
#ifdef DEBUG_MEMORY_FAILURE

#include "mle/DwpMemoryManager.h"

#endif /* DEBUG_MEMORY_FAILURE */

/**
 * @brief Initialize Digital Workprint library.
 *
 * This function initializes Magic Lantern system types.  It is possible
 * to link these in on demand using a DSO, but since they will be
 * commonly used it is simpler not to use that mechanism.
 */
MLE_DWP_API void mleDwpInit(void);

#endif /* __MLE_DWP_H_ */


