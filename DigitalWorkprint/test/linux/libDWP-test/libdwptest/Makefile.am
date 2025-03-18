#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libdwptest.la

#######################################
# Build information for each library

# Sources for libdwptest
libdwptest_la_SOURCES = \
    libdwptest.cxx testString.cxx testInt.cxx testFloat.cxx \
    testMlVector2.cxx testMlVector3.cxx testMlVector4.cxx \
    testFloatArray.cxx

# Linker options libTestProgram
libdwptest_la_LDFLAGS = 

libdwptest_la_LIBDADD =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libdwptest_la_CPPFLAGS = \
	-DML_MATH_DEBUG=0 \
	-DML_FIXED_POINT=0 \
	-DMLE_NOT_UTIL_DLL \
	-DMLE_NOT_MATH_DLL \
	-DMLE_NOT_DWP_DLL \
	-DMLE_REHEARSAL \
	-I$(top_srcdir)/include \
	-I$(MLE_ROOT)/include \
	-std=c++17

if HAVE_LOG4CXX
#libdwptest_la_SOURCES += \
#    audio_random_portaudio.h \
#    audio_random_portaudio.cpp

libdwptest_la_CPPFLAGS += $(LOG4CXX_CFLAGS)

libdwptest_la_LIBDADD += $(LOG4CXX_LIBS)
endif
