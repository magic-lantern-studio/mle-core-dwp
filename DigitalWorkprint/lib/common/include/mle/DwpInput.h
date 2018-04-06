/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpInput.h
 * @ingroup MleDWPAccess
 *
 * This file defines the input object used by the Magic Lantern Digital
 * Workprint Library API.
 *
 * @author Mark S. Millard
 * @date July 10, 2003
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_DWP_INPUT_H_
#define __MLE_DWP_INPUT_H_


// Include system header files.
#include <stdio.h>

// Include Digital Workprint header files.
#include "mle/Dwp.h"


/**
 * @brief The default input buffer size.
 */
#define MLE_DWP_INPUT_BUFSIZE 1024


struct MleDwpInputFile;
class MleDwpFilename;


/**
 * This is the enumeration for table driven input.
 */
typedef enum
{
	MleDwpInputEnd,    /**< End of input. */
	MleDwpInputToken,  /**< An input token. */
	MleDwpInputInt,    /**< An input of type int. */
	MleDwpInputFloat,  /**< An input of type float. */
	MleDwpInputString  /**< An input of type string. */
} MleDwpInputType;


/**
 * This is the element type for table driven input.  An array of this
 * structure may be passed to the input lexer.
 */
struct MLE_DWP_API MleDwpInputTable
{
	/** The type of input. */
	MleDwpInputType type;
	/** A pointer to the associated input data. */
	void *ptr;

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


/**
 * @brief This is a generalized input object that is intended to abstract away the
 * actual source of input - file, stream, memory buffer, etc.
 *
 * @see MleDwpOutput
 */
class MLE_DWP_API MleDwpInput
{

  public:

	/**
	 * @brief A constructor.
	 *
	 * The argument to the constructor is a flag that specifies whether
	 * to look for the magic identifier at the beginning of the data
	 * stream that verifies the input type.  A nonzero value means
	 * that the magic identifier will be required.
	 *
	 * @param magicFlag Check for magic identifier.
	 */
	MleDwpInput(int magicFlag = 1);
	
	/**
	 * @brief The destructor.
	 */
	~MleDwpInput();

	/**
	 * Set input source.
	 *
	 * <i>openFile()</i> opens a file given a filename.  <i>openFile()</i> may
	 * be called multiple times, which puts files on a file
	 * stack.  Input comes from the file on the top of the stack.
	 * A search policy is imposed to find directories of filenames
	 * that are not complete path specifications.  This policy is:
	 * <li>
	 * <ol>If the filename is an absolute path, use it.</ol>
	 * <ol>Try the directory of the current file, if any.</ol>
	 * <ol>Else look in some well-known places, currently
	 * "./"</ol>
	 *</li>
	 * <p>
	 * All input setting routines return nonzero on an error.
	 * </p>
	 *
	 * @param filename The name of the file to open.
	 *
	 * @return If the file is successfully opened, then <b>TRUE</b> is returned.
	 * Otherwise, <b>FALSE</b> will be returned.
	 */
	int openFile(const char *filename);

	/**
	 * Set input source.
	 *
	 * <i>setFilePointer()</i> uses an existing opened stream for input.
	 *
	 * @param fp A file pointer.
  	 *
	 * @return If the file pointer is successfully set, then <b>TRUE</b> is returned.
	 * Otherwise, <b>FALSE</b> will be returned.
	 */
	int setFilePointer(FILE *fp);

	/**
	 * Set input source.
	 *
	 * <i>setBuffer()</i> uses a null-terminated string for input.
	 *
	 * @param buffer A pointer to an input buffer.
	 *
	 * @return If the buffer is successfully set, then <b>TRUE</b> is returned.
	 * Otherwise, <b>FALSE</b> will be returned.
	 */
	int setBuffer(const char *buffer);

	/**
	 * Set the magic number.
	 *
	 * By default, a magic number will be read whenever a new
	 * input source is chosen.  Sometimes it is convenient not
	 * to have the magic number.  In this case, the read and
	 * test for it can be suppressed with this call.  It can
	 * also be configured from the constructor.  A nonzero value
	 * will require the magic number.
	 *
	 * @param magicFlag A flag indicating whether to check for 
	 * a magic number in the input.
	 */
	void setMagic(int magicFlag);

	/**
	 * This closes the most recently opened file.
	 *
	 * Input will continue from the next most recently opened
     * file, if one exists, or from an input buffer.
	 */
	void closeFile(void);

	/**
	 * Get the path.
	 *
	 * <i>getPath()</i> returns the path to the current file (if any)
	 * being read.  The path is more specific than the filename
	 * used in the call to <i>openFile()</i> because there is some
	 * directory ambiguity that is resolved.  The memory for
	 * the string returned will be deallocated when <i>closeFile()</i>
	 * is called for this file.
	 *
	 * @return A pointer to a string representing the path is returned.
	 */
	const char *getPath(void) const;

	// lexer functions
	//   read{String,Int,Float,Char}() attempt to read an item of the
	//   indicated type from the input.
	//     readLine()   All the functions return nonzero if an error is
	//   encountered.


