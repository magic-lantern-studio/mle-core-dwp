Design Note: WorkPrint API Primer

Magic Lantern Game Authoring System 1.0

Mark S. Millard
March 23, 2004

-------------------------------------------------------------------------------

Contents

   * Intro
   * Workprint Organization and Structure
   * Base Class
   * Finding Items in a Workprint
   * Creating Workprint Item Types

Introduction

This document describes an API for accessing digital workprints and the method
for extending what types they can store.

Workprint Organization and Structure

A workprint is organized as a containership of workprint items. A workprint
item is a fundamental unit of a workprint, a building block of the workprint
structure. Any workprint item can be a container for zero or more other
workprint items.

Workprint containership is generic, which simply means that there is no type
discrimination on contained items. An item may contain subitems of all the same
type or of all different types. While the workprint mechanism implements
generic containership, however, applications can impose policy on what item
types can be contained within other item types. For example, the Magic Lantern
design specifies that Property items occur within Actor items. Putting a
Property inside a Forum is a semantic error in FirewalkerMAgic Lantern.

The term "workprint" is often used to refer to both the file system
representation and the in-memory representation of the structure. These
representations store the same information, but it is often confusing when the
context of the term requires a distinction.

Containership is quite apparent in the workprint file, which uses a LISP-like
syntax:

    (Group group0
        (ActorSet hudForum
            (Actor speedometer Dial
               (Property value 0.6)
            )
            (Actor target CrossHair)
        )
    )

The first word after a left parenthesis is always a workprint item type. After
that comes data specific to that type (for example the Actor name and type),
and then a list of contained items.

In a file, there may be several items at the top level:

    (Forum ... )
    (Group group0 ... )
    (Group group1 ... )

which makes this workprint look like a set of containers instead of a single
container. However, it really is most appropriate to think of this as a single
containership with the outermost container being the file.

In the in-memory workprint, the workprint items have the same relationship as
they do in a workprint file. In the in-memory workprint it is common to picture
the containership as a tree. These metaphors are equivalent, as there is an
identical mapping of "is contained by" and "is a child of" relations. The tree
paradigm is convenient for the in-memory workprint because it more closely
reflects the implementation.

Base Class

The base class for all workprint items is MleDwpItem. The basic capabilities
defined in the base class are identity, navigation, structure editing, and I/O.
Identity functions are how an item describes itself. Because containership is
generic, an application traversing a workprint has no idea what an item is when
it encounters one. Member functions on the item allow querying the item for
information to identify its type.

    virtual const char *getTypeName(void) const;
    virtual MleDwpType getTypeId(void) const;
    virtual int isa(const char *type) const;
    virtual int isa(MleDwpType) const;

The getTypeName() function returns the specific item type as a string. This is
the full class name, not the truncated name used in the workprint file (e.g.
MleDwpActor instead of Actor). The isa() function returns nonzero if the item
is an instance of the class whose name is passed in, or an instance of a class
descendant of that class. Typically, however, new item classes will subclass
directly from MleDwpItem. Here is an example of the use of these functions:

    // assume item is a pointer to some MleDwpItem
    // see if this is an actor
    if ( item->isa("MleDwpActor") )
        printf("item is an actor.\n");
    else
        printf("item is a %s.\n",item->getTypeName());

The alternate versions of these functions use a MleDwpType which is a uniquee
type identifier (as opposed to a string). This makes type comparison more
efficient as it can be equality testing instead of string matching. It also
allows the compiler to catch type misspellings. A minor disadvantage is that
the type ids are not easily interpreted in a debugger.

In addition, each item can optionally be given a string name.

    const char *getName(void) const;
    void setName(const char *name);

The value that getName() returns if setName() has not been called is currently
undefined. Do not assume that getName() will return NULL (or anything else) if
setName() has not been called. The spaces of names beginning with "0x", "ml",
"ML", and "Mle" are reserved.

Beyond the reservation of some namespaces, the library has no policy on naming.
Item subclasses and applications can define their own policy for names, such as
how they are scoped and whether they must be defined.

