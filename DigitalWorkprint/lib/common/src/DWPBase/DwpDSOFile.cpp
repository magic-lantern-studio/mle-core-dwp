/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpDSOFile.h
 * @ingroup MleDWPAccess
 *
 * This file implements the DSO File workprint item.
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/MleDsoLoader.h"

// Include Digital Workprint header files.
#include "mle/DwpDSOFile.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"


MLE_DWP_SOURCE(MleDwpDSOFile,MleDwpItem);

int MleDwpDSOFile::g_loadOnSet = 0;

MleDwpDSOFile::MleDwpDSOFile(void)
{
	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}

MleDwpDSOFile::~MleDwpDSOFile(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);
}

int
MleDwpDSOFile::readContents(MleDwpInput *in)
{
	if ( in->readFilename(&m_dsofile) )
	{
		in->reportError("error reading dso file");
		in->skip();
		return 1;
	}

	// Load the DSO file as a side effect.
	if ( g_loadOnSet )
		load();

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);

	return 0;
}

int
MleDwpDSOFile::writeContents(MleDwpOutput *out)
{
	out->writeFilename(&m_dsofile);
	return 0;
}

void
MleDwpDSOFile::setDSOFile(const char *f)
{
	m_dsofile.setName(f);

	// Load the DSO file as a side effect
	if ( g_loadOnSet && f )
		load();

	// Call the modification notification callbacks for this dwp item.
	notify(MleDwpChangeNotification);
}

const char *
MleDwpDSOFile::getDSOFile(void) const
{
	return m_dsofile.getName();
}

MleDwpItem *
MleDwpDSOFile::copy(int copyChildren,MleDwpItem *parent)
{
	// Call the base class copy.
	MleDwpItem *rval = MleDwpItem::copy(copyChildren,parent);

	// If the filename has a path, then set the path on the copy.
	if ( m_dsofile.getPath() )
		((MleDwpDSOFile *)rval)->m_dsofile.setPath(m_dsofile.getPath());
	
	return rval;
}

void
MleDwpDSOFile::load(void)
{
    const char *filepath = m_dsofile.getPath() ? m_dsofile.getPath() : m_dsofile.getName();
    char dso_file[1028];
#if defined(__linux__)
    sprintf(dso_file,"%s.so",filepath);
#endif /* __linux__ */
#if defined(__APPLE__)
    sprintf(dso_file,"%s.dylib",filepath);
#endif /* __APPLE__ */
#if defined(WIN32)
#ifdef MLE_DEBUG
    sprintf(dso_file,"%sd.dll",filepath);
#else
    sprintf(dso_file,"%s.dll",filepath);
#endif /* ! MLE_DEBUG */
#endif /* WIN32 */

    // Load the DSO file.
    void *handle = MleDSOLoader::loadFile(dso_file);
    if ( handle == NULL )
        printf("MleDwpDSOFile: Error loading DSO %s.\n",getDSOFile());
}

void
MleDwpDSOFile::setLoadOnSet(int flag)
{
	g_loadOnSet = flag;
}

void *
MleDwpDSOFile::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpDSOFile::operator delete(void *p)
{
	mlFree(p);
}
