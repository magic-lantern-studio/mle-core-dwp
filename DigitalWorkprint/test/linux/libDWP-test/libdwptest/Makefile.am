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

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libdwptest_la_CPPFLAGS = \
	-DML_MATH_DEBUG=0 \
	-DML_FIXED_POINT=0 \
	-DMLE_NOT_DLL \
	-DMLE_REHEARSAL \
	-I$(top_srcdir)/include \
	-I$(MLE_HOME)/DigitalWorkprint/lib/common/include \
	-I$(MLE_HOME)/DigitalWorkprint/lib/linux/include \
	-I$(MLE_HOME)/Core/util/common/include \
	-I$(MLE_HOME)/Core/util/linux/include \
	-I$(MLE_HOME)/Core/math/common/include \
	-I$(MLE_HOME)/Core/mlert/common/src/foundation \
	-std=c++11

if HAVE_LOG4CXX
#libdwptest_la_SOURCES += \
#    audio_random_portaudio.h \
#    audio_random_portaudio.cpp

libdwptest_la_CPPFLAGS += $(LOG4CXX_CFLAGS)

libdwptest_la_LDADD += $(LOG4CXX_LIBS)
endif