	/**
	 * Attempt to read a string from the input.
     *
	 * <i>readString()</i> does not allocate any space, but requires a pointer
	 * to a buffer to be passed in.
	 * <i>readString()</i> takes a second optional argument that is the size
	 * of the buffer.  By default, the buffer is assumed to be the input
	 * buffer size.  <i>readString()</i> will not read left or right parens.
	 * If reading parens is necessary, </i>readChar()</i> must be used for this.
	 *
	 * @param s The buffer to read the string in to.
	 * @param bufsize The size of the buffer to read into.
	 *
	 * @return If the string is successfully read, then <b>0</b> is returned.
	 * Otherwise, nonzero will be returned.
	 */
	int readString(char *s,int bufsize = MLE_DWP_INPUT_BUFSIZE);

	/**
	 * Attempt to read an integer from the input.
     *
	 * @param d The integer to read the value in to.
	 *
	 * @return If the value is successfully read, then <b>0</b> is returned.
	 * Otherwise, nonzero will be returned.
	 */
	int readInt(int *d);

	/**
	 * Attempt to read a single precision floating-poing from the input.
     *
	 * @param f The float to read the value in to.
	 *
	 * @return If the value is successfully read, then <b>0</b> is returned.
	 * Otherwise, nonzero will be returned.
	 */
	int readFloat(float *f);

	/**
	 * Attempt to read the next nonspace character.
	 *
	 * @param c The char to read the value in to.
	 *
	 * @return If the value is successfully read, then <b>0</b> is returned.
	 * Otherwise, nonzero will be returned.
	 */
	int readChar(char *c);

	/**
	 * Attempt to read and return all characters until the next carriage return is
	 * is encountered.
	 *
	 * @param s The buffer to read the string in to.
	 * @param bufsize The size of the buffer to read into.
	 *
	 * @return If the value is successfully read, then <b>0</b> is returned.
	 * Otherwise, nonzero will be returned.
	 */
	int readLine(char *s, int bufsize =  MLE_DWP_INPUT_BUFSIZE);

	/**
	 * <i>readFilename()</i> is a special version of <i>readString()</i> that will
	 * read a file specifier and write it into a special <code>MleDwpFilename</code>
	 * object <i>with</i> the absolute path to the first existing file
	 * that it finds looking in the directory stack.
	 *
	 * @param f The name of the file to read.
	 *
	 * @return If the file is successfully read, then <b>0</b> is returned.
	 * Otherwise, nonzero will be returned.
	 */
	int readFilename(MleDwpFilename *f);

	/**
	 * <i>readTable()</i> is a convenience method to do multiple reads.
	 *
	 * An array item containing the terminator <code>MleDwpEnd</code> is used.
	 * Each entry has a type enum and a pointer where
	 * data is to be placed.  There is one additional feature, and
	 * that is that tokens may be recognized.  If the entry is 
	 * designated as a DwpToken type, the pointer is assumed to point
	 * to a character string that must match the string encountered
	 * in the input.
	 * <p>
	 * For string input, the buffer supplied should be of at least
	 * <code>MLE_DWP_INPUT_BUFSIZE</code> long to avoid overrunning the buffer on
	 * bad input.
	 * </p>
	 *
	 * @param table The array of input item types to look for in the
	 * input.
	 *
	 * @return <i>readTable()</i> returns nonzero if an error is
	 * encountered.
	 */
	int readTable(const MleDwpInputTable *table);

	/**
	 * Error reporting.
	 *
	 * <i>reportError()</i> prints its string argument with the line number
	 * the error occurred on with the offending line.
	 *
	 * @param str A message string to use to report the error with.
	 */
	void reportError(const char *msg);
	
	/**
	 * Attempts to skip to the end of the workprint item.
	 */
	void skip(void);

	/**
	 * Raw buffer processing, read.
	 *
	 * The use of raw reads and unreads is discouraged but is
	 * available for special applications.
	 *
	 * @return The next byte from the input is returned.
	 */
	int getNextByte(void);

	/**
	 * Raw buffer processing, unread.
	 *
	 * The use of raw reads and unreads is discouraged but is
	 * available for special applications.
	 *
	 * @param b The byte to put back into the input.
	 */
	void putBackByte(int b);

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

	// The number of the line being processed.
	int m_lineno;
	// An index into the line being processed.
	int m_lineIndex;
	// The line buffer.
	char m_linebuf[MLE_DWP_INPUT_BUFSIZE];
	// The buffer being processed.
	const char *m_buffer;
	// The file pointer being processed.
	FILE *m_fp;
	// The stack of files being processed.
	MleDwpInputFile *m_stack;

	// Magic header flag.
	int m_readHeader;

	// Utility for skipping comments.
	void skipComment(void);

	// readMagic() reads the beginning of input for a special string.
	// Nonzero is returned if the string does not match.
	int readMagic(void);

	// lex() reads off a token input to supplied buffer.  Nonzero
	// is returned on an error.  This function will not read left
	// or right parens.
	int lex(char *token,int bufsize = MLE_DWP_INPUT_BUFSIZE);
};


#endif /* __MLE_DWP_INPUT_H_ */
