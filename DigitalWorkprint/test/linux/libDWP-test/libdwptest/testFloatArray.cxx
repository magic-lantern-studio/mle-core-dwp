// COPYRTIGH_BEGIN
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
#include <mle/mlArray.h>

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpItem.h>
#include <mle/DwpFinder.h>
#include <mle/DwpProperty.h>
#include <mle/DwpDatatype.h>
#include <mle/DwpFloatArray.h>

class FloatArrayTestFixture: public ::testing::Test
{
  public:
	FloatArrayTestFixture( ) {
        // Initialization code here.
		m_workprint = NULL;

		// Initialize DWP library.
		mleDwpInit();
    }

    void SetUp( ) {
        // Code here will execute just before the test ensues .
    	if (m_workprint == NULL)
    	    m_workprint = mlLoadWorkprint("workprints/TestDWPProperties.dwp");
    }

    void TearDown( ) {
        // Code here will be called just after the test completes.
        // Ok to throw exceptions from here if need be.

    	// TBD: unload the Digital Workprint.
    }

    ~FloatArrayTestFixture( )  {
        // Cleanup any pending stuff, but no exceptions allowed.

    	// TBD: delete m_workprint.
    }

    // Put in any custom data members that you need.
    MleDwpItem *m_workprint;
};

TEST_F(FloatArrayTestFixture, LoadWorkprint) {
    // This test is named "LoadWorkprint", and belongs to the "FloatArrayTestFixture"
    // test case.

	// Load the test Digital Workprint.
	EXPECT_TRUE(m_workprint != NULL);
}

TEST_F(FloatArrayTestFixture, ProcessFloatArrayProperty) {
    // This test is named "ProcessFloatArrayProperty", and belongs to the "FloatArrayTestFixture"
    // test case.

	// Set up search constraints.
	MleDwpFinder findArrays;
	findArrays.setType(MleDwpProperty::typeId);
	findArrays.setName("floatArrayTest");

	// Find the floatArrayTest Property for the TestActor in the DWP.
	MleDwpItem *item = findArrays.find(m_workprint);
	EXPECT_TRUE(item != NULL);
	EXPECT_TRUE(item->isa(MleDwpProperty::typeId));

	// Make sure it is of data type MleDwpFloatArray.
	MleDwpProperty *property = (MleDwpProperty *)item;
	const MleDwpDatatype *datatype = property->getDatatype();
	EXPECT_TRUE(datatype->isa(MleDwpFloatArray::typeId));

	// Retrieve and test the value of the Property.
	MleArray<float> array;
	const MleDwpFloatArray *arraytype = (MleDwpFloatArray *)datatype;
	arraytype->get(&property->m_data, &array);
	EXPECT_EQ(5, array.size());
	EXPECT_FLOAT_EQ(1.0, array[0]);
	EXPECT_FLOAT_EQ(2.0, array[1]);
	EXPECT_FLOAT_EQ(3.0, array[2]);
	EXPECT_FLOAT_EQ(4.0, array[3]);
	EXPECT_FLOAT_EQ(5.0, array[4]);
}