Navigation refers to moving around the workprint structure from a given item.
MleDwpItem uses a typical tree model of pointers to parent, previous sibling,
next sibling, first child, and last child:

    MleDwpItem *getParent(void) const;
    MleDwpItem *getPrev(void) const;
    MleDwpItem *getNext(void) const;
    MleDwpItem *getFirstChild(void) const;
    MleDwpItem *getLastChild(void) const;

A tree of items may be traversed using recursion:

    void traverse(MleDwpItem *item)
    {
        // do stuff on item here for preorder traversal

        // get the list of children
        MleDwpItem *child = item->getFirstChild();

        // loop through the children
        while ( child )
        {
            // recurse on child
            traverse(child);

            // step to the next child
            child = child->getNext();
        }
        // do stuff on item here for postorder traversal
    }

Or the root of a tree can be found by iteration:

    // start with the local item
    MleDwpItem *root = item;

    // loop while there is a parent
    while ( root->getParent() )
        root = root->getParent();  // become the parent

    // root is now the top of the tree

Convenience functions could be defined to perform common operations like this.
At this time, however, such functions are not included in the item base class.

Structure editing allows insertion and removal of children on an item.

    void insertChild(MleDwpItem *child,MleDwpItem *beforeChild = NULL);
    void removeChild(MleDwpItem *whichChild);

The routine insertChild() makes a linkage for a new subitem. The new item will
be placed before the optionally indicated existing subitem in the subitem list,
or at the end if omitted. The routine removeChild() removes the linkage for a
subitem. Note that removeChild() does not delete the subitem.

I/O routines convert between the in-memory workprint and the workprint file.
Reading is done from a MleDwpInput object, which abstracts away whether the
source of the workprint is a file, a pipe, a memory buffer, etc. For the
purposes of this document, input is assumed to come from a file. The code to
set up a MleDwpInput object to use a file looks like:

    #include "mle/DwpInput.h"
    ...
    MleDwpInput *in = new MleDwpInput; // create the input object
    in->openFile(filename);        // open a file, filename is a char *

openFile() returns zero if the file was opened successfully, nonzero in the
case of an error.

The routine to read an item from an input object is a static member function on
MleDwpItem:

    static MleDwpItem *read(MleDwpInput *in,MleDwpItem *parent = NULL);

The read() routine returns a pointer to the item it reads, NULL if an item
could not be read (which is typically the end of file condition). An optional
side effect is to make the item that was read the child of an item whose
pointer is passed in. A succinct way to read in a workprint after setting up
the input object is:

    // assumes MleDwpInput object has been created and initialized

    MleDwpItem *root = new MleDwpItem; // create a root item

    // loop while we can read items
    while ( MleDwpItem::read(in,root) )
        ;

Writing is done through a MleDwpOutput object, the dual of MleDwpInput. The output
object abstracts away whether the output is to a file, a pipe, a memory buffer,
etc. For the purposes of this document, output is assumed to go to a file. The
code to set up a MleDwpOutput object to use a file looks like:

    #include "mle/DwpOutput.h"
    ...
    MleDwpOutput *out = new MleDwpOutput; // create the output object
    out->openFile(filename); // open a file, filename is a char *

openFile() returns zero if the file was opened successfully, nonzero in the
case of an error.

The routine to write an item to an output object is a regular member function
on the MleDwpItem (not a static member function like read()):

    virtual int write(MleDwpOutput *out);

The write() routine writes the item to the output and returns nonzero on an
error. The workprint just read in the example above could be written out by:

    // assumes MleDwpOutput object has been created and initialized

    root->write(out);

This would add one level of hierarchy to the workprint by putting the root item
in the file. To preserve identical structure, the code would be:

    // assumes MleDwpOutput object has been created and initialized

    // loop over all the children
    MleDwpItem *item = root->getFirstChild(); // get the first child
    while ( item )
    {
        item->write(out);
        item = item->getNext();
    }

It is likely that convenience API will be written to take care of the details
of reading and writing workprint files. For the present, however, this is how
that should be accomplished.

Finding Items in a Workprint

Items of a particular type or name can be found by traversing the workprint and
using the appropriate identity member functions to match data. However, because
this is a common operation, a helper class called MleDwpFinder has been created
to do this.

A finder is created, configured, executed, and queried. Creation is typically
done by allocation on the stack. Configuration is accomplished either during
construction or by member functions:

    void setType(MleDwpType type);
    void setName(const char *name);
    void setCallback(int (*cb)(MleDwpItem *,void *),void *data);
    void setFindAll(int findAllFlag);

