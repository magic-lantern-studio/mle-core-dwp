/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpMediaRef.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date September 28, 2015
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


#ifndef __MLE_DWP_MEDIAREF_H_
#define __MLE_DWP_MEDIAREF_H_

// Include DigitalWorkprint header files.
#include "mle/DwpItem.h"

typedef struct _MleDwpMediaRefTargetMedia
{
    int m_flags;
    char *m_label;
    char *m_filename;

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

} MleDwpMediaRefTargetMedia;

class MleDppMediaOutput;

/**
 * This is the root container that defines a media reference workprint item.
 * It is used to during mastering.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * (MediaRef <name> <type>
 *   (MediaRefSource ...)
 *   (MediaRefTarget ...)
 *   (MediaRefTarget ...)
 * )
 * 
 * <name>	name of the media reference (char *)
 * <type>	type or class of media reference (char *)
 * 
 * Contained by: none
 * Contains:     MleDwpMediaRefSource, MleDwpMediaRefTarget
 * 
 * END
 * 
 * @see MleDwpMRSource, MleDwpMRTarget
 */
class MLE_DWP_API MleDwpMediaRef : public MleDwpItem
{
    MLE_DWP_HEADER(MleDwpMediaRef);

  public:

    MleDwpMediaRef(void);

    MleDwpMediaRef(char *mrname, char *mrclass);

    virtual ~MleDwpMediaRef(void);

    /**
     * Writes a fixed-size external media reference record.
	 */
    int writeMediaRefChunk(MleDppMediaOutput *out,unsigned long type);

    /**
     * Sets the media ref type name (C++ class).
	 */
    void setMediaRefClass(const char *mediaRefType);

    /**
	 * Returns the media ref type.  NULL is returned
     * if no media ref type has been set.
	 */
    const char *getMediaRefClass(void) const;

    /**
     * queryTargetEntries() is a convenience function that returns the
     * number of entries in the MediaRefTarget workprint subhierarchy.
     * As a side effect, it will also pass back an array of
     * MleDwpMRTargetMediaEntry structures which will contain the
     * contents of the MleDwpMedia workprint items found in the MediaRefTarget.
     * This method expects the discriminator to be set for the MediaRef
     * target such that only one target will be found in the
     * subhierarchy. If more than one target is matched, then a value
     * of -1 will be returned.
	 */
    int queryTargetEntries(MleDwpMediaRefTargetMedia **entries);

	/**
	 * Release the target entries that were allocated by queryTargetEntries().
	 */
	int releaseTargetEntries(MleDwpMediaRefTargetMedia **entries);

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

    virtual int readContents(MleDwpInput *in);

    virtual int writeContents(MleDwpOutput *out);

    // WorkPrint item data
    char *m_mediaRefClass; // Media reference class.
};

#endif /* __MLE_DWP_MEDIAREF_H_ */
