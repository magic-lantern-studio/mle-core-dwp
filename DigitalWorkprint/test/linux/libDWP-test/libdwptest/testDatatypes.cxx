// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2016-2018 Wizzer Works
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

// Table driven tests for every datatype in DWPType.
//
// Datatypes are tested directly, without a workprint: a value is read from
// text into a MleDwpDataUnion, written back out, and compared. Adding a
// datatype is one row in g_types, and adding a value is one row in g_values.

#include <string.h>
#include <string>
#if defined(__GLIBC__)
#include <malloc.h>
#endif
#include <vector>
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "math/vector.h"
#include "mle/mlMalloc.h"
#include "mle/mlArray.h"

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpDatatype.h>
#include <mle/DwpDataUnion.h>
#include <mle/DwpEnumDef.h>
#include <mle/DwpFinder.h>
#include <mle/DwpInput.h>
#include <mle/DwpOutput.h>
#include <mle/DwpProperty.h>
#include <mle/DwpInt.h>
#include <mle/DwpFloat.h>
#include <mle/DwpFloat3.h>
#include <mle/DwpFloat4.h>
#include <mle/DwpScalar.h>
#include <mle/DwpVector2.h>
#include <mle/DwpVector3.h>
#include <mle/DwpVector4.h>
#include <mle/DwpRotation.h>
#include <mle/DwpTransform.h>
#include <mle/DwpMatrix34.h>
#include <mle/DwpIntArray.h>
#include <mle/DwpFloatArray.h>
#include <mle/DwpScalarArray.h>
#include <mle/DwpVector3Array.h>
#include <mle/DwpString.h>
#include <mle/DwpEnum.h>
#include <mle/DwpVoidStarType.h>

// Include Digital Workprint test header files.
#include "libdwptest.h"