A search for item(s) of a particular item type is done by calling setType()
with the class type. The class type for any item is the static member variable
typeId on the item class, e.g. MleDwpProperty::typeId. Omitting the call or
passing NULL means that type will be disregarded in the find execution.

A search for item(s) of a particular name is done by calling setName() with the
name. Omitting the call or passing NULL means that the name will be disregarded
in the find execution.

The application can set up its own discrimination function and pass that in by
calling setCallback(). The function should take an item pointer and client
data, and return nonzero if the item matches the find criteria. Omitting the
call or passing NULL for the function pointer means that no
application-specific discrimination will be done.

These configurations may be done in combination, in which case an item must
pass all activated criteria in order to be found. For example, if both type and
name are specified, only items that match both type and name will be returned.
The criteria are applied in the order - type, name, application callback - and
testing is terminated when any one fails. So if type and application callback
are active, only items of the right type will be passed to the callback.

An orthogonal configuration option is to specify whether the find execution
terminates after finding an item that meets the criteria or finds all the items
that match. The routine setFindAll() selects finding all items if nonzero is
passed, termination after the first matching item in depth first preorder
traversal otherwise (the default).

As a convenience, the constructor for MleDwpFindItem can be used to do some
common configurations:

    MleDwpFinder(MleDwpType type = NULL,const char *name = NULL,
        int findAll = 0);

The effect of passing type, name, or findAll flag is the same as calling the
respective configuration function. User discrimination functions cannot be
specified in the constructor.

Finding is executed by a member function on the finder:

    MleDwpItem *find(const MleDwpItem *item);

A pointer to the place in the workprint to begin the search for matching items
is passed to the find() function. The first item found, NULL if none, is
returned by this function as a convenience. After the find has been executed,
the finder may be queried to extract the items found.

    int getNumItems(void) const;
    const MleDwpItem **getItems(void) const;

The number of items found is returned by getNumItems(). An array of the item
pointers found can be obtained by calling getItems(). The array returned is in
memory allocated by the finder object and will be deallocated when the finder
is destroyed.

Here is an example that finds the Group item named group0 in a workprint:

    // assume root is the workprint root item

    // create and configure the finder
    MleDwpFinder finder(MleDwpGroup::typeId,"group0");

    // execute and retrieve the found item
    MleDwpItem *group = finder.find(root);

and here is an example that prints the names of all the Actor items in a
workprint:

    // assume root is the workprint root item

    // create and configure the finder
    MleDwpFinder finder(MleDwpActor::typeId,NULL,1);

    // execute the finder
    finder.find(root);

    // loop over the items found
    int numItems = finder.getNumItems();
    MleDwpItem **items = finder.getItems();
    for ( int i = 0; i < numItems; i++ )
        printf("Actor %s found.\n",items[i]->getName());

Creating Workprint Item Types

Creating new workprint item types is done by subclassing. The minimal workprint
item class has no data of its own, acting only as a container for other items.
This minimal class still needs to implement the basic identity functions,
however, to be able to describe itself properly. There are macros in WpItem.h
that make this easy:

    MLE_DWP_HEADER(<class name>);
    MLe_DWP_SOURCE(<class name>,<superclass name>);

The new class should be a descendant of MleDwpItem, and the MLE_DWP_HEADER()
macro should be placed in the class definition. The unquoted name of the new
class is the argument to the macro. The MLE_DWP_SOURCE() macro should be placed
in the class implementation, normally the .c++ file. The arguments to this
macro are the unquoted names of the new class and the immediate superclass of
the new class.

MinimalItem.h:

    #include "mle/DwpItem.h"
    class MinimalItem : public MleDwpItem
    {
        MLE_DWP_HEADER(MinimalItem);
    };

MinimalItem.c++:

    #include "MinimalItem.h"
    MLE_DWP_SOURCE(MinimalItem,MleDwpItem);

This defines a new workprint item called MinimalItem. There are several ways to
activate this item. If this is a new basic system item type, then you can add
it to the workprint library initialization function. Edit Wp.c++ and add the
header file and a call to MinimalItem::initClass() in mlDwpInit().
Alternatively, you can simply call the initClass function in every application
that uses this type. The last method is to create a dynamic shared object
(DSO), and let the code be loaded automatically when an item of that type is
encountered. To do this, add MyItem.so to your Makefile targets and add the
rule:

