/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpFile.h
 * @ingroup MleDWPAccess
 *
 * This file defines the File workprint item.
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

#ifndef __MLE_DWP_FILE_H_
#define __MLE_DWP_FILE_H_


// Include Digital Workprint header files.
#include "mle/Dwp.h"
#include "mle/DwpItem.h"


/**
 * @brief This defines a file inclusion.
 *
 * A file inclusion is itself a workprint item and exists in the tree
 * structure.  At read time, the items in the included file are parented
 * to the file item.  At write time, the items under the file item are
 * written out to the file specified.
 * <p>
 * Note that the filename in the workprint can have some ambiguity.  For
 * example, if it contains "foo.wp", it could be read from a number of
 * different places - the directory the application was run from, the
 * directory the current workprint file is in, etc.  The search policy
 * of <code>MleDwpInput::openFile()</code> is used.
 * </p><p>
 * The result of the search policy is a path, and that is kept with
 * the file item but is not written into the workprint.  The path
 * may be specified by the application as well, to disambiguate the
 * filename.  On write, the path is used if it exists, else the
 * filename is used as a path.
 * </p><p>
 * @verbatim
 * WORKPRINT SYNTAX
 * Syntax:    (File filename)
 *            <filename>  file to include at this point (char *)
 *
 *
 * Contained by: all
 * Contains:     none
 *
 * EXAMPLE
 * END
 * @endverbatim
 * </p><p>
 * The name field (getName(), setName()) is used to store the filename.
 * </p>
 */
class MLE_DWP_API MleDwpFile : public MleDwpItem
{
	// Include mandatory workprint header macro.
	MLE_DWP_HEADER(MleDwpFile);

  public:

	/**
	 * @brief Default constructor.
	 */
	MleDwpFile(void);

	/**
	 * @brief The destructor.
	 */
	~MleDwpFile();

	/**
	 * @brief Set the path of the file.
	 *
	 * The path is a specific file specifier for this item.  This
	 * is not the filename that is kept in the workprint, but an
	 * interpretation of that filename.  The filename can have
	 * ambiguity as to which directory it refers to.  That is why
	 * there is a policy in <code>MleDwpInput</code> as to how directories are
	 * searched to open new files.  The path, if it exists, is
	 * used to disambiguate this directory.  <code>mlFOpen()</code> is called
	 * directly on the path.
	 * </p><p>
	 * The path is automatically set when this item is loaded
	 * from file.  An application may also set the path to direct
	 * writing to a specific file.  If the path is not set, the
	 * filename is used as the path.
	 * </p>
	 *
	 * @param path A pointer to a character array identifying the
	 * file path.
	 */
	void setPath(const char *path);

	/**
	 * @brief Set the path of the file.
	 *
	 * @return A pointer to a character array identifying the file
	 * path is returned.
	 */
	const char *getPath(void) const;

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

  protected:

	/**
	 * @brief Read the contents of the workprint item.
	 *
	 * @param in A pointer to an workprint input object.
	 *
	 * @return If the contents are successfully read, then <b>TRUE</b>
	 * will be returned. Otherwise, <b>FALSE</b> will be returned.
	 */
	virtual int readContents(MleDwpInput *in);

	/**
	 * @brief Write the contents of the workprint item.
	 *
	 * @param out A pointer to an workprint output object
	 * to write to.
	 * @param writeChildren A flag indicating whether to write
	 * the children also.
	 *
	 * @return If the contents are successfully written, then <b>TRUE</b>
	 * will be returned. Otherwise, <b>FALSE</b> will be returned.
	 */
	virtual int write(MleDwpOutput *out,int writeChildren = 1);

	char *m_path;  /**< The file path. */
};

#endif /* __MLE_DWP_FILE_H_ */
