// dataVectorTest.cpp

#include "SparseDataVector.h"
#include "DenseDataVector.h"
#include "SparseBinaryDataVector.h"
#include "ZeroDataVector.h"
#include "OnesDataVector.h"
using namespace dataset;

// types
#include "RealArray.h"
using types::DoubleArray;

// linear
#include "DoubleVector.h"
using linear::DoubleVector;

// stl
#include <iostream>
using std::cout;
using std::endl;

#include<string>
using std::string;

bool testFailed = false;

/// checks if two doubles are equal, up to a small numerical error
///
bool isEqual(double a, double b, double tolerance = 1.0e-8)
{
    if(a-b < tolerance && b-a < tolerance)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/// checks if two vectors are equal, up to a small numerical error in each coordinate
///
bool isEqual(const DoubleVector& a, const DoubleVector& b, double tolerance = 1.0e-8)
{
    for(int i = 0; i < a.Size(); ++i)
    {
        if(isEqual(a[i], b[i], tolerance) == false)
        {
            return false;
        }
    }
    return true;
}

/// Generates a DoubleVector, for the purpose of testing
///
DoubleVector getVector()
{
    DoubleVector a(15);
    a[0] = 0.1;
    a[1] = 1.2;
    a[2] = 2.3;
    a[3] = 3.4;
    a[4] = 4.5;
    a[5] = 5.6;
    a[6] = 6.7;
    a[7] = 7.8;
    a[8] = 8.9;
    a[9] = 9.0;
    a[10] = -0.1;
    a[11] = -1.2;
    a[12] = -2.3;
    a[13] = -3.4;
    a[14] = -4.5;

    return a;
}

/// Prints the results of a test and sets the global testFailed flag if appropriate
///
void processTest(const string& testName, const string& typeName, bool success)
{
    cout << "Running " << testName << " test with " << typeName << " ... ";

    if (success)
    {
        cout << "Passed\n";
    }
    else
    {
        cout << "Failed\n";
        testFailed = true;
    }
}

/// Tests the Dot() member of DataVectors
///
template<typename DataVectorType>
void dotTest()
{
    auto a = getVector();

    DoubleArray b(15);
    b[3] = 1.0;
    b[4] = 1.0;
    b[12] = 1.0;
    b[13] = 1.0;

    DataVectorType c(b.GetIterator());

    double result = c.Dot(a);
    const double expectedResult = 2.2;

    processTest("Dot()", typeid(DataVectorType).name(), isEqual(result, expectedResult));
}

/// Tests the Dot() member of ZeroDataVector
///
void dotTestZeroDataVector()
{
    auto a = getVector();
    ZeroDataVector z;
    double result = z.Dot(a);
    processTest("Dot()", "ZeroDataVector", isEqual(result, 0.0));
}

/// Tests the Dot() member of OnesDataVector
///
void dotTestOnesDataVector()
{
    auto a = getVector();
    OnesDataVector o(4);
    double result = o.Dot(a);
    processTest("Dot()", "OnesDataVector", isEqual(result, 7.0));
}

/// Tests the Dot() member of DataVectors
///
void dotTest()
{
    dotTest<DoubleDataVector>();
    dotTest<FloatDataVector>();
    dotTest<SparseDoubleDataVector>();
    dotTest<SparseFloatDataVector>();
    dotTest<SparseShortDataVector>();
    dotTest<SparseBinaryDataVector>();
    dotTest<UncompressedSparseBinaryDataVector>();
    dotTestZeroDataVector();
    dotTestOnesDataVector();
}

/// Adds a DataVector to a DoubleVector in two different ways and checks that the result is the same
///
template<typename DataVectorType1, typename DataVectorType2>
void addToTest()
{
    auto a1 = getVector();
    auto a2 = getVector();

    DoubleArray b(15);
    b[3] = 1.0;
    b[4] = 1.0;
    b[12] = 1.0;
    b[13] = 1.0;

    DataVectorType1 c1(b.GetIterator());
    DataVectorType2 c2(b.GetIterator());

    c1.AddTo(a1);
    c2.AddTo(a2);

    string name1 = typeid(DataVectorType1).name();
    string name2 = typeid(DataVectorType2).name();

    processTest("AddTo()", name1 + " and " + name2, isEqual(a1, a2));
}

/// Tests the AddTo() member of ZeroDataVector
///
void addToTestZeroDataVector()
{
    auto a1 = getVector();
    auto a2 = getVector();

    ZeroDataVector z;

    z.AddTo(a1);

    processTest("AddTo()", "class dataset::ZeroDataVector", isEqual(a1, a2));
}

/// Tests the AddTo() member of OnesDataVector
///
void addToTestOnesDataVector()
{
    auto a1 = getVector();
    auto a2 = getVector();

    OnesDataVector o(a1.Size());

    o.AddTo(a1);

    double norm1 = a1.Norm2();
    double norm2 = a2.Norm2() + 2 * o.Dot(a2) + a2.Size();

    processTest("AddTo()", "class dataset::OnesDataVector", isEqual(norm1, norm2));
}

/// Tests the AddTo() member of DataVectors
///
void addToTest()
{
    addToTest<DoubleDataVector, FloatDataVector>();
    addToTest<DoubleDataVector, SparseDoubleDataVector>();
    addToTest<DoubleDataVector, SparseFloatDataVector>();
    addToTest<DoubleDataVector, SparseShortDataVector>();
    addToTest<DoubleDataVector, SparseBinaryDataVector>();
    addToTest<DoubleDataVector, UncompressedSparseBinaryDataVector>();
    addToTestZeroDataVector();
    addToTestOnesDataVector();
}

void iteratorConstructorTest()
{
    auto a = getVector();

    DoubleDataVector b(a.GetIterator());
    SparseDoubleDataVector c(b.GetIterator());
    FloatDataVector d(c.GetIterator());
    SparseFloatDataVector e(d.GetIterator());
    DoubleVector f(e.GetIterator());

    processTest("Iterator and Ctor", "all float/double precision DataVector types", isEqual(a, f, 1.0e-6));
}

/// Runs all tests
///
int main()
{
    dotTest();
    addToTest();
    iteratorConstructorTest();

    if(testFailed)
    {
        return 1;
    }

    return 0;
}



