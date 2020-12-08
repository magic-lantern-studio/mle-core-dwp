/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpLoad.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the loader used by the Magic Lantern Digital
 * Workprint Library API.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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
//  For information concerning this source code, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/Dwp.h"
#include "mle/DwpItem.h"
#include "mle/DwpDSOFile.h"
#include "mle/DwpSet.h"
#include "mle/DwpGroup.h"
#include "mle/DwpScene.h"
#include "mle/DwpMediaRef.h"
#include "mle/DwpFinder.h"
#include "mle/DwpLoad.h"

// _mlWorkprint is a global handle for the last loaded Digital Workprint.
static MleDwpItem *_mlWorkprint =  NULL;

MLE_DWP_API MleDwpItem *mlGetWorkprint()
{
	return _mlWorkprint;
}

MLE_DWP_API void mlSetWorkprint(MleDwpItem *dwp)
{
    _mlWorkprint = dwp;
}

MLE_DWP_API MleDwpItem *mlLoadWorkprint(const char *filename)
{
	MLE_ASSERT(filename);

	// Set the flag to load DSOs when they are specified.
	MleDwpDSOFile::setLoadOnSet(1);

	// Read in the workprint.
	_mlWorkprint = MleDwpItem::readAll(filename);

	return _mlWorkprint;
}
