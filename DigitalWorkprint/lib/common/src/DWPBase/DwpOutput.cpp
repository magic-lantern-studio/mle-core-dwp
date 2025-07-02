/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpOutput.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the output object used by the Magic Lantern Digital
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
#include <ctype.h>

// Include Magic Lantern header files.
#include <mle/DwpOutput.h>
#include <mle/DwpFilename.h>
#include <mle/mlAssert.h>
#include <mle/mlFileio.h>
#include <mle/mlMalloc.h>
#include <mle/mlExpandFilename.h>


MleDwpOutput::MleDwpOutput(int magicFlag)
{
	fp = NULL;
	closeFlag = 0;

	expandFilenames = 0;

	buffer = NULL;
	bufSize = 0;
	bufUsed = 0;
	reallocFunc = NULL;
	reallocData = NULL;

	lineno = 0;
	ilevel = 0;
	newline = 1;

	setMagic(magicFlag);

#if defined(MLE_DEBUG)
	bufGetFlag = 0;
#endif
}


MleDwpOutput::~MleDwpOutput()
{
	if ( closeFlag )
		mlFClose(fp);

#if defined(MLE_DEBUG)
	if ( buffer && bufGetFlag == 0 )
		printf("warning: output buffer never extracted from MleDwpOutput.\n");
#endif
}


void
MleDwpOutput::setMagic(int magicFlag)
{
	writeHeader = magicFlag;
}


// This function writes the magic type identification.
void
MleDwpOutput::writeMagic(void)
{
	if ( !writeHeader )
		return;

	char *magic = const_cast<char*>("#DWP 1.0 ascii");

	writeString(magic);
	writeTerminal();
}


/*
 * This function opens a file for output.
 */
int
MleDwpOutput::openFile(const char *filename)
{
	MLE_ASSERT(filename);

	/* expand the filename - env vars, tilde notation, etc. */
	char *expand = mlFilenameExpand((char *)filename);

	/* open the file */
	fp = mlFOpen(expand,"w");

	/* release the expanded filename */
	mlFree(expand);

	if ( fp )
	{
		writeMagic();
		closeFlag = 1;
		return 0;
	}
	else
		return 1;
}


/*
 * This function opens a file for output and appends all written
 * information to the end of the file's current contents.
 */
int
MleDwpOutput::appendFile(const char *filename)
{
	MLE_ASSERT(filename);

	/* expand the filename - env vars, tilde notation, etc. */
	char *expand = mlFilenameExpand((char *)filename);

	/* open the file */
	fp = mlFOpen(expand,"a");

	/* release the expanded filename */
	mlFree(expand);

	if ( fp )
	{
		closeFlag = 1;
		return 0;
	}
	else
		return 1;
}


/*
	This function directs output to a file pointer.
*/
void
MleDwpOutput::setFilePointer(FILE *ufp)
{
	MLE_ASSERT(ufp);

	fp = ufp;

	writeMagic();
}


/*
 * This function sets output to a buffer.  The arguments are the
 * initial buffer, the size of that buffer, a user reallocation
 * function, and user reallocation data.  If NULL is passed for
 * the user reallocation function and data, the initial buffer
 * is assumed to have been created with mlMalloc(), and realloc()
 * will be used.
 */
void
MleDwpOutput::setBuffer(char *b,int initSize,
	char *(*func)(char *,int,void *),void *data)
{
	/* make sure a buffer hasn't already been created */
	/*   This can happen without calling setBuffer() previously */
	/*   by writing to this object without specifying any output */
	MLE_ASSERT(buffer == NULL);

	/* set internal buffer management state from arguments */
	buffer = b;
	bufSize = initSize;
	reallocFunc = func;
	reallocData = data;

	/* reset other state */
	bufUsed = 0;

#if defined(MLE_DEBUG)
	bufGetFlag = 0;
#endif

	writeMagic();
}


/*
 * This function returns the buffer information, pointer and size.
 */
char *
MleDwpOutput::getBuffer(char **b,int *size)
{
	/* make sure that this is a memory buffer output object */
	MLE_ASSERT(fp == NULL);

	/* make sure the buffer is terminated */
	if ( bufUsed == bufSize )
		expandBuffer();
	buffer[bufUsed] = 0;

	/* don't set the values if the pointers are NULL */
	if ( b )	*b = buffer;
	if ( size )	*size = bufUsed + 1;	// add one for the terminator

#if defined(MLE_DEBUG)
	bufGetFlag = 1;
#endif

	/* return the pointer for convenience */
	return buffer;
}