.o.so:
    $(LD) $(LDDSOOPTS) -o $@ $?

The workprint library is capable of reading in things it doesn't know about, so
DSO loading probably shouldn't be needed very often.

Putting data for an item into the workprint requires adding two more member
functions:

    virtual int readContents(MleDwpInput *in);
    virtual int writeContents(MleDwpOutput *out);

The readContents() and writeContents() functions read and write any data
specific to this type of item. Nonzero should be returned if there was an
error. This is everything in a workprint file except the item type and any
contained items. For example:

    (Actor foo MyActor
        (Property fuel 10.0)
    )

The Actor readContents() and writeContents() is responsible for transcribing
the actor name (foo) and the actor type (MyActor). These routines do not need
to worry about anything else - the system will take care of reading or writing
the item type and invoking the transcription of the Property subitem.

MleDwpInput and MleDwpOutput are abstract i/o objects that provide services to
help with transcription of data. A string, int, float, or char can be read or
written. Here are the i/o routines for MleDwpActor:

int
MleDwpActor::readContents(MleDwpInput *in)

    char buffer[MLE_DWP_INPUT_BUFSIZE];

    // read the actor name
    if ( in->readString(buffer) )
    {
        in->reportError("Actor couldn't read name");
        n->skip();  // try to skip to the end of this item
        return 1;
    }
    setName(buffer);

    // read the actor class
    if ( in->readString(buffer) )
    {
        in->reportError("Actor couldn't read class");
        in->skip(); // try to skip to the end of this item
        return 1;
    }
    setActorClass(buffer);
    return 0;


int
MleDwpActor::writeContents(MleDwpOutput *out)

    // write the actor name
    out->writeString(getName());

    // write the actor class
    out->writeString(getActorClass());
    return 0;


Note that you must supply your own buffer for string reading. Also, strings
that may have embedded spaces or special characters should be written with
writeFancyString() instead of writeString(). The special characters in the
workprint are '(' (open paren), ')' (close paren), '#' (hash), newline, and
double quotes.

Large input lists can be handled with an input table mechanism. This is simply
a table-driven sequence of reads. One additional type provided with the table
is a WpToken which will return an error if the input does not match the
supplied string.

    // this is a member function for MleDwpInput
    int readTable(const MleDwpInputTable *table,int n);

The table definition is:

// This is the enumeration for table driven input.
typedef enum

        MleDwpInputEnd,
        MleDwpInputToken,
        MleDwpInputInt,
        MleDwpInputFloat,
        MleDwpInputString
} MleDwpInputType;

// This is the element type for table driven input.  An array of this
//   structure may be passed to the input lexer.
struct MleDwpInputTable

        MleDwpInputType type;
        void *ptr;
};

Typical usage is to create a static array of MleDwpInputTable with initialized
data, with the last element having a MleDwpEnd. This example shows how a new
item might implement its readContents() function using readTable(), where its
input syntax has the string "test", an int, a float, and a string.

int MyItem::readContents(MleDwpInput *in)

    int d;
    float f;
    char s[MLE_DWP_INPUT_BUFSIZE];
    static MleDwpInputTable table[] = {
        MleDwpInputToken, "test",
        MleDwpInputInt, &d,
        MleDwpInputFloat, &f,
        MleDwpInputString, s,
        MleDwpInputEnd
    };

    if ( in->readTable(table) )
    {
        // the table handler will have already called reportError()
        in->skip();
        return 1;
    }

    // store away the input parameters here
    ...

    return 0;


If an unrecoverable error is discovered on read, the reading routine should
report the error using the reportError() member function on the input object
and try to skip over the object using skip().

Writing the contents of an item is quite a bit easier than reading. Some
attention should be paid to formatting, however. The member functions
adjustIndent() and writeTerminal() on the output object should be used to
provide neat transcription. adjustIndent() changes the indentation level by its
integer argument (usually either +1 or -1), which takes place after every
writeTerminal(). Items are automatically preindented to their level of
containership, so short items might not require any additional formatting.