namespace datatypetest {

// How a datatype hands its value to and from caller memory with get() and set().
enum CopyKind {
	CopyRaw,      // The value is copied into a buffer of getSize() bytes.
	CopyString,   // get() allocates a copy of the string, set() copies it.
	CopyArray     // The value is copied into an MleArray (tested separately).
};

struct TypeInfo {
	const char *name;                 // The name in a workprint.
	MleDwpDatatypeType typeId;
	int size;                         // The expected getSize().
	CopyKind copy;
	const MleDwpDatatype *(*get)(void);
};

#define REGISTERED(NAME, CLASS, SIZE, COPY) \
	{ NAME, CLASS::typeId, SIZE, COPY, \
	  [](void) -> const MleDwpDatatype * { return MleDwpDatatype::findType(NAME); } }

const int g_arraySize = (int)sizeof(MleArray<char>);

const TypeInfo g_types[] = {
	REGISTERED("int",         MleDwpInt,        4,  CopyRaw),
	REGISTERED("float",       MleDwpFloat,      4,  CopyRaw),
	REGISTERED("MlScalar",    MleDwpScalar,     4,  CopyRaw),
	REGISTERED("MlVector2",   MleDwpVector2,    8,  CopyRaw),
	REGISTERED("MlVector3",   MleDwpVector3,    12, CopyRaw),
	REGISTERED("MlVector4",   MleDwpVector4,    16, CopyRaw),
	REGISTERED("MlRotation",  MleDwpRotation,   16, CopyRaw),
	REGISTERED("MlTransform", MleDwpTransform,  48, CopyRaw),
	REGISTERED("enum",        MleDwpEnum,       4,  CopyRaw),
	REGISTERED("IntArray",    MleDwpIntArray,   g_arraySize, CopyArray),
	REGISTERED("FloatArray",  MleDwpFloatArray, g_arraySize, CopyArray),
	REGISTERED("string",      MleDwpString,     (int)sizeof(char *), CopyString),
	REGISTERED("MediaRef",    MleDwpMediaRefType, (int)sizeof(void *), CopyString),

	REGISTERED("float3",         MleDwpFloat3,       12, CopyRaw),
	REGISTERED("float4",         MleDwpFloat4,       16, CopyRaw),
	REGISTERED("MlMatrix34",     MleDwpMatrix34,     48, CopyRaw),
	REGISTERED("ScalarArray",    MleDwpScalarArray,  g_arraySize, CopyArray),
	REGISTERED("Vector3Array",   MleDwpVector3Array, g_arraySize, CopyArray),
	REGISTERED("void*",          MleDwpVoidStar,     (int)sizeof(void *), CopyString),
};

const TypeInfo *findTypeInfo(const std::string &name) {
	for (const TypeInfo &t : g_types)
		if (name == t.name)
			return &t;
	return NULL;
}

// The value written is the text after the "#DWP" header line, without the
// trailing blank.
std::string body(const MleDwpDatatype *datatype, MleDwpDataUnion *data) {
	MleDwpOutput out;
	datatype->write(&out, data);
	std::string text(out.getBuffer());
	text.erase(0, text.find('\n') + 1);
	while (! text.empty() && isspace((unsigned char)text.back()))
		text.pop_back();
	return text;
}

// Read a bare value, without the "#DWP" header. The text must stay alive
// while the data is read.
int readValue(const MleDwpDatatype *datatype, const std::string &text,
	MleDwpDataUnion *data) {
	MleDwpInput in(0);
	if (in.setBuffer(text.c_str()))
		return -1;
	return datatype->read(&in, data);
}

// A value to read, and what it is written back as.
struct ValueCase {
	const char *label;
	const char *type;
	const char *input;
	const char *expected;
};

const ValueCase g_values[] = {
	// int: zero, negative, hex and the 32 bit limits.
	{ "IntPositive",   "int", "42",          "42" },
	{ "IntZero",       "int", "0",           "0" },
	{ "IntNegative",   "int", "-7",          "-7" },
	{ "IntHex",        "int", "0x10",        "16" },
	{ "IntMax",        "int", "2147483647",  "2147483647" },
	{ "IntMin",        "int", "-2147483648", "-2147483648" },
	{ "IntHexBitPattern", "int", "0xFFFFFFFF", "-1" },
	{ "IntHexSignBit",    "int", "0x80000000", "-2147483648" },
	{ "IntOctalBitPattern", "int", "037777777777", "-1" },

	// float and MlScalar: zero, negative zero, very large and very small.
	{ "FloatFraction", "float", "0.25",   "0.25" },
	{ "FloatZero",     "float", "0",      "0" },
	{ "FloatNegZero",  "float", "-0.0",   "-0" },
	{ "FloatNegative", "float", "-1.5",   "-1.5" },
	{ "FloatLarge",    "float", "1e30",   "1e+30" },
	{ "FloatSmall",    "float", "1e-30",  "1e-30" },
	{ "FloatMax",      "float", "3.4e38", "3.4e+38" },
	{ "FloatTiny",     "float", "1e-38",  "1e-38" },
	{ "ScalarPositive","MlScalar", "2.5",  "2.5" },
	{ "ScalarNegative","MlScalar", "-0.5", "-0.5" },

	{ "Vector2",         "MlVector2", "1 2",            "1 2" },
	{ "Vector2Negative", "MlVector2", "-1.5 0",         "-1.5 0" },
	{ "Vector3",         "MlVector3", "1 2 3",          "1 2 3" },
	{ "Vector3Zero",     "MlVector3", "0 0 0",          "0 0 0" },
	{ "Vector3Large",    "MlVector3", "1e30 -1e30 1e-30", "1e+30 -1e+30 1e-30" },
	{ "Vector4",         "MlVector4", "1 2 3 4",        "1 2 3 4" },
	{ "Vector4Negative", "MlVector4", "-1 -2 -3 -4",    "-1 -2 -3 -4" },
	{ "Rotation",        "MlRotation", "0 0 1 3.14",    "0 0 1 3.14" },
	{ "RotationZero",    "MlRotation", "0 0 0 0",       "0 0 0 0" },
	{ "TransformIdentity", "MlTransform", "1 0 0 0 1 0 0 0 1 0 0 0", "1 0 0 0 1 0 0 0 1 0 0 0" },
	{ "TransformMoved",    "MlTransform", "1 0 0 0 1 0 0 0 1 -5 6.5 7", "1 0 0 0 1 0 0 0 1 -5 6.5 7" },

	{ "Enum",          "enum", "3",  "3" },
	{ "EnumZero",      "enum", "0",  "0" },
	{ "EnumNegative",  "enum", "-1", "-1" },

	// Arrays, with and without blanks around the comma, and empty.
	{ "IntArraySingle",   "IntArray", "[ 5 ]",             "[ 5 ]" },
	{ "IntArrayMany",     "IntArray", "[ 1 , 2 , 3 ]",     "[ 1 , 2 , 3 ]" },
	{ "IntArrayNegative", "IntArray", "[ -1 , 0 , 1 ]",    "[ -1 , 0 , 1 ]" },
	{ "IntArrayCommaAfterNumber", "IntArray", "[ 1, 2, 3 ]", "[ 1 , 2 , 3 ]" },
	{ "IntArrayNoBlanks",  "IntArray", "[1,2,3]",          "[ 1 , 2 , 3 ]" },
	{ "IntArrayEmpty",     "IntArray", "[ ]",              "[ ]" },
	{ "IntArrayEmptyNoBlank", "IntArray", "[]",            "[ ]" },
	{ "FloatArrayEmpty",   "FloatArray", "[ ]",            "[ ]" },
	{ "FloatArrayCommaAfterNumber", "FloatArray", "[ 1.0, 2.0 ]", "[ 1 , 2 ]" },
	{ "FloatArraySingle", "FloatArray", "[ 1.5 ]",         "[ 1.5 ]" },
	{ "FloatArrayMany",   "FloatArray", "[ 0.5 , -2 , 1e30 ]", "[ 0.5 , -2 , 1e+30 ]" },

	{ "String",          "string", "\"hello world\"", "\"hello world\"" },
	{ "StringEmpty",     "string", "\"\"",            "\"\"" },
	{ "StringSymbols",   "string", "\"a/b-c_d.txt\"", "a/b-c_d.txt" },
	{ "MediaRef",        "MediaRef", "\"a/b.jpg\"",   "a/b.jpg" },

	{ "Float3",          "float3", "1 2 3",   "1 2 3" },
	{ "Float4",          "float4", "1 2 3 4", "1 2 3 4" },
	{ "Matrix34",        "MlMatrix34", "1 0 0 0 1 0 0 0 1 4 5 6", "1 0 0 0 1 0 0 0 1 4 5 6" },
	{ "ScalarArray",     "ScalarArray", "[ 1.5 , 2.5 ]", "[ 1.5 , 2.5 ]" },
	{ "Vector3Array",    "Vector3Array", "[ 1 2 3 , 4 5 6 ]", "[ 1 2 3 , 4 5 6 ]" },
	{ "Vector3ArrayComma", "Vector3Array", "[ 1 2 3, 4 5 6 ]", "[ 1 2 3 , 4 5 6 ]" },
	{ "ScalarArrayEmpty", "ScalarArray", "[ ]", "[ ]" },
	{ "Vector3ArrayEmpty", "Vector3Array", "[ ]", "[ ]" },
	{ "VoidStar",        "void*", "\"x/y\"",  "x/y" },
};

// Input that must be rejected.
const ValueCase g_invalid[] = {
	{ "IntWord",           "int",        "abc",          NULL },
	{ "IntAboveMax",       "int",        "2147483648",   NULL },
	{ "IntFarAboveMax",    "int",        "4294967296",   NULL },
	{ "IntBelowMin",       "int",        "-2147483649",  NULL },
	{ "IntHexAbove32Bits", "int",        "0x100000000",  NULL },
	{ "IntNegativeHexPattern", "int",    "-0xFFFFFFFF",  NULL },
	{ "IntTooManyDigits",  "int",        "99999999999999999999", NULL },
	{ "EnumTooLarge",      "enum",       "4294967296",   NULL },
	{ "IntArrayElementTooLarge", "IntArray", "[ 1 , 4294967296 ]", NULL },
	{ "FloatWord",         "float",      "xyz",          NULL },
	{ "Vector3Word",       "MlVector3",  "1 two 3",      NULL },
	{ "TransformWord",     "MlTransform","1 0 0 0 1 0 0 0 1 0 0 x", NULL },
	{ "EnumWord",          "enum",       "red",          NULL },
	{ "IntArrayNoComma",   "IntArray",   "[ 1 2 ]",      NULL },
	{ "IntArrayNoBracket", "IntArray",   "1 , 2",        NULL },
	{ "IntArrayUnclosed",  "IntArray",   "[ 1 , 2",      NULL },
	{ "Vector3Truncated",  "MlVector3",  "1 2",          NULL },
	{ "IntNothing",        "int",        "",             NULL },
	{ "FloatArrayWord",    "FloatArray", "[ x ]",        NULL },
};

template <class T>
std::string caseName(const ::testing::TestParamInfo<T> &info) {
	return info.param.label;
}

} // namespace datatypetest

