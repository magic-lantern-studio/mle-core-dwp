// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2016 - 2018 Wizzer Works
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

// Include Magic Lantern header files.
#include "math/vector.h"
#include <mle/MleLoad.h>

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpItem.h>
#include <mle/DwpFinder.h>
#include <mle/DwpProperty.h>
#include <mle/DwpDatatype.h>
#include <mle/DwpFloat.h>

class FloatTestFixture: public ::testing::Test
{
  public:
	FloatTestFixture( ) {
		// Initialize DWP library.
		mleDwpInit();
    }

    void SetUp( ) {
        // Code here will execute just before the test ensues .
    	if (g_workprint == NULL)
    	    g_workprint = mlLoadWorkprint("workprints/TestDWPProperties.dwp");
    }

    void TearDown( ) {
        // Code here will be called just after the test completes.
        // Ok to throw exceptions from here if need be.

    	// TBD: unload the Digital Workprint.
    }

    ~FloatTestFixture( )  {
        // Cleanup any pending stuff, but no exceptions allowed.

    	// TBD: delete g_workprint.
    }

    // Put in any custom data members that you need.
    static MleDwpItem *g_workprint;
};

MleDwpItem *FloatTestFixture::g_workprint = NULL;

TEST_F(FloatTestFixture, LoadWorkprint) {
    // This test is named "LoadWorkprint", and belongs to the "FloatTestFixture"
    // test case.

	// Load the test Digital Workprint.
	EXPECT_TRUE(g_workprint != NULL);
}

TEST_F(FloatTestFixture, ProcessFloatProperty) {
    // This test is named "ProcessFloatProperty", and belongs to the "FloatTestFixture"
    // test case.

	// Set up search constraints.
	MleDwpFinder findFloats;
	findFloats.setType(MleDwpProperty::typeId);
	findFloats.setName("floatTest");

	// Find the floatTest Property for the TestActor in the DWP.
	MleDwpItem *item = findFloats.find(g_workprint);
	EXPECT_TRUE(item != NULL);
	EXPECT_TRUE(item->isa(MleDwpProperty::typeId));

	// Make sure it is of data type MleDwpVector2.
	MleDwpProperty *property = (MleDwpProperty *)item;
	const MleDwpDatatype *datatype = property->getDatatype();
	EXPECT_TRUE(datatype->isa(MleDwpFloat::typeId));

	// Retrieve and test the value of the Property.
	float testFloat;
	const MleDwpFloat *floattype = (MleDwpFloat *)datatype;
	floattype->get(&property->m_data, &testFloat);
	EXPECT_FLOAT_EQ(0.25, testFloat);
}

