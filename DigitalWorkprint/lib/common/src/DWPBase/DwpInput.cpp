/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpInput.cpp
 * @ingroup MleDWPAccess
 *
 * This file implements the input object used by the Magic Lantern Digital
 * Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2018 Wizzer Works
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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

// Include Magic Lantern header files.
#include <mle/DwpInput.h>
#include <mle/DwpFilename.h>
#include <mle/mlAssert.h>
#include <mle/mlMalloc.h>
#include <mle/mlFileio.h>
#include <mle/mlExpandFilename.h>
#include <mle/MlePath.h>
#ifdef WIN32
#include <mle/MleWin32Path.h>
#else
#include <mle/MleLinuxPath.h>
#endif /* WIN32 */


/*
 * This is a helper class for MleDwpInput.  It is an element of a
 * stack (implemented as a list) that contains information about
 * the files opened by this input object.
 *
 * The constructor actually opens the file (with the destructor
 * closing it), searching for the right directory if necessary.
 * The policy is:
 *
 * 	1.  Expand any environment vars or tilde notation.
 * 	2.  If the name is an absolute path, use that.
 * 	3.  Else use the directories from previous opens.
 * 	4.  Else look in some well known places.
 */
struct MleDwpInputFile
{
	MleDwpInputFile(const char *filename,MleDwpInputFile *next);
	~MleDwpInputFile();

	//char *m_path; // This is the path used (disambiguating the filename).
	MlePath *m_path; // This is the path used (disambiguating the filename).

	int m_lineno; // This is the saved line number, not the current line number.
	FILE *m_fp;   // File pointer.

	MleDwpInputFile *m_next; // List linkage.

	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);
};


/*
 * This is the list of default directories to search.  Each entry
 * <b>must</b> include a trailing '/', and the list must be terminated
 * with a NULL.
 */
static const char *_mleDir[] = {
	"./",
#if defined(__sgi) || defined (__linux__)
	"$MLE_ROOT/usr/WizzerWorks/MagicLantern/parts/actors/",
#else
#if defined(WIN32)
	"//C/Program Files/WizzerWorks/MagicLantern/parts/actors/",
	"$MLE_ROOT/parts/actors/",
#endif
#endif
	NULL
};