using namespace datatypetest;

// ---------------------------------------------------------------------------
// Tests of each datatype.

class DatatypeTypeTest : public ::testing::TestWithParam<TypeInfo> {};

TEST_P(DatatypeTypeTest, Identity) {
	const TypeInfo &info = GetParam();
	const MleDwpDatatype *datatype = info.get();
	ASSERT_TRUE(datatype != NULL);
	EXPECT_STREQ(info.name, datatype->getName());
	EXPECT_TRUE(datatype->isa(info.typeId));
	EXPECT_EQ(info.size, datatype->getSize());
}

TEST_P(DatatypeTypeTest, IsRegistered) {
	const TypeInfo &info = GetParam();
	const MleDwpDatatype *found = MleDwpDatatype::findType(info.name);
	ASSERT_TRUE(found != NULL) << info.name << " is not registered";
	EXPECT_EQ(found, MleDwpDatatype::findType(info.name));
	EXPECT_STREQ(info.name, found->getName());
}

TEST_P(DatatypeTypeTest, OtherTypesAreNotThisType) {
	// Asking for the wrong datatype fails. The one exception is MediaRef,
	// which is a kind of void*.
	const TypeInfo &info = GetParam();
	const MleDwpDatatype *datatype = info.get();
	ASSERT_TRUE(datatype != NULL);
	for (const TypeInfo &other : g_types) {
		if (std::string(other.name) == info.name)
			continue;
		bool expected = std::string(info.name) == "MediaRef" &&
			std::string(other.name) == "void*";
		EXPECT_EQ(expected, datatype->isa(other.typeId) != 0)
			<< info.name << " as " << other.name;
	}
}