/*
 * This function reallocates the buffer to double the present size,
 * or 1024 bytes if the current size is zero.
 */
void
MleDwpOutput::expandBuffer(void)
{
	/* make sure we are really using the memory buffer */
	MLE_ASSERT(fp == NULL);

	/* we don't assert for a buffer here, because realloc should */
	/*   do the right thing if the buffer is NULL coming in */

	// if the buffer is NULL, we'll want to output the magic
	int firstAllocation = (buffer == NULL);

	/* check for the case of bufSize = 0 */
	bufSize = bufSize ? 2*bufSize : 1024;

	/* use the user reallocation function if present */
	if ( reallocFunc )
		buffer = (*reallocFunc)(buffer,bufSize,reallocData);
	else
		buffer = (char *)mlRealloc(buffer,bufSize);
	
	/* verify that we actually have a buffer now */
	MLE_ASSERT(buffer);

	if ( firstAllocation )
		writeMagic();

#if defined(MLE_DEBUG)
	bufGetFlag = 0;
#endif
}


/*
 * This function writes a string and a trailing space to the output.
 */
int
MleDwpOutput::writeString(const char *s)
{
	MLE_ASSERT(s);

	/* indent if this is the first thing on a new line */
	if ( newline ) indent();

	// special case for empty string
	if ( *s == 0 )
	{
		if ( fp )
		{
			// write out an empty string to the file
			if ( fprintf(fp,"\"\" ") > 0 )
				return 0;
			else
				return 1;
		}
		else
		{
			// make sure the buffer is big enough
			while ( bufSize - bufUsed < 3 )
				expandBuffer();
			
			// copy the empty string
			bufUsed += sprintf(buffer + bufUsed,"\"\" ");
			return 0;
		}
	}

	/* do file output */
	if ( fp )
	{
		if ( fprintf(fp,"%s ",s) > 0 )
			return 0;
		else
			return 1;
	}
	else
	{
		/* make sure the buffer is big enough */
		/*   add one for the trailing space, one for the terminal */
		int strSize = strlen(s) + 2;
		while ( bufSize - bufUsed < strSize )
			expandBuffer();
		
		/* write to the buffer */
		bufUsed += sprintf(buffer + bufUsed,"%s ",s);

		return 0;
	}
}


/*
 * This function writes a fancy string and a trailing space to the
 * output.  A fancy string is one that contains embedded spaces or
 * special characters.
 * 
 * Currently the list of special characters is:
 * 
 * 	#	comment	(but not inside a token)
 * 	(	start item
 * 	)	end item
 * 	"	string delimiter
 * 	
 * If the string has embedded spaces, the entire string will be
 * enclosed in double quotes on output.  Any special characters
 * will be escaped with a backslash.
 */
int
MleDwpOutput::writeFancyString(const char *s)
{
	/* indent if this is the first thing on a new line */
	if ( newline ) indent();

	// special case for empty string
	if ( *s == 0 )
	{
		if ( fp )
		{
			// write out an empty string to the file
			if ( fprintf(fp,"\"\" ") > 0 )
				return 0;
			else
				return 1;
		}
		else
		{
			// make sure the buffer is big enough
			while ( bufSize - bufUsed < 3 )
				expandBuffer();
			
			// copy the empty string
			bufUsed += sprintf(buffer + bufUsed,"\"\" ");
			return 0;
		}
	}

	/* see if there is any embedded white space or special characters */
	/* in the string */
	int whiteSpace = 0;
	const char *cptr;
	for ( cptr = s; *cptr; cptr++ )
	{
		if ( isspace(*cptr) )
		{
			whiteSpace = 1;
			break;
		}
	}

// this macro writes a single character to the output,
//   checking if output to a file pointer or a buffer
#define PUTC(C) { \
	if ( fp ) putc(C,fp); \
	else { \
		if ( bufUsed == bufSize ) expandBuffer(); \
		buffer[bufUsed++] = C; \
	} \
}

	/* if there is white space, open with double quotes */
	if ( whiteSpace )
		PUTC('\"');

	/* loop over the characters */
	for ( cptr = s; *cptr; cptr++ )
	{
// this macro escapes the special character argument
//   the macro *cannot* be terminated with a semicolon
#define IF(C) \
	else if ( *cptr == C ) { \
		PUTC('\\'); \
		PUTC(*cptr); \
	}

		if ( 0 ) ;

		// put a macro instance here for every character to
		//   be escaped
		IF('(')
		IF(')')
		IF('\"')
		IF('\\')

		else
			PUTC(*cptr);
#undef IF
	}

	/* if there is white space, close with double quotes */
	if ( whiteSpace )
		PUTC('\"');
	
	/* add the trailing space */
	PUTC(' ');

	return 0;
}