#if 0
MleDwpInputFile::MleDwpInputFile(const char *f,MleDwpInputFile *n)
{
    m_fp = NULL;
	// Set the next pointer.
	m_next = n;

	// Initialize the path.
	m_path = NULL;

	// Expand the filename.
	char *expand = mlFilenameExpand((char *)f);
	int explen = strlen(expand);

	// See if this is an absolute path.
#ifdef __MWERKS__
	if ( *expand == ':' )
#else
	if ( *expand == '/' )
#endif
	{
		m_path = expand;
		m_fp = mlFOpen(m_path,"r");
		return;
	}

	// Otherwise, try the previous directory.
	MleDwpInputFile *elem = m_next;
	if ( elem )
	{
		// Allocate path.
		m_path = (char *)mlRealloc(m_path,strlen(elem->m_path) + explen + 1);

		// Construct the path .
		strcpy(m_path,elem->m_path);
		strcpy(strrchr(m_path,'/') + 1,expand);

		if ( m_fp = mlFOpen(m_path,"r") )
		{
			mlFree(expand);
			return;
		}
	}

	// Get rid of allocated path.
	if ( m_path )
		mlFree(m_path);

	// Otherwise, try system directories .
	int i = 0;
	char *tmpPath = NULL;
	while ( _mleDir[i] )
	{
		// Allocate path.
		tmpPath =
		    (char *)mlRealloc(tmpPath,strlen(_mleDir[i]) + explen + 1);

		// Construct the path.
		strcpy(tmpPath,_mleDir[i]);
		strcat(tmpPath,expand);

		// Expand again (directory may include environment vars).
		m_path = mlFilenameExpand(tmpPath);

		if ( m_fp = mlFOpen(m_path,"r") )
		{
			mlFree(tmpPath);
			mlFree(expand);
			return;
		}

		// Try the next one.
		i++;
	}
}
#else /* !0 */
MleDwpInputFile::MleDwpInputFile(const char *f,MleDwpInputFile *n)
{
	// Initialize the file pointer.
    m_fp = NULL;

	// Set the next pointer.
	m_next = n;

	// Expand the filename.
	char *expand = mlFilenameExpand((char *)f);
	int explen = strlen(expand);

	// Initialize the path. The input is expected to be in its canonical form.
#ifdef WIN32
	m_path = new MleWin32Path((MlChar *)expand,false);
#else 
	m_path = new MleLinuxPath((MlChar *)expand,false);
#endif /* WIN32 */

	MLE_ASSERT(m_path);

	// See if this is an absolute path.
    if (m_path->isAbsolutePath())
	{
		m_fp = mlFOpen((char *)m_path->getPlatformPath(),"r");
		mlFree(expand);
		return;
	}

	// Otherwise, try the previous directory.
	MleDwpInputFile *elem = m_next;
	if ( elem )
	{
		// Construct a new path from the previous one.
		char *tmpPath = (char *)mlMalloc(strlen((char *)elem->m_path->getPath()) + explen + 1);
		strcpy(tmpPath,(char *)elem->m_path->getPath());
		strcpy(strrchr(tmpPath,'/') + 1,expand);
		m_path->setPath((MlChar *)tmpPath);

        // Attempt to open the new path.
		if ( m_fp = mlFOpen((char *)m_path->getPlatformPath(),"r") )
		{
			mlFree(tmpPath);
			mlFree(expand);
			return;
		} else
		{
			mlFree(tmpPath);
		}
	}

	// Otherwise, try system directories.
	int i = 0;
	char *tmpPath = NULL;
	while ( _mleDir[i] )
	{
		// Allocate path.
		tmpPath =
		    (char *)mlRealloc(tmpPath,strlen(_mleDir[i]) + explen + 1);

		// Construct the path.
		strcpy(tmpPath,_mleDir[i]);
		strcat(tmpPath,expand);

		// Expand again (directory may include environment vars).
		char *tmpExpand = mlFilenameExpand(tmpPath);

		// Set the new path.
		m_path->setPath((MlChar *)tmpExpand);

		// Attempt to open the new path.
		if ( m_fp = mlFOpen((char *)m_path->getPlatformPath(),"r") )
		{
			mlFree(tmpExpand);
			mlFree(tmpPath);
			mlFree(expand);
			return;
		} else
		{
			mlFree(tmpExpand);
		}

		// Try the next one.
		i++;
	}

	// No path was successfully opened.
	if (expand) mlFree(expand);
	if (tmpPath) mlFree(tmpPath);
	delete m_path;
	m_path = NULL;
}
#endif /* 0 */


MleDwpInputFile::~MleDwpInputFile()
{
	// Free allocated memory.
	//mlFree(m_path);
	if (m_path)
		delete m_path;

	// Close file.
	if ( m_fp )
		mlFClose(m_fp);
}


MleDwpInput::MleDwpInput(int magicFlag)
{
	// Initialize variables.
	m_lineno = 1;
	m_lineIndex = 0;
	m_buffer = NULL;
	m_fp = NULL;
	m_stack = NULL;

	setMagic(magicFlag);
}


MleDwpInput::~MleDwpInput()
{
	// Get rid of the file stack.
	while ( m_stack )
	{
		MleDwpInputFile *tmp = m_stack;
		m_stack = m_stack->m_next;
		delete tmp;
	}
}


void
MleDwpInput::setMagic(int magicFlag)
{
	m_readHeader = magicFlag;
}


// This checks to see if we have a workprint file on the input
int
MleDwpInput::readMagic(void)
{
	if ( !m_readHeader )
		return 0;

	// Look for the magic start.
	int c;
	char *magic = "#DWP";
	while ( c = getNextByte() )
	{
		if ( c != *magic++ )
		{
			reportError("no magic");
			return 1;
		}
		
		if ( *magic == 0 )
			break;
	}

	// That much has confirmed that this is a workprint file.
	// The remainder of the line may contain additional information,
	// like version or ascii/binary.  For now, it is ignored.
	while ( (c = getNextByte()) && c != '\n' )
		;	// Do nothing and loop
	
	return 0;
}


/*
 * This function opens a file for input.  Calling this function
 * more than once creates a file stack.
 */