TEST_P(DatatypeTypeTest, ArrayTypesDeriveFromArray) {
	const TypeInfo &info = GetParam();
	const MleDwpDatatype *datatype = info.get();
	ASSERT_TRUE(datatype != NULL);
	EXPECT_EQ(info.copy == CopyArray, datatype->isa(MleDwpArray::typeId) != 0);
}

INSTANTIATE_TEST_SUITE_P(Datatypes, DatatypeTypeTest,
	::testing::ValuesIn(g_types),
	[](const ::testing::TestParamInfo<TypeInfo> &info) {
		std::string name(info.param.name);
		for (char &c : name)
			if (! isalnum((unsigned char)c))
				c = '_';
		return name;
	});

// ---------------------------------------------------------------------------
// Tests of each value.

class DatatypeValueTest : public ::testing::TestWithParam<ValueCase> {
  protected:
	const TypeInfo *info() { return findTypeInfo(GetParam().type); }
};

TEST_P(DatatypeValueTest, ReadAndWrite) {
	ASSERT_TRUE(info() != NULL);
	const MleDwpDatatype *datatype = info()->get();
	ASSERT_TRUE(datatype != NULL);

	std::string text(GetParam().input);
	MleDwpDataUnion data;
	ASSERT_EQ(0, readValue(datatype, text, &data));
	EXPECT_EQ(datatype, data.m_datatype);
	EXPECT_EQ(std::string(GetParam().expected), body(datatype, &data));
}

TEST_P(DatatypeValueTest, WriteThenReadGivesTheSameValue) {
	ASSERT_TRUE(info() != NULL);
	const MleDwpDatatype *datatype = info()->get();
	ASSERT_TRUE(datatype != NULL);

	std::string text(GetParam().input);
	MleDwpDataUnion first;
	ASSERT_EQ(0, readValue(datatype, text, &first));
	std::string written = body(datatype, &first);

	MleDwpDataUnion second;
	ASSERT_EQ(0, readValue(datatype, written, &second));
	EXPECT_EQ(written, body(datatype, &second));
}

