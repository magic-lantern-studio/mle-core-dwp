/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpOutput.h
 * @ingroup MleDWPAccess
 *
 * This file defines the output object used by the Magic Lantern Digital
 * Workprint Library API.
 *
 * @author Mark S. Millard
 * @date July 10, 2003
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

#ifndef __MLE_DWP_OUTPUT_H_
#define __MLE_DWP_OUTPUT_H_


// Include system header files.
#include <stdio.h>

// Include Digital Workprint header files.
#include "mle/Dwp.h"


class MleDwpFilename;

/**
 * @brief This is a generalized output object that is intended to abstract away
 * the actual output type - file, stream, memory buffer, etc.
 *
 * @see MleDwpInput
 */
class MLE_DWP_API MleDwpOutput
{

  public:

	/**
	 * @brief A constructor.
	 *
	 * @param magicFlag The argument to the constructor is a flag that specifies
	 * whether to write out a magic identifier at the beginning
	 * of the data stream that will verify it to a reader of the
	 * stream.  A nonzero value means that the magic identifier
	 * will be written.
	 */
	MleDwpOutput(int magicFlag = 1);

	/**
	 * The destructor.
	 */
	~MleDwpOutput();

	/**
	 * @brief Set the output sink to be a file.
	 *
	 * @param filename A pointer to the file name to open.
	 *
	 * @return Nonzero will be returned in the case of an error.
	 * Otherwise, 0 will be returned.
	 */
	int openFile(const char *filename);

	/**
	 * @brief Set the output sink to be a file that will
	 * be appended to.
	 *
	 * <i>appendFile()</i> opens a file and appends all subsequent writes to
	 * it's current contents.
	 *
	 * @param filename A pointer to the file name to open.
	 *
	 * @return Nonzero will be returned in the case of an error.
	 * Otherwise, 0 will be returned.
	 */
    int appendFile(const char *filename);

	/**
	 * @brief Set the output sink to be a file.
	 *
	 * @param fp A pointer to a <b>FILE</b>.
	 */
	void setFilePointer(FILE *fp);

	/**
	 * @brief Set the output sink to be a buffer.
	 *
	 * <i>setBuffer()</i> passes in a memory buffer and initial size of
	 * the buffer.  This buffer is assumed to have been allocated
	 * by <i>mlMalloc()</i>.  However, optional arguments can be passed if
	 * this is not the case to provide a user reallocation function.
	 * The user function should take a buffer pointer, the requested
	 * new size, and user data.  The function returns a pointer to
	 * the new buffer.  This allows writing to a buffer in shared
	 * memory, for example.
	 *
	 * A handy tip is if <b>NULL</b. is passed as the original buffer with
	 * an initial size of zero, the reallocation function will be
	 * called before any writing is done.  This works conveniently
	 * with the default <i>mlMalloc/mlRealloc</i>.
	 *
	 * @param buffer A pointer to a buffer in which to write output
	 * to.
	 * @param initialSize The initial size of the buffer.
	 * @param reallocFunc A user reallocation function.
	 * @param reallocData User data to be used with the realocation
	 * routine.
     */
	void setBuffer(char *buffer,int initialSize,
		char *(*reallocFunc)(char *,int,void *) = NULL,
		void *reallocData = NULL);

	/**
	 * Enable writing of header.
	 *
	 * Output is generally preceded with a magic string that
	 * identifies the type of file.  In some cases, transcription
	 * is not used for file storage but for display or other
	 * purpose where the magic header is just an annoyance.
	 * <i>setMagic()</i> allows activating or deactivating
	 * the writing of the magic header.  It can also be specified
	 * from the constructor.  Passing a nonzero value activates
	 * writing the magic identifier.
	 *
	 * @param magicFlag The flag indicating whether to write the
	 * magic number or not.
	 */
	void setMagic(int magicFlag);

	/**
	 * Get buffer information back out.
	 *
	 * This function is not const because the buffer may need to
	 * be terminated.  The function may either be used to return
	 * the buffer pointer, or the buffer and size may be returned
	 * by passing pointers to storage values.
	 *
	 * @param buffer The buffer is returned as an output
	 * paramater.
	 * @param size The size of the buffer is returned as an output
	 * parameter.
	 *
	 * @return The oputput buffer is returned.
	 */
	char *getBuffer(char **buffer = NULL,int *size = NULL);

	//////////
	// output functions
	//   write{String,Int,Float,Char} write an item of the indicated
	//   type to the output.  All the functions return nonzero if an
	//   error is encountered.
	//   writeFancyString() is a special version of writeString()
	//   that scans the input string for embedded spaces and special
	//   characters and encloses the string in quotes and escapes
	//   characters as needed.  Any string that might have these
	//   attributes should use the fancier output.  Strings that
	//   are known not to have these attributes will be written
	//   faster with writeString().
	int writeString(const char *s);
	//////////
	int writeFancyString(const char *s);
	//////////
	int writeInt(int d);
	//////////
	int writeFloat(float f);
	//////////
	int writeChar(char c);

	//////////
	// writeFilename() is a special version of writeString() that
	//   operates on a filename object.
	int writeFilename(MleDwpFilename *f);

	//////////
	// setFilenameExpansion() configures how filenames
	//   are written.  When filename expansion is on, full absolute
	//   paths are written out.
	void setFilenameExpansion(int onOrOff);

	//////////
	// formatting
	//   writeTerminal() writes an abstract terminal to the output.
	//   In practice, this will be a newline for ascii output and
	//   nothing for binary output.  Terminals may be required to
	//   help input objects keep reasonable buffer sizes.
	//   adjustIndent() is abstract indenting operation.  The argument
	//   (typically either +1 or -1) is a delta to the current indenting
	//   level.
	int writeTerminal(void);
	//////////
	int adjustIndent(int indent);

	//////////
	// flushing
	// flush() writes all buffered characters out, where applicable
	// return value is zero unless an error was encountered
	int flush(void);

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

 private:

	void indent(void);

	int lineno;
	int ilevel;
	int newline;

	// Magic header flag.
	int writeHeader;

	// Filename expansion.
	int expandFilenames;

	// State for file or file pointer output.
	FILE *fp;
	int closeFlag;

	// State for memory buffer output.
	char *buffer;
	int bufSize;	// The current buffer size, in bytes.
	int bufUsed;	// The number of bytes used in the buffer.

	char *(*reallocFunc)(char *,int,void *); // User realloc func.
	void *reallocData;                       // User realloc data.

	// writeMagic() outputs a magic string that identifies the output.
	//  as a workprint.
	void writeMagic(void);

	// expandBuffer() is an internal function that reallocates the
	// memory buffer.
	void expandBuffer(void);

#if defined(MLE_DEBUG)
	int bufGetFlag;
#endif
};


#endif  /* __MLE_DWP_OUTPUT_H_ */