int
MleDwpInput::openFile(const char *filename)
{
	MLE_ASSERT(filename);

	// Create a new file on the stack.
	m_stack = new MleDwpInputFile(filename,m_stack);

	// Use a direct member variable to hold the file pointer.
	m_fp = m_stack->m_fp;

	// Remember the old line number in the stack.
	m_stack->m_lineno = m_lineno;

	// and set up state for a new file
	m_lineno = 1;
	m_lineIndex = 0;

	// Check for a successful file open.
	if ( m_fp )
	{
		// and if this is a real workprint file.
		if ( readMagic() )
		{
			closeFile();
			return 1;
		}
		else
			return 0;
	}
	else
	{
		closeFile();
		return 1;
	}
}


/*
 * This function pops a file off the file stack.  The file is
 * closed (if it was opened successfully).
 */
void
MleDwpInput::closeFile(void)
{
	MLE_ASSERT(m_stack);

	// Restore the previous line number.
	m_lineno = m_stack->m_lineno;

	// Do list deletion.
	MleDwpInputFile *tmp = m_stack;
	m_stack = m_stack->m_next;
	delete tmp;

	// Set the file pointer.
	if ( m_stack )
		m_fp = m_stack->m_fp;
	else
		m_fp = NULL;
}


/*
 * This function returns the path to the current file, if any.
 */
const char *
MleDwpInput::getPath(void) const
{
	// Make sure that there is a file stack *and* that the
	// top of the stack actually has a file pointer.  When
	// a file is failed to be opened there is still a stack
	// entry for it until closeFile() is called.

	if ( m_stack && m_stack->m_fp )
		return (char *)m_stack->m_path->getPath();
	else
		return NULL;
}


/*
 * This function uses the passed buffer as input.
 */
int
MleDwpInput::setBuffer(const char *b)
{
	MLE_ASSERT(b);

	// It is an error to set a buffer with current file input.
	MLE_ASSERT(m_stack == NULL && m_fp == NULL);

	// Set the buffer pointer.
	m_buffer = b;

	m_lineno = 1;
	m_lineIndex = 0;

	if ( readMagic() )
	{
		m_buffer = NULL;
		return 1;
	}
	else
		return 0;
}


/*
 * This function tries to read a string from the input.  A string
 * is delimited by white space or defined special characters unless
 * it is enclosed in double quotes.
 */
int
MleDwpInput::readString(char *s,int bufsize)
{
	MLE_ASSERT(s);

	return lex(s,bufsize);
}


// This function tries to read a filename from the input.
int
MleDwpInput::readFilename(MleDwpFilename *f)
{
	int status;

	// Declare a buffer to read into.
	char buffer[MLE_DWP_INPUT_BUFSIZE];

	// Read in a string.
	// Return the error code if this fails.
	if ( (status = readString(buffer,MLE_DWP_INPUT_BUFSIZE)) != 0 )
		return status;
	
	// We have a valid string, set it on the filename object.
	f->setName(buffer);

	// Fix path.
	//   This attempts to find the appropriate path to a file
	//   specification.  If the filename is already an absolute
	//   filename, then we don't need to do anything special.
	//   If the filename is a relative name, however, this
	//   code attempts to convert that into an absolute path
	//   by looking for it in the context of the top of the
	//   file stack.

	// XXX There is an incredible hack here because the filename
	//   in the Media item is being misused to contain multiple
	//   filenames separated by commas for scene graphs.  This
	//   hack parses the list to look for commas, independently
	//   expands each component, and puts them together to form
	//   the path.
#define COMMAHACK
#ifdef COMMAHACK
	// Rest to see if there are embedded commas.
	if ( strchr(buffer,',') )
	{
		// We can use buffer directly since we're done with
		// setting the name field.
		char *component = strtok(buffer,",");
		while ( component )
		{
			// Expand the filename.
			char *expand = mlFilenameExpand(component);

			// See if this is an absolute path or no current.
			// Use it directly if it is.
			if ( *expand == '/' || getPath() == NULL )
			{
				// Allocate space for the path.
				char *path = new char[
				    (f->getPath() ? strlen(f->getPath()) : 0) +
				    1 +		// For the comma.
				    strlen(expand) +
				    1];

				// Concatentate verbatim.
				if ( f->getPath() )
				{
					strcpy(path,f->getPath());
					strcat(path,",");
				}
				else
					*path = 0;

				strcat(path,expand);

				f->setPath(path);

				delete path;
			}
			else
			{
				// Allocate space for the path.
				char *path = new char[
				    (f->getPath() ? strlen(f->getPath()) : 0) +
				    1 +		// For the comma.
				    strlen(getPath()) + strlen(expand) +
				    1];
				
				// Put the current paths.
				if ( f->getPath() )
				{
					strcpy(path,f->getPath());
					strcat(path,",");
				}
				else
					*path = 0;

				// Remember where we are.
				char *filepath = path + strlen(path);

				// Add the path and the expanded name.
				strcat(path,getPath());
				strcpy(strrchr(path,'/') + 1,expand);

				// stat the file to see if it exists.
				struct stat sbuf;
				if ( stat(filepath,&sbuf) != 0 )
				{
					// There has been an error testing
					// the file.  Use the base name.
					strcpy(filepath,expand);
				}

				f->setPath(path);

				delete path;
			}

			mlFree(expand);
					
			component = strtok(NULL,",");
		}
	}
	else
	{
#endif /* COMMAHACK */

	// Expand the filename.
	char *expand = mlFilenameExpand(buffer);

	// See if this is an absolute path and
	// return success if it is.
	if ( *expand == '/' )
		return 0;	// Success.
	
	// Otherwise we'll look for it in the current directory.
	if ( getPath() == NULL )
		return 0;	// No current directory, just return.
	
	// Allocate space for the path.
	char *path = new char[strlen(getPath()) + strlen(expand) + 1];
	
	// Construct the path from the current file and the filename.
	strcpy(path,getPath());
	strcpy(strrchr(path,'/') + 1,expand);

	// stat the file to see if it exists.
	// If this doesn't succeed, we could issue a warning here.
	struct stat sbuf;
	if ( stat(path,&sbuf) == 0 )
		f->setPath(path);
	
	// Recover memory.
	mlFree(expand);
	delete path;

#ifdef COMMAHACK
	}
#endif // COMMAHACK

	return 0;  // Success.
}