TEST_P(DatatypeValueTest, ReadingTwiceKeepsTheLastValue) {
	// Reading into a data union that already holds a value replaces it.
	ASSERT_TRUE(info() != NULL);
	const MleDwpDatatype *datatype = info()->get();
	ASSERT_TRUE(datatype != NULL);

	// Use the first value in the table for this type as the old value.
	const ValueCase *old = NULL;
	for (const ValueCase &v : g_values)
		if (std::string(v.type) == GetParam().type) {
			old = &v;
			break;
		}
	ASSERT_TRUE(old != NULL);

	MleDwpDataUnion data;
	std::string first(old->input), second(GetParam().input);
	ASSERT_EQ(0, readValue(datatype, first, &data));
	ASSERT_EQ(0, readValue(datatype, second, &data));
	EXPECT_EQ(std::string(GetParam().expected), body(datatype, &data));
}

TEST_P(DatatypeValueTest, CopyWithGetAndSet) {
	ASSERT_TRUE(info() != NULL);
	const MleDwpDatatype *datatype = info()->get();
	ASSERT_TRUE(datatype != NULL);

	std::string text(GetParam().input);
	MleDwpDataUnion source;
	ASSERT_EQ(0, readValue(datatype, text, &source));

	MleDwpDataUnion copy;
	switch (info()->copy) {
	  case CopyRaw: {
		// get() fills getSize() bytes, set() takes them.
		alignas(8) char buffer[64];
		memset(buffer, 0, sizeof(buffer));
		datatype->get(&source, buffer);
		datatype->set(&copy, buffer);
		break;
	  }
	  case CopyString: {
		// get() allocates a copy of the string. set() makes another one.
		char *string = NULL;
		datatype->get(&source, &string);
		ASSERT_TRUE(string != NULL || strlen(GetParam().input) == 2);
		datatype->set(&copy, &string);
		mlFree(string);
		break;
	  }
	  case CopyArray:
		// Arrays are copied by the array tests.
		return;
	}
	EXPECT_EQ(datatype, copy.m_datatype);
	EXPECT_EQ(std::string(GetParam().expected), body(datatype, &copy));
}

INSTANTIATE_TEST_SUITE_P(Values, DatatypeValueTest,
	::testing::ValuesIn(g_values), caseName<ValueCase>);

// ---------------------------------------------------------------------------
// Tests of input that is not valid.

class DatatypeInvalidTest : public ::testing::TestWithParam<ValueCase> {};

TEST_P(DatatypeInvalidTest, ReadFails) {
	const TypeInfo *info = findTypeInfo(GetParam().type);
	ASSERT_TRUE(info != NULL);
	const MleDwpDatatype *datatype = info->get();
	ASSERT_TRUE(datatype != NULL);

	std::string text(GetParam().input);
	MleDwpDataUnion data;
	EXPECT_EQ(1, readValue(datatype, text, &data));
}

INSTANTIATE_TEST_SUITE_P(Invalid, DatatypeInvalidTest,
	::testing::ValuesIn(g_invalid), caseName<ValueCase>);

// ---------------------------------------------------------------------------
// Arrays.

// Read an array of ints and copy it out with get().
static bool getIntArray(const std::string &text, MleArray<int> *result) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	MleDwpDataUnion data;
	if (readValue(datatype, text, &data) != 0)
		return false;
	datatype->get(&data, result);
	return true;
}

TEST(DatatypeArrayTest, IntArrayElements) {
	MleArray<int> array;
	ASSERT_TRUE(getIntArray("[ 10 , -20 , 30 ]", &array));
	ASSERT_EQ(3, array.size());
	EXPECT_EQ(10, array[0]);
	EXPECT_EQ(-20, array[1]);
	EXPECT_EQ(30, array[2]);
}

TEST(DatatypeArrayTest, IntArraySetAndWrite) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	ASSERT_TRUE(datatype != NULL);

	// MleArray(num, ptr) takes ownership of ptr, so fill an owned array.
	MleArray<int> source(4);
	for (int i = 0; i < 4; i++)
		source[i] = 7 + i;
	MleDwpDataUnion data;
	datatype->set(&data, &source);
	EXPECT_EQ(std::string("[ 7 , 8 , 9 , 10 ]"), body(datatype, &data));

	// The data union has its own copy of the array.
	source[0] = 99;
	EXPECT_EQ(std::string("[ 7 , 8 , 9 , 10 ]"), body(datatype, &data));
}

