/** @defgroup MleDWPModel Magic Lantern Digital Workprint Library API - Model */

/**
 * @file DwpMedia.h
 * @ingroup MleDWPModel
 *
 * Magic Lantern Digital Workprint Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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


#ifndef __MLE_DWP_MEDIA_H_
#define __MLE_DWP_MEDIA_H_

// Include Digital Workprint header files.
#include <mle/DwpItem.h>
#include <mle/DwpFilename.h>


/**
 * This is the root container that defines a media reference workprint item.
 * It is used during mastering to specify the mastering flags, media label,
 * and media filename.
 * 
 * The flags are used to identify how to master the Media. For Magic Lantern 1.0,
 * this field should always be 0. Zero means that the Media is an external media
 * reference. That is, the mastered MediaRef chunk will contain an external file
 * name.  Future versions of the mastering tools will use this field to
 * distinguish whether the media is to be embedded into the Digital Playprint and whether
 * the media will use some form of containership.
 * 
 * WORKPRINT SYNTAX
 * 
 * EXAMPLE
 * Syntax:	(Media <flag> <label> <filename>)
 * 
 * <flag>::= tag of media ( an int)
 * (label)::= label of media
 * (filename)::= filename of media
 * 
 * Contained by: MleDwpMediaRefSource and MleDwpMediaRefTarget
 * 
 * Contains: Conversion string
 * 
 * END
 */

class MLE_DWP_API MleDwpMedia : public MleDwpItem
{
    MLE_DWP_HEADER(MleDwpMedia);

  public:

    MleDwpMedia(void);

    virtual ~MleDwpMedia(void);

    virtual int readContents(MleDwpInput *in);

    virtual int writeContents(MleDwpOutput *out);

    void setFlags(const int);

    int getFlags(void) const;

    void setFilename(const char *);

    const char *getFilename(void) const;

    void setLabel(const char *);

    const char *getLabel(void) const;

    /**
     * copy() is an override of MleDwpItem::copy(), not a new interface.
     * This item needs its own copy function because it can contain
     * state that is not preserved across transcription (the mechanism
     * used in the base class copy).
	 */
    virtual MleDwpItem *copy(int copyChildren = 1,MleDwpItem *parent = NULL);

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

    char *m_label; // Label of media.
    int  m_flags; // Target flag infor (extenal or internal reference).
    MleDwpFilename m_filename; // Pathname of file in target (for external file references).
};


#endif /* __MLE_DWP_MEDIA_H_ */
