/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpUnknown.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the unknown workprint item used by the Magic Lantern Digital
 * Workprint Library API.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2025 Wizzer Works
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
#include "mle/DwpUnknown.h"
#include "mle/DwpInput.h"
#include "mle/DwpOutput.h"
#include "mle/mlMalloc.h"


MleDwpUnknown::MleDwpUnknown(const char *t)
{
	printf("substituting Unknown item for %s.\n",t);
	m_type = strdup(t);

	m_allocSize = 16;
	m_buffer = (char *)mlMalloc(m_allocSize);
	m_size = 0;

	// Call the creation notification callbacks for this type of dwp item.
	notify(MleDwpCreateNotification);
}


MleDwpUnknown::~MleDwpUnknown(void)
{
    // Call the notification callbacks for this specific dwp item.
    m_notifier.notify(this, this, MleDwpDeleteNotification);

	// Call the notification callbacks for those who registered
	// general interest on all wp items (NULL key).
	m_notifier.notify(NULL, this, MleDwpDeleteNotification);

	mlFree(m_type);
	mlFree(m_buffer);
}


int
MleDwpUnknown::readContents(MleDwpInput *in)
{
	int paren = 0;
	int comment = 0;

	while ( 1 )
	{
		int c = in->getNextByte();

		switch ( c )
		{
			case 0:		// EOF
				in->reportError("unexpected end of file");
				return 1;
			case '#':	// comment
				comment = 1;
				break;
			case '\n':
				comment = 0;
				break;
			case '(':
				if ( comment == 0 )
					paren++;
				break;
			case ')':
				if ( comment == 0 )
					paren--;
				
				if ( paren < 0 )
				{
					in->putBackByte(c);

					// Call the modification notification
					// callbacks for this dwp item.
					notify(MleDwpChangeNotification);

					return 0;
				}
				break;
		}

		/* Add to buffer. */
		if ( m_size == m_allocSize )
		{
			m_allocSize *= 2;
			m_buffer = (char *)mlRealloc(m_buffer,m_allocSize);
		}

		m_buffer[m_size++] = c;
	}
}


int
MleDwpUnknown::write(MleDwpOutput *out,int /* writeChildren */)
{
	/* Write the opening paren and item type. */
	out->writeString("(");
	out->writeString(m_type);

	/* Write contents. */
	out->adjustIndent(1);
	for ( int i = 0; i < m_size; i++ )
		out->writeChar(m_buffer[i]);

	/* Write the closing paren. */
	out->adjustIndent(-1);
	out->writeString(")");
	out->writeTerminal();

	return 0;
}

void *
MleDwpUnknown::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleDwpUnknown::operator delete(void *p)
{
	mlFree(p);
}

void*
MleDwpUnknown::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleDwpUnknown::operator delete[](void* p)
{
	mlFree(p);
}
