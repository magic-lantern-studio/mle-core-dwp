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

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpItem.h>
#include <mle/DwpFinder.h>
#include <mle/DwpProperty.h>
#include <mle/DwpSetDef.h>
#include <mle/DwpDatatype.h>
#include <mle/DwpInt.h>
#include <mle/DwpLoad.h>

// Include Digital Workprint test header files.
#include "libdwptest.h"

// A small workprint that does not depend on any file.
static const char *g_text = R"DWP(#DWP 1.0 ascii
( ActorDef TestActor
  ( HeaderFile actors/TestActor.h )
  ( PropertyDef intTest int )
)
( Scene testscene MleScene
  ( HeaderFile mle/MleScene.h )
  ( Group testgroup MleGroup
    ( HeaderFile mle/MleGroup.h )
    ( Actor test TestActor
      ( Property intTest int "7" )
    )
  )
)
)DWP";

// Get the value of an int Property, or -1 if it is not an int.
static int getInt(MleDwpProperty *property)
{
	const MleDwpDatatype *datatype = property->getDatatype();
	if (! datatype->isa(MleDwpInt::typeId))
		return -1;

	int value = -1;
	((const MleDwpInt *)datatype)->get(&property->m_data, &value);
	return value;
}

class WorkprintTestFixture: public TestWorkprintFixture {};

TEST(WorkprintLoadTest, LoadWorkprint) {
	// mlLoadWorkprint() loads the workprint and remembers it as the current one.
	MleDwpItem *workprint = mlLoadWorkprint(
		dwptest::workprintPath("TestDWPProperties.dwp").c_str());
	ASSERT_TRUE(workprint != NULL);
	EXPECT_EQ(workprint, mlGetWorkprint());

	// Clean up the workprint and the global reference to it.
	mlSetWorkprint(NULL);
	delete workprint;
}

TEST(WorkprintLoadTest, MissingFile) {
	// A file that does not exist cannot be loaded.
	dwptest::WorkprintPtr workprint = dwptest::loadFile(
		dwptest::workprintPath("DoesNotExist.dwp"));
	EXPECT_TRUE(workprint == NULL);
}

TEST_F(WorkprintTestFixture, IncludeIsResolved) {
	// The workprint includes Mle2dSet.wpf, which defines the 2D set. This
	// shows the include was found relative to the including workprint.
	MleDwpFinder finder;
	finder.setType(MleDwpSetDef::typeId);
	finder.setName("Mle2dSet");
	EXPECT_TRUE(finder.find(g_workprint) != NULL);
}

TEST_F(WorkprintTestFixture, FindMissingProperty) {
	EXPECT_TRUE(dwptest::findProperty(g_workprint, "noSuchProperty") == NULL);
}

TEST(WorkprintStringTest, LoadString) {
	dwptest::WorkprintPtr workprint = dwptest::loadString(g_text);
	ASSERT_TRUE(workprint != NULL);

	MleDwpProperty *property = dwptest::findProperty(workprint.get(), "intTest");
	ASSERT_TRUE(property != NULL);
	EXPECT_EQ(7, getInt(property));
}

TEST(WorkprintStringTest, WriteString) {
	// Write the workprint and read the result back.
	dwptest::WorkprintPtr workprint = dwptest::loadString(g_text);
	ASSERT_TRUE(workprint != NULL);
	std::string text = dwptest::writeString(workprint.get());
	EXPECT_FALSE(text.empty());

	dwptest::WorkprintPtr copy = dwptest::loadString(text);
	ASSERT_TRUE(copy != NULL);
	MleDwpProperty *property = dwptest::findProperty(copy.get(), "intTest");
	ASSERT_TRUE(property != NULL);
	EXPECT_EQ(7, getInt(property));
}