/*
 * This function tries to read an int from the input.  It uses the
 * scanf %i format, so decimal, octal, or hex notation can be used.
 */
int
MleDwpInput::readInt(int *d)
{
	MLE_ASSERT(d);

	char buffer[64];
	char *ptr;

	// Read a string from the input ...
	if ( lex(buffer,64) )
		return 1;
	
	// ... and try to create an int out of it.
	*d = (int)strtol(buffer,&ptr,0);
	if ( ptr == buffer )
	{
		reportError("illegal conversion: bad int");
		return 1;
	}

	// Push unused characters back onto the input stream.
	int n = strlen(ptr);
	while ( n-- )
		putBackByte(ptr[n]);

	return 0;
}


/*
 * This function tries to read a float from the input.
 */
int
MleDwpInput::readFloat(float *f)
{
	MLE_ASSERT(f);

	char buffer[64];
	char *ptr;

	// Read a string from the input ...
	if ( lex(buffer,64) )
		return 1;
	
	// ... and try to create a float out of it.
	*f = (float)strtod(buffer,&ptr);
	if ( ptr == buffer )
	{
		reportError("illegal conversion: bad float");
		return 1;
	}

	// Push unused characters back onto the input stream.
	int n = strlen(ptr);
	while ( n-- )
		putBackByte(ptr[n]);

	return 0;
}


/*
 * This function tries to read a char from the input.  Leading
 * white space will be ignored.
 */
int
MleDwpInput::readChar(char *c)
{
	MLE_ASSERT(c);

	while ( 1 )
	{
		int x = getNextByte();

		if ( x == 0 )
			return 1;

		if ( isspace(x) )
			;
		else if ( x == '#' )
			skipComment();
		else
		{
			*c = x;
			return 0;
		}
	}
}


/*
 * This function reads bytes until a newline is encountered, and end
 * of input (null character) is enountered, or the end of the storage
 * buffer is reached.
 */
int
MleDwpInput::readLine(char *s, int bufsize)
{
	// Look for a newline or the end of the buffer.
	int c, count = 0;
	do {
		c = getNextByte();
		if (s != NULL) {
			*s++ = c;
		}
		count++; // Must be inside loop or will be wrong when c == '\n'.
	} while ( c != '\n' && c != '\r' && c != 0 && (count < bufsize || s == NULL));

	if (count == 0) {
		return(1);
	}

	// Terminate string.
	if (s != NULL && count < bufsize) {
		*s = 0;
	}
	
	// Put back the terminating character because we don't
	// want to read past the end of input.
	putBackByte(c);

	return(0);
}


