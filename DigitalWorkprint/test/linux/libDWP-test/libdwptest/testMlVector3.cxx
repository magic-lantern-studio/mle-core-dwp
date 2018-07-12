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

// Include Magic Lantern header files.
#include "math/vector.h"
#include <mle/MleLoad.h>

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpItem.h>
#include <mle/DwpFinder.h>
#include <mle/DwpProperty.h>
#include <mle/DwpDatatype.h>
#include <mle/DwpVector3.h>
#include <mle/DwpLoad.h>

class MlVector3TestFixture: public ::testing::Test
{
  public:
	MlVector3TestFixture( ) {
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

    ~MlVector3TestFixture( )  {
        // Cleanup any pending stuff, but no exceptions allowed.

    	// TBD: delete g_workprint.
    }

    // Put in any custom data members that you need.
    static MleDwpItem *g_workprint;
};

MleDwpItem *MlVector3TestFixture::g_workprint = NULL;

TEST_F(MlVector3TestFixture, LoadWorkprint) {
    // This test is named "LoadWorkprint", and belongs to the "MlVector3TestFixture"
    // test case.

	// Load the test Digital Workprint.
	EXPECT_TRUE(g_workprint != NULL);
}

TEST_F(MlVector3TestFixture, ProcessMlVector3Property) {
    // This test is named "ProcessMlVector3Property", and belongs to the "MlVector3TestFixture"
    // test case.

	// Set up search constraints.
	MleDwpFinder findVectors;
	findVectors.setType(MleDwpProperty::typeId);
	findVectors.setName("vector3Test");

	// Find the vector2Test Property for the TestActor in the DWP.
	MleDwpItem *item = findVectors.find(g_workprint);
	EXPECT_TRUE(item != NULL);
	EXPECT_TRUE(item->isa(MleDwpProperty::typeId));

	// Make sure it is of data type MleDwpVector3.
	MleDwpProperty *property = (MleDwpProperty *)item;
	const MleDwpDatatype *datatype = property->getDatatype();
	EXPECT_TRUE(datatype->isa(MleDwpVector3::typeId));

	// Retrieve and test the value of the Property.
	MlVector3 vector;
	const MleDwpVector3 *vectortype = (MleDwpVector3 *)datatype;
	vectortype->get(&property->m_data, &vector);
	EXPECT_FLOAT_EQ(50.0, vector[0]);
	EXPECT_FLOAT_EQ(50.0, vector[1]);
	EXPECT_FLOAT_EQ(50.0, vector[2]);
}

