#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
bin_PROGRAMS=DWPChecker

# Because a.out is only a sample program we don't want it to be installed.
# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
# noinst_PROGRAMS=DWPChecker

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

ACLOCAL_AMFLAGS=-I ../m4

# Sources for the a.out 
DWPChecker_SOURCES= ../../common/test/dwpCheckerTest.cpp

# Libraries for a.out
DWPChecker_LDADD = \
	$(top_srcdir)/libDWP/libDWP.la \
	$(MLE_ROOT)/lib/libmlutil.la \
	$(MLE_ROOT)/lib/libmlmath.la \
	-ldl

# Linker options for a.out
DWPChecker_LDFLAGS = -rpath `cd $(top_srcdir);pwd`/libDWP/.libs

# Compiler options for a.out
DWPChecker_CPPFLAGS = \
	-DMLE_NOT_UTIL_DLL \
	-DMLE_NOT_MATH_DLL \
	-DMLE_NOT_DWP_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../common/include \
	-I$(MLE_ROOT)/include \
	-std=c++17

if HAVE_LOG4CXX
#DWPChecker_SOURCES += \
#    source.h \
#    source.cpp
	
DWPChecker_CPPFLAGS += $(LOG4CXX_CFLAGS)
#DWPChecker_CPPFLAGS += -DLINUX -D_REENTRANT -D_GNU_SOURCE -I/usr/local/include -I/usr/include/apr-1.0

DWPChecker_LDFLAGS += -Wl,-rpath=/usr/local/lib
DWPChecker_LDADD += $(LOG4CXX_LIBS)
#DWPChecker_LDADD += -L/usr/local/lib -llog4cxx
endif