/*
 * This function reads a sequence of elements from the input as
 * instructed by a table passed in and the number of elements.
 * The element types come from an enumeration (and thus cannot
 * support type extension) that include int, float, string, and
 * token.
 *
 * For int, float, and string types, the pointer passed in the
 * table for that element should point to storage for the read
 * object.  For the token type, the pointer should point to a
 * string that contains what the input stream *should* have.
 *
 * Zero is returned for any error encountered.
 */
int
MleDwpInput::readTable(const MleDwpInputTable *table)
{
	MLE_ASSERT(table);

	char buffer[MLE_DWP_INPUT_BUFSIZE];

	int i = 0;
	while ( 1 )
	{
		switch ( table[i].type )
		{
			case MleDwpInputEnd:
				return 0;
			case MleDwpInputToken:
				// Read a string and return on error.
				if ( readString(buffer) )
					return 1;
				
				// Check against the passed string.
				if ( strcmp(buffer,(char *)table[i].ptr) )
				{
					char err[2*MLE_DWP_INPUT_BUFSIZE];
					sprintf(err,
					  "bad token: expected \"%s\" found \"%s\"",
					  table[i].ptr,buffer);
					reportError(err);

					return 1;
				}
				break;
			case MleDwpInputInt:
				// Read an int and return on error.
				if ( readInt((int *)table[i].ptr) )
					return 1;
				break;
			case MleDwpInputFloat:
				// Read a float and return on error.
				if ( readFloat((float *)table[i].ptr) )
					return 1;
				break;
			case MleDwpInputString:
				// Read a string into the provided buffer.
				if ( readString((char *)table[i].ptr) )
					return 1;
		}

		// Go to the next one.
		i++;
	}
}


/*
 * This function attempts to skip to the end of a workprint item.
 * It is intended to try to continue after encountering an error.
 */
void
MleDwpInput::skip(void)
{
	while ( 1 )
	{
		char c;

		if ( readChar(&c) )
		{
			reportError("unexpected eof");
			break;
		}

		if ( c == '(' || c == ')' )
		{
			putBackByte(c);
			break;
		}
	}
}


/*
 * This function skips to the end of a comment.  A comment is
 * denoted by a # character through the end of line.
 */
void
MleDwpInput::skipComment(void)
{
	// Look for a newline or the end of the buffer.
	int c;
	do {
		c = getNextByte();
	} while ( c != '\n' && c != 0 );
	
	// Put back the terminating character because we don't
	// want to read past the end of input.
	putBackByte(c);
}


/*
 * This function reports an error by writing the passed string
 * to stdout along with line number information and the offending
 * line of input.
 */
void
MleDwpInput::reportError(const char *reason)
{
	fprintf(stderr,"DWP input error: %s at line %d of %s\n",reason,m_lineno,
		m_stack ? (char *)m_stack->m_path->getPath() : "memory buffer");

	// Terminate line buffer.
	if ( m_lineIndex < MLE_DWP_INPUT_BUFSIZE )
		m_linebuf[m_lineIndex] = 0;
	else
		m_linebuf[MLE_DWP_INPUT_BUFSIZE - 1] = 0;

	fprintf(stderr,"%*s\n",m_lineIndex,m_linebuf);
}


/*
 * This function sets input to come from a file pointer.
 */
int
MleDwpInput::setFilePointer(FILE *ufp)
{
	MLE_ASSERT(ufp);

	// It is an error to set a file pointer when a file is open.
	MLE_ASSERT(m_stack == NULL);

	m_fp = ufp;

	m_lineno = 1;
	m_lineIndex = 0;

	if ( readMagic() )
	{
		m_fp = NULL;
		return 1;
	}
	else
		return 0;
}


/*
 * This function puts the last read byte back into the input
 * stream.  The putBack buffer contains only one byte, so
 * multiple putbacks are not possible.
 */
void
MleDwpInput::putBackByte(int putBack)
{
	if ( m_fp )
		ungetc(putBack,m_fp);
	else if ( m_buffer )
		m_buffer--;
	
	if ( putBack == '\n' )
		m_lineno--;
	else
		m_lineIndex--;
}


/*
 * This function performs raw byte reading, using buffering
 * and keeping track of the number of lines read.
 */