TEST(DatatypeArrayTest, FloatArrayElements) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("FloatArray");
	MleDwpDataUnion data;
	std::string text("[ 0.5 , -1.25 ]");
	ASSERT_EQ(0, readValue(datatype, text, &data));

	MleArray<float> array;
	datatype->get(&data, &array);
	ASSERT_EQ(2, array.size());
	EXPECT_FLOAT_EQ(0.5f, array[0]);
	EXPECT_FLOAT_EQ(-1.25f, array[1]);
}

TEST(DatatypeArrayTest, Vector3ArrayElements) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("Vector3Array");
	MleDwpDataUnion data;
	std::string text("[ 1 2 3 , 4 5 6 ]");
	ASSERT_EQ(0, readValue(datatype, text, &data));

	MleArray<MlVector3> array;
	datatype->get(&data, &array);
	ASSERT_EQ(2, array.size());
	EXPECT_FLOAT_EQ(1.0f, array[0][0]);
	EXPECT_FLOAT_EQ(6.0f, array[1][2]);
}

TEST(DatatypeArrayTest, LargeArray) {
	// A large array reads back with every element in place.
	const int count = 2000;
	std::string text("[ ");
	for (int i = 0; i < count; i++) {
		if (i)
			text += " , ";
		text += std::to_string(i - 1000);
	}
	text += " ]";

	MleArray<int> array;
	ASSERT_TRUE(getIntArray(text, &array));
	ASSERT_EQ(count, array.size());
	EXPECT_EQ(-1000, array[0]);
	EXPECT_EQ(0, array[1000]);
	EXPECT_EQ(999, array[count - 1]);

	// And it writes back as the same text.
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	MleDwpDataUnion data;
	datatype->set(&data, &array);
	EXPECT_EQ(text, body(datatype, &data));
}

TEST(DatatypeArrayTest, EmptyArray) {
	MleArray<int> array;
	ASSERT_TRUE(getIntArray("[ ]", &array));
	EXPECT_EQ(0, array.size());
}

TEST(DatatypeArrayTest, EmptyArraySetAndWrite) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	MleArray<int> source;
	MleDwpDataUnion data;
	datatype->set(&data, &source);
	EXPECT_EQ(std::string("[ ]"), body(datatype, &data));
}

TEST(DatatypeArrayTest, GetReturnsAnIndependentCopy) {
	// Changing the copy from get() does not change the data union.
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	MleDwpDataUnion data;
	std::string text("[ 1 , 2 , 3 ]");
	ASSERT_EQ(0, readValue(datatype, text, &data));

	{
		MleArray<int> copy;
		datatype->get(&data, &copy);
		ASSERT_EQ(3, copy.size());
		copy[0] = 99;
	}
	EXPECT_EQ(std::string("[ 1 , 2 , 3 ]"), body(datatype, &data));
}

TEST(DatatypeArrayTest, CommaDirectlyAfterNumberInBuffer) {
	// The input used to lose a comma that followed a number without a blank
	// when it was read from a memory buffer. Files were not affected.
	MleArray<float> array;
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("FloatArray");
	MleDwpDataUnion data;
	std::string text("[ 1.5, 2.5,3.5 ]");
	ASSERT_EQ(0, readValue(datatype, text, &data));
	datatype->get(&data, &array);
	ASSERT_EQ(3, array.size());
	EXPECT_FLOAT_EQ(1.5f, array[0]);
	EXPECT_FLOAT_EQ(2.5f, array[1]);
	EXPECT_FLOAT_EQ(3.5f, array[2]);
}

// ---------------------------------------------------------------------------
// Memory: an array in a data union must be released when it is replaced, when
// the data union goes away and when a read fails.

#if defined(__GLIBC__) && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 33))
#define HAVE_HEAP_SIZE 1
static size_t heapInUse() { return mallinfo2().uordblks; }
#endif

