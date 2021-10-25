/** @defgroup MleDWPTest Magic Lantern Digital Workprint Library API - Test */

/**
 * @file dwpCheckerTest.cpp
 * @ingroup MleDWPTest
 *
 * This program reads a digital workprint and runs the MleDwpChecker on it to 
 * test for workprint validity.
 */

// COPYRIGHT_BEGIN
// The MIT License (MIT)
//
// Copyright (c) 2015-2021 Wizzer Works
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
#ifdef WIN32
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlErrno.h"
#include <mle/MlePath.h>
#ifdef WIN32
#include "mle/mlGetOpt.h"
#include <mle/MleWin32Path.h>
#else
#include <unistd.h>
#include <mle/MleLinuxPath.h>
#endif /* WIN32 */

// Include Digital Wrokprint header files.
#include "mle/Dwp.h"
#include "mle/DwpItem.h"
#include "mle/DwpInput.h"
#include "mle/DwpDatatype.h"
#include "mle/DwpChecker.h"

extern void mleDwpInit(void);


#ifdef WIN32
static char *getCanonicalPath(char *path)
{
	char *cpath = NULL;
	MleWin32Path *wpath = new MleWin32Path((MlChar *)path, true);
    cpath = strdup((char *)wpath->getPath());
	delete wpath;
	return cpath;
}
#else
static char *getCanonicalPath(char *path)
{
	return strdup(path);
}
#endif /* WIN32 */


// Argument structures for parser.
typedef struct _pattern
{
    char            *str;
    struct _pattern *next;
} Pattern;

typedef struct _ArgStruct
{
    char       *commandName;  /* name of command */
    char       *workprint;    /* name of workprint file to build */
    char       *tags;         /* Digital Workprint tags */
    MlBoolean  dumpRegistry;  /* Dump internal registries. */
	MlBoolean  verbose;       /* Be verbose. */
} ArgStruct;


const char *usage_str = "\
Syntax:   DWPChecker [-v] -r | [-t <tags>] <workprint>\n\
\n\
          -v                   Be verbose\n\
          -r                   Dump registries\n\
          -t <tags>            Digital Workprint tags\n\
          <workprint>          Digital Workprint\n\
\n\
Function: Check the specified Digital Workprint for syntactic and symantic conformance.\n";


int parseArgs(int argc, char *argv[], ArgStruct *args)
{
    /* Declare local variables. */
    int c;
    int errflg;
    extern char *optarg;
    extern int optind;

    errflg = 0;
    while ((c = getopt(argc, argv, "vrt:")) != -1)
	{
        switch (c)
		{
		  case 'r':
			/* Dump internal registries. */
			args->dumpRegistry = TRUE;
			break;
		  case 't':
			if (! args->tags)
			    args->tags = strdup(optarg);
			break;
          case 'v':
        	// Set verbosity flag.
        	args->verbose = TRUE;
        	break;
          case '?':
            errflg++;
        }
    }

    if (errflg)
	{
        (void)fprintf(stderr, "%s\n", usage_str);
        return FALSE;
    }

    for ( ; optind < argc; optind++)
	{
    	// The -r option is mutually exclusive to <workprint> specification.
    	if (args->dumpRegistry)
    		return FALSE;

        if (! args->workprint)
		{
            args->workprint = getCanonicalPath(argv[optind]);
        } else
		{
            fprintf(stderr,"%s\n",usage_str);
            return FALSE;
        }
    }

    /* If there is no specified workprint, complain. */
    if ((args->dumpRegistry == FALSE) && (args->workprint == NULL))
	{
        fprintf(stderr,"%s\n",usage_str);
        return FALSE;
    }

    /* Having made it to here implies that we have good arguments. */

    return TRUE;
}


//extern MleDwpDatatypeDict *g_registry;

void printDwpDatatypeDictionary(ArgStruct *args)
{
	if (args->verbose)
		fprintf(stdout, "*** Dumping Registered DwpDatatypes ***\n");
	MleDwpDatatype::dumpRegistry();
}


int main(int argc,char **argv)
{
    // Declare local variables.
    ArgStruct args;

    // Parse arguments.
    args.commandName = argv[0];
    args.workprint = NULL;
    args.tags = NULL;
	args.dumpRegistry = FALSE;
	args.verbose = FALSE;
    if (! parseArgs(argc, argv, &args))
	{
        exit(1);
    }

	// Initialize the DWP API.
    mleDwpInit();

    if (args.dumpRegistry)
    {
    	// Dump the internal DWP registries.
    	if (args.verbose)
    		fprintf(stdout, "*** Dumping DWP Registries ****\n");
    	printDwpDatatypeDictionary(&args);
    } else
    {
    	// Check the Digital Workprint.

	    // Open the DWP.
	    MleDwpInput in;
	    if (in.openFile(args.workprint))
	    {
		    fprintf(stderr, "DwpChecker: can't open file '%s'\n", args.workprint);
		    return 1;
	    }
    
	    // Read the DWP.
	    MleDwpItem *root = new MleDwpItem;
	    while ( MleDwpItem::read(&in,root) )
		    ;
		
	    // Check the semantics of the DWP.
	    MleDwpChecker checker(root);
	    MlErr err = checker.check();
	    if (err)
	    {
		    fprintf(stderr, "DwpChecker: file '%s': %s\n",
		            args.workprint, checker.getErrorMsg());
		    return err;
	    }

	    // Return status of 0 indicates that there were no known issues with
	    // the specified Digital Workprint.
        return 0;
    }
}
