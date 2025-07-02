/** @defgroup MleDWPAccess Magic Lantern Digital Workprint Library API - Base */

/**
 * @file DwpInclude.h
 * @ingroup MleDWPAccess
 *
 * This file defines the file inclusion used by the Magic Lantern Digital
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

#ifndef __MLE_DWP_INCLUDE_H_
#define __MLE_DWP_INCLUDE_H_


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
 * example, if it contains "foo.dwp", it could be read from a number of
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
 * Syntax:    (Include filename)
 *            <filename>  file to include at this point (char *)
 *
 * Contained by:	all
 * Contains:		none
 *
 * EXAMPLE
 * END
 * @endverbatim
 * </p><p>
 * The name field (getName(), setName()) is used to store the filename.
 * </p>
 */
class MLE_DWP_API MleDwpInclude : public MleDwpItem
{
	MLE_DWP_HEADER(MleDwpInclude);

  public:

	/**
	 * @brief The default constructor.
	 */
	MleDwpInclude(void);

	/**
	 * @brief The destructor.
	 */
	~MleDwpInclude();

	/**
	 * @brief Set the path.
	 *
	 * The path is a specific file specifier for this item.  This
	 * is not the filename that is kept in the workprint, but an
	 * interpretation of that filename.  The filename can have
	 * ambiguity as to which directory it refers to.  That is why
	 * there is a policy in MleDwpInput as to how directories are
	 * searched to open new files.  The path, if it exists, is
	 * used to disambiguate this directory.  <code>mlFOpen()</code> is called
	 * directly on the path.
	 * <p>
	 * The path is automatically set when this item is loaded
	 * from file.  An application may also set the path to direct
	 * writing to a specific file.  If the path is not set, the
	 * filename is used as the path.
	 * </p>
	 *
	 * @param path A pointer to a character string representing the
	 * path to set.
	 */
	void setPath(const char *path);
	
	/**
	 * @brief Get the path.
	 *
	 * @return A pointer to a character string representing the path
	 * is returned.
	 */
	const char *getPath(void) const;

	/**
	 * @brief Copy the workprint item.
	 *
	 * The default <i>copy()</i> from <code>MleDwpItem</code> needs to be overridden
	 * because an Include item will generally have children.
	 * The way the base copy() works, copying the item itself
	 * will bring in the children and then the children will be
	 * copied, resulting in duplicate children.
	 *
	 * @param copyChildren A flag indicating whether children of the
	 * workprint item should be copied.
	 * @param parent A pointer to a parent workprint item in which to
	 * attach the copied workprint to.
	 *
	 * @return A pointer to the copied workprint item is returned.
	 */
	virtual MleDwpItem *copy(int copyChildren = 1,MleDwpItem *parent = NULL);

	/**
	 * An enumeration specifying the mode to write.
	 */
	enum Enabling {
		DEFAULT,         /**< Write to the specified file if not overridden. */
		WRITE_FILE,      /**< Write to the specified file. */
		WRITE_NONE,      /**< Do not write children. */
		WRITE_CURRENT    /**< Write to the current output as a container. */
	};

	/**
	 * @brief Enable writing children on this instance.
	 *
	 * These routines enable or disable writing child items into
	 * the file specified by the Include item.  It is sometimes not
	 * desired to propagate a write operation, for example, for in-
	 * memory transcription.  This controls writing children on an
	 * instance basis.  This overrides any class enabling unless
	 * <b>DEFAULT</b> is set.
	 *
	 * @param flag A flag indicating the mode for writing children
	 * on an instance.
	 */
	void setInstanceWriteChildren(Enabling flag);

	/**
	 * @brief Get the mode for writing children on an instance.
	 *
	 * @return The mode for a particular instance is returned.
	 */
	Enabling getInstanceWriteChildren(void) const;

	/**
	 * @brief Enable writing children for all instances.
	 *
	 * These routines enable or disable writing child items into
	 * the file specified by the Include item.  This controls writing
	 * children on a classwide basis.  This value can be overridden
	 * by instances.
	 */
	static void setClassWriteChildren(Enabling flag);
	
	/**
	 * @brief Get the mode for writing children for all
	 * instances.
	 *
	 * @return The mode for all instances is returned.
	 */
	static Enabling getClassWriteChildren(void);

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

	/**
	 * @brief Read the contents of the workprint item.
	 *
	 * @param in A pointer to an workprint input object.
	 *
	 * @return If the contents are successfully read, then <b>TRUE</b>
	 * will be returned. Otherwise, <b>FALSE</b> will be returned.
	 */
	virtual int readContents(MleDwpInput *in);

  public:

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

	/**
	 * Populate the subtree under us with the current 
	 * data from the file. Current children are deleted if they 
	 * exist, and the subtree is populated. This allows updating of
	 * included files, as well as populating an include subtree
	 * after constructing the include node.
	 *
	 * @return <b>TRUE</b> is returned if the subtree is successfully
	 * populated. Otherwise <b>FALSE</b> will be returned.
	 */
	virtual int populate(void);

  protected:

	/** The name of the path for the Include file. */
	char *m_path;
	/** Flag indicating mode of local write policy. */
	Enabling m_localWriteEnable;

    /** Flag indicating mode of global write policy. */
	static Enabling g_globalWriteEnable;
};


#endif /* __MLE_DWP_INCLUDE_H_ */