namespace datatypetest {
// The text of an array of ints with count elements.
std::string intArrayText(int count, const char *last = NULL) {
	std::string text("[ ");
	for (int i = 0; i < count; i++) {
		if (i)
			text += " , ";
		text += std::to_string(i);
	}
	if (last)
		text += std::string(" , ") + last;
	return text + " ]";
}
}

class DatatypeMemoryTest : public ::testing::Test {
  protected:
	enum { iterations = 200, elements = 1000, allowed = 64 * 1024 };

	void SetUp() override {
#ifndef HAVE_HEAP_SIZE
		GTEST_SKIP() << "The heap size is only available with glibc.";
#endif
	}

	// Run the operation once to warm up the allocator, then many times, and
	// check that the heap did not grow by anything like an array per run.
	template <class F> void expectNoGrowth(F operation) {
#ifdef HAVE_HEAP_SIZE
		operation();
		size_t before = heapInUse();
		for (int i = 0; i < iterations; i++)
			operation();
		size_t after = heapInUse();
		EXPECT_LT(after, before + allowed)
			<< "heap grew by " << (after - before) << " bytes in " << iterations
			<< " runs; one array is " << elements * sizeof(int) << " bytes";
#endif
	}
};

TEST_F(DatatypeMemoryTest, DataUnionFreesItsArray) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	std::string text = intArrayText(elements);
	expectNoGrowth([&] {
		MleDwpDataUnion data;
		ASSERT_EQ(0, readValue(datatype, text, &data));
	});
}

TEST_F(DatatypeMemoryTest, ReadingOverAnArrayFreesTheOldOne) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	std::string text = intArrayText(elements);
	MleDwpDataUnion data;
	expectNoGrowth([&] {
		ASSERT_EQ(0, readValue(datatype, text, &data));
	});
}

TEST_F(DatatypeMemoryTest, SettingOverAnArrayFreesTheOldOne) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	MleArray<int> source(elements);
	for (int i = 0; i < elements; i++)
		source[i] = i;
	MleDwpDataUnion data;
	expectNoGrowth([&] {
		datatype->set(&data, &source);
	});
}

TEST_F(DatatypeMemoryTest, FailedReadFreesTheElements) {
	// The error is in the last element, after the array has been built up.
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	std::string text = intArrayText(elements, "x");
	expectNoGrowth([&] {
		MleDwpDataUnion data;
		ASSERT_EQ(1, readValue(datatype, text, &data));
	});
}

TEST_F(DatatypeMemoryTest, MissingCommaFreesTheElements) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("IntArray");
	std::string text = intArrayText(elements) ;
	text.insert(text.rfind(" ,") , " 7");   // two numbers with no comma
	expectNoGrowth([&] {
		MleDwpDataUnion data;
		ASSERT_EQ(1, readValue(datatype, text, &data));
	});
}

TEST(DatatypeInputTest, ValueAtEndOfBufferIsReadOnce) {
	// A token that ends with the buffer must not be read a second time.
	MleDwpInput in(0);
	std::string text("12");
	ASSERT_EQ(0, in.setBuffer(text.c_str()));
	int value = 0;
	ASSERT_EQ(0, in.readInt(&value));
	EXPECT_EQ(12, value);
	EXPECT_EQ(1, in.readInt(&value));
}

// ---------------------------------------------------------------------------
// Strings.

TEST(DatatypeStringTest, GetReturnsACopy) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("string");
	MleDwpDataUnion data;
	std::string text("\"original\"");
	ASSERT_EQ(0, readValue(datatype, text, &data));

	char *first = NULL, *second = NULL;
	datatype->get(&data, &first);
	datatype->get(&data, &second);
	ASSERT_TRUE(first != NULL && second != NULL);
	EXPECT_STREQ("original", first);
	EXPECT_NE(first, second);

	first[0] = 'X';
	EXPECT_STREQ("original", second);
	mlFree(first);
	mlFree(second);
}

TEST(DatatypeStringTest, SetCopiesTheSource) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("string");
	char buffer[] = "changing";
	char *source = buffer;
	MleDwpDataUnion data;
	datatype->set(&data, &source);
	buffer[0] = 'X';
	EXPECT_EQ(std::string("changing"), body(datatype, &data));
}

