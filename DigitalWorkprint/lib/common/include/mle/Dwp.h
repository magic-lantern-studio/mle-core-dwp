/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file Dwp.h
 * @ingroup MleDWPAccess
 *
 * This file contains the definitions for initializing the Digital Workprint
 * library.
 */


// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2022 Wizzer Works
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

//#define DEBUG_MEMORY_FAILURE 1

//
// The memory override calls.
//
#ifdef DEBUG_MEMORY_FAILURE

#include "mle/DwpMemoryManager.h"

#endif /* DEBUG_MEMORY_FAILURE */

#if defined(__linux__)
#if defined(HAVE_LOG4CXX)
#include "log4cxx/logger.h"

using namespace log4cxx;
extern LoggerPtr g_dwpLogger;;
#endif /* HAVE_LOG4CXX */
#endif

/**
 * @brief Initialize Digital Workprint library.
 *
 * This function initializes Magic Lantern system types.  It is possible
 * to link these in on demand using a DSO, but since they will be
 * commonly used it is simpler not to use that mechanism.
 */
MLE_DWP_API void mleDwpInit(void);

#endif /* __MLE_DWP_H_ */