int
MleDwpInput::getNextByte(void)
{
	int c;

	if ( m_fp )
	{
		if ( (c = getc(m_fp)) == EOF )
			return 0;
	}
	else if ( m_buffer )
	{
		if ( *m_buffer == 0 )
			return 0;

		c = *m_buffer++;
	}
	else 
	{
		// If both are null, then c is undefined below.
		MLE_ASSERT( (NULL != m_fp) || (NULL != m_buffer) );
		return 0;
	}
	

	if ( m_lineIndex < MLE_DWP_INPUT_BUFSIZE )
		m_linebuf[m_lineIndex++] = c;

	if ( c == '\n' )
	{
		m_lineno++;
		m_lineIndex = 0;
	}

	return c;
}


/*
 * This function is the basic lexing engine.  It returns a token,
 * which is vaguely defined as a sequence of input characters
 * delimited by white space or a small set of special characters
 * (which are tokens by themselves).  Sequences inside double
 * quotes are returned (without the quotes) as a single token.  
 * The pound sign (#) indicates a comment, and input is ignored
 * to the end of the line.
 *
 * Currently the list of special characters are:
 *
 * 	#	comment
 * 	(	start item
 * 	)	end item
 */
int MleDwpInput::lex(char *token,int bufsize)
{
	MLE_ASSERT(token);
	MLE_ASSERT(bufsize >= 2);

	enum { White, Token, QuoteToken };

	int state = White;

	// Subtract one off the buffer size to account for the terminator.
	bufsize--;

	int escapeFlag = 0;	// Signals if last character was backslash.

	while ( 1 )
	{
		if ( bufsize == 0 )
		{
			reportError("input buffer overflow.\n");
			*token = 0;
			return 1;
		}

		// Get the next character.
		char c = getNextByte();
		
		if ( state == White )
		{
			if ( isspace(c) )
				;
			else if ( c == '#' )
				skipComment();
			else if ( c == '(' || c == ')' )
			{
				// Don't lex off a paren - they should not
				// be read as tokens, only chars.
				putBackByte(c);
				return 1;
			}
			else if ( c == '\"' )
				state = QuoteToken;
			else if ( c == '\\' )
			{
				// If the first character is a backslash,
				// then set the escape flag.
				escapeFlag = 1;
				state = Token;
				bufsize--;
			}
			else if ( c == 0 )
			{
				reportError("input terminated without a token");
				return 1;
			}
			else
			{
				state = Token;
				*token++ = c;
				bufsize--;
			}
		}
		else if ( state == Token )
		{
			if ( escapeFlag && c )
			{
				// If the previous character was a backslash,
				// then accept the next character without
				// checking for special characters.
				*token++ = c;
				bufsize--;
				escapeFlag = 0;
			}
			else if ( isspace(c) )
			{
				*token = 0;
				return 0;
			}
			else if ( c == '(' || c == ')' )
			{
				putBackByte(c);
				*token = 0;
				return 0;
			}
			else if ( c == '\\' )
				escapeFlag = 1;
			else if ( c == 0 )
			{
				putBackByte(c);
				*token = 0;
				return 0;
			}
			else
			{
				*token++ = c;
				bufsize--;
			}
		}
		else if ( state == QuoteToken )
		{
			if ( escapeFlag && c )
			{
				// If the previous character was a backslash,
				// accept it unconditionally.
				*token++ = c;
				bufsize--;
				escapeFlag = 0;
			}
			else if ( c == '\n' )
			{
				reportError("newline in the middle of string");
				return 1;
			}
			else if ( c == '\"' )
			{
				*token = 0;
				return 0;
			}
			else if ( c == '\\' )
				escapeFlag = 1;
			else if ( c == 0 )
			{
				reportError("input terminated in middle of string");
				return 1;
			}
			else
			{
				*token++ = c;
				bufsize--;
			}
		}
	}
}


void *
MleDwpInput::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpInput::operator delete(void *p)
{
	mlFree(p);
}


void *
MleDwpInputTable::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpInputTable::operator delete(void *p)
{
	mlFree(p);
}


void *
MleDwpInputFile::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleDwpInputFile::operator delete(void *p)
{
	mlFree(p);
}


#ifdef UNIT_TEST

// Include system header files.
#include <stdlib.h>

main()
{
	MleDwpInput *in = new MleDwpInput;

	in->setFilePointer(stdin);

	int d;
	MleDwpInputTable table[] = {
		MleDwpInputToken, "test",
		MleDwpInputInt, &d,
		MleDwpInputEnd
	}
	;

//	int d;
//	table[1].ptr = &d;

	in->readTable(table);

	printf("d = %d\n",d);
}


#endif /* UNIT_TEST */