TEST(DatatypeStringTest, NullStringIsEmpty) {
	const MleDwpDatatype *datatype = MleDwpDatatype::findType("string");
	char *source = NULL;
	MleDwpDataUnion data;
	datatype->set(&data, &source);
	EXPECT_EQ(std::string("\"\""), body(datatype, &data));

	char *result = (char *)1;
	datatype->get(&data, &result);
	EXPECT_TRUE(result == NULL);
}

// ---------------------------------------------------------------------------
// Properties and datatypes in a workprint.

namespace datatypetest {
struct PropertyCase {
	const char *label;
	const char *property;   // A Property in TestDWPProperties.dwp.
	const char *type;       // Its datatype.
};

const PropertyCase g_properties[] = {
	{ "String",     "stringTest",     "string" },
	{ "Int",        "intTest",        "int" },
	{ "Float",      "floatTest",      "float" },
	{ "Vector2",    "vector2Test",    "MlVector2" },
	{ "Vector3",    "vector3Test",    "MlVector3" },
	{ "Vector4",    "vector4Test",    "MlVector4" },
	{ "FloatArray", "floatArrayTest", "FloatArray" },
};
}

class PropertyDatatypeTest : public TestWorkprintFixture,
	public ::testing::WithParamInterface<PropertyCase> {};

TEST_P(PropertyDatatypeTest, HasOnlyItsOwnDatatype) {
	MleDwpProperty *property =
		dwptest::findProperty(g_workprint, GetParam().property);
	ASSERT_TRUE(property != NULL);

	const MleDwpDatatype *datatype = property->getDatatype();
	ASSERT_TRUE(datatype != NULL);
	EXPECT_STREQ(GetParam().type, datatype->getName());

	const TypeInfo *own = findTypeInfo(GetParam().type);
	ASSERT_TRUE(own != NULL);
	for (const TypeInfo &other : g_types) {
		EXPECT_EQ(std::string(other.name) == own->name,
			datatype->isa(other.typeId) != 0)
			<< GetParam().property << " as " << other.name;
	}
}

INSTANTIATE_TEST_SUITE_P(Properties, PropertyDatatypeTest,
	::testing::ValuesIn(g_properties), caseName<PropertyCase>);

// ---------------------------------------------------------------------------
// EnumDef: the list of names for an enum.

namespace datatypetest {
// readContents() and writeContents() are protected.
class TestEnumDef : public MleDwpEnumDef {
  public:
	int read(const std::string &text) {
		MleDwpInput in(0);
		if (in.setBuffer(text.c_str()))
			return -1;
		return readContents(&in);
	}
	std::string write() {
		MleDwpOutput out;
		writeContents(&out);
		std::string text(out.getBuffer());
		text.erase(0, text.find('\n') + 1);
		while (! text.empty() && isspace((unsigned char)text.back()))
			text.pop_back();
		return text;
	}
};
}

TEST(EnumDefTest, ReadNames) {
	TestEnumDef def;
	ASSERT_EQ(0, def.read("\"red\" \"green\""));
	ASSERT_EQ(2, def.getNumEnums());
	EXPECT_STREQ("red", def.getString(0));
	EXPECT_STREQ("green", def.getString(1));
}

TEST(EnumDefTest, WriteNames) {
	TestEnumDef def;
	ASSERT_EQ(0, def.read("\"red\" \"green\""));
	EXPECT_EQ(std::string("red green"), def.write());
}

TEST(EnumDefTest, LongList) {
	// The array of names grows with the list.
	std::string text, expected;
	for (int i = 0; i < 100; i++) {
		std::string name = "name" + std::to_string(i);
		text += "\"" + name + "\" ";
		expected += (i ? " " : "") + name;
	}

	TestEnumDef def;
	ASSERT_EQ(0, def.read(text));
	ASSERT_EQ(100, def.getNumEnums());
	EXPECT_STREQ("name0", def.getString(0));
	EXPECT_STREQ("name99", def.getString(99));
	EXPECT_EQ(expected, def.write());
}

TEST(EnumDefTest, NoNames) {
	TestEnumDef def;
	ASSERT_EQ(0, def.read(""));
	EXPECT_EQ(0, def.getNumEnums());
}