/*
 * This function writes an int and a trailing space to the output.
 */
int
MleDwpOutput::writeInt(int d)
{
	/* indent if this is the first thing on a new line */
	if ( newline ) indent();

	if ( fp )
	{
		if ( fprintf(fp,"%d ",d) > 0 )
			return 0;
		else
			return 1;
	}
	else
	{
		/* longest possible 32-bit int is -2147483648 */
		while ( bufSize - bufUsed < 12 )
			expandBuffer();
		
		bufUsed += sprintf(buffer + bufUsed,"%d ",d);

		return 0;
	}
}


/*
 * This function writes a float and a trailing space to the output.
 */
int
MleDwpOutput::writeFloat(float f)
{
	/* indent if this is the first thing on a new line */
	if ( newline ) indent();

	if ( fp )
	{
		if ( fprintf(fp,"%g ",f) > 0 )
			return 0;
		else
			return 1;
	}
	else
	{
		/* longest possible 32-bit float is ? */
		while ( bufSize - bufUsed < 20 )
			expandBuffer();
		
		bufUsed += sprintf(buffer + bufUsed,"%g ",f);

		return 0;
	}
}


/*
 * This function writes a single char to the output.  Note that
 * no trailing space is written, unlike the write routines for
 * string, int, and float.
 */
int
MleDwpOutput::writeChar(char c)
{
	/* indent if this is the first thing on a new line */
	if ( newline ) indent();

	if ( fp )
	{
		if ( fprintf(fp,"%c",c) > 0 )
			return 0;
		else
			return 1;
	}
	else
	{
		if ( bufUsed == bufSize )
			expandBuffer();
		
		buffer[bufUsed++] = c;

		return 0;
	}
}


/*
 * This function writes a terminal to the output.  In practice,
 * this is a newline for ascii format.
 */
int
MleDwpOutput::writeTerminal(void)
{
	// output a new line
	if ( fp )
		fprintf(fp,"\n");	// output a new line
	else
	{
		if ( bufUsed == bufSize )
			expandBuffer();
		
		buffer[bufUsed++] = '\n';
	}

	newline = 1;		// set the new line flag
	lineno++;		// increment the number of lines

	return 0;
}


// This function writes a filename to the output.  It really just does a
// writeString(), except that which string (relative or absolute path)
// is configurable.
int
MleDwpOutput::writeFilename(MleDwpFilename *f)
{
	if ( expandFilenames )
		return writeFancyString(
			f->getPath() ? f->getPath() : f->getName());
	else
		return writeFancyString(f->getName());
}


// This function sets the filename expansion flag.
void
MleDwpOutput::setFilenameExpansion(int onOrOff)
{
	expandFilenames = onOrOff;
}


/*
 * This function changes the indention level by the indicated
 * amount (usually +1 or -1).
 */
int
MleDwpOutput::adjustIndent(int i)
{
	ilevel += i;

	return ilevel;
}


/*
 * This function outputs the appropriate amount of indention
 * space.
 */
void
MleDwpOutput::indent(void)
{
	if ( fp )
	{
		for ( int i = 0; i < ilevel; i++ )
			fprintf(fp,"\t");
	}
	else
	{
		while ( bufSize - bufUsed < ilevel + 1 )
			expandBuffer();
		
		for ( int i = 0; i < ilevel; i++ )
			bufUsed += sprintf(buffer + bufUsed,"\t");
	}

	newline = 0;
}


/*
 * This function flushes all buffered output to disk, where applicable.
 */
int
MleDwpOutput::flush(void)
{
	if (fp != NULL)
		return(fflush(fp));
	return(0);
}


void *
MleDwpOutput::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpOutput::operator delete(void *p)
{
	mlFree(p);
}


void*
MleDwpOutput::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}


void
MleDwpOutput::operator delete[](void* p)
{
	mlFree(p);
}
