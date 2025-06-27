/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpFilename.h
 * @ingroup MleDWPAccess
 *
 * This file defines a file name utility used by the Magic Lantern Digital
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_DWP_FILENAME_H_
#define __MLE_DWP_FILENAME_H_

// Include Digital Worprint header files.
#include "mle/Dwp.h"

/**
 * @brief This is an object that specifies a filename.
 *
 * The name may be either relative or absolute. The path is always absolute.
 */
class MLE_DWP_API MleDwpFilename
{
  public:

	/**
	 * @brief Default constructor.
	 */
	MleDwpFilename(void);
	
	/**
	 * @brief The destructor.
	 */
	~MleDwpFilename(void);

	/**
	 * @brief Set the name of the file.
	 *
	 * @param name A pointer to a character array identifying the name of
	 * the file to set.
	 */
	void setName(const char *name);
	
	/**
	 * @brief Get the name of the file.
	 *
	 * @return A pointer to a character array is returned identifying the
	 * name of the file.
	 */
	const char *getName(void) const { return m_name; }

	/**
	 * @brief Set the path of the file.
	 *
	 * @param path A pointer to a character array identifying the path of
	 * the file to set.
	 */
	void setPath(const char *path);

	/**
	 * @brief Get the path of the file.
	 *
	 * @return A pointer to a character array is returned identifying the
	 * path of the file.
	 */
	const char *getPath(void) const { return m_path; }

	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
     * Override operator new array.
     *
     * @param tSize The size, in bytes, to allocate.
     */
	void* operator new[](size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);

	/**
     * Override operator delete array.
     *
     * @param p A pointer to the memory to delete.
     */
	void  operator delete[](void* p);

  protected:

	char *m_name;  /**< The name of the file. */
	char *m_path;  /**< The path of the file. */
};


#endif /* __MLE_DWP_FILENAME_H_ */
