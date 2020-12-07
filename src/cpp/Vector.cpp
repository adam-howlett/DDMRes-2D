#include <iomanip>
#include <iostream>
#include <cmath>
#include <cassert>
#include "assert.h"

#include "include/Vector.hpp"

//The majority of this class & its methods were written by one of my lecturers at the University of Nottingham. Certain modifications & additions have been made by me in order to fit it for purpose

//Default Constructor
Vector::Vector(){

}

//Set up Size etc.
void Vector::Initialise(int size){
   assert(size > 0);
   mSize = size;
   mData = new double [mSize];
   for (int i=0; i<mSize; i++)
   {
      mData[i] = 0.0;
   }
}

void Vector::Clear(){
    mSize = 0;
    delete[] mData;
}

// Overridden copy constructor
// Allocates memory for new vector, and copies
// entries of other vector into it
Vector::Vector(const Vector& otherVector)
{
   mSize = otherVector.GetSize();
   mData = new double [mSize];
   for (int i=0; i<mSize; i++)
   {
      mData[i] = otherVector.mData[i];
   }
}

// Constructor for vector of a given size
// Allocates memory, and initialises entries
// to zero
Vector::Vector(int size)
{
   assert(size > 0);
   mSize = size;
   mData = new double [mSize];
   for (int i=0; i<mSize; i++)
   {
      mData[i] = 0.0;
   }
}

// Overridden destructor to correctly free memory
Vector::~Vector()
{
   delete[] mData;
}

// Method to get the size of a vector
int Vector::GetSize() const
{
   return mSize;
}

//Add Operator -> adds the argument at the end of the vector, increasing its length by one
void Vector::Add(double x){
    Vector hold(mSize);
    for(int i = 0; i < mSize; i++){
        hold[i] = Read(i);
    }

    Clear();
    Initialise(hold.GetSize()+1);

    for(int i = 0; i < mSize-1; i++){
        this->operator[](i) = hold.Read(i);
    }

    this->operator[](mSize-1) = x;
}


//Remove Opeator -> removes the value at a given index, decreasing its length by one
void Vector::Remove(int index){
    Vector hold(mSize);
    for(int i = 0; i < mSize; i++){
        hold[i] = Read(i);
    }

    Clear();
    Initialise(hold.GetSize()-1);

    int i=0,j=0;
    while(i < mSize+1){
        if(i != index){
            this->operator[](j) = hold.Read(i);
            j++;
        }
        i++;
    }
}

// Overloading square brackets
// Note that this uses `zero-based' indexing,
// and a check on the validity of the index
double& Vector::operator[](int i)
{
   assert(i > -1);
   assert(i < mSize);
   return mData[i];
}

// Read-only variant of []
// Note that this uses `zero-based' indexing,
// and a check on the validity of the index
double Vector::Read(int i) const
{
   assert(i > -1);
   assert(i < mSize);
   return mData[i];
}

// Overloading round brackets
// Note that this uses `one-based' indexing,
// and a check on the validity of the index
double& Vector::operator()(int i)
{
   assert(i > 0);
   assert(i < mSize+1);
   return mData[i-1];
}

// Overloading the assignment operator
Vector& Vector::operator=(const Vector& otherVector)
{
   assert(mSize == otherVector.mSize);
   for (int i=0; i<mSize; i++)
   {
      mData[i] = otherVector.mData[i];
   }
   return *this;
}

// Overloading the unary + operator
Vector Vector::operator+() const
{
   Vector v(mSize);
   for (int i=0; i<mSize; i++)
   {
      v[i] = mData[i];
   }
   return v;
}

// Overloading the unary - operator
Vector Vector::operator-() const
{
   Vector v(mSize);
   for (int i=0; i<mSize; i++)
   {
      v[i] = -mData[i];
   }
   return v;
}

// Overloading the binary + operator
Vector Vector::operator+(const Vector& v1) const
{
   assert(mSize == v1.mSize);
   Vector v(mSize);
   for (int i=0; i<mSize; i++)
   {
      v[i] = mData[i] + v1.mData[i];
   }
   return v;
}

// Overloading the binary - operator
Vector Vector::operator-(const Vector& v1) const
{
   assert(mSize == v1.mSize);
   Vector v(mSize);
   for (int i=0; i<mSize; i++)
   {
      v[i] = mData[i] - v1.mData[i];
   }
   return v;
}

// Overloading scalar multiplication
Vector Vector::operator*(double a) const
{
   Vector v(mSize);
   for (int i=0; i<mSize; i++)
   {
      v[i] = a*mData[i];
   }
   return v;
}

// Method to calculate norm (with default value p=2)
// corresponding to the Euclidean norm
double Vector::CalculateNorm(int p) const
{

   double norm_val, sum = 0.0;
   for (int i=0; i<mSize; i++)
   {
      sum += pow(fabs(mData[i]), p);
   }
   norm_val = pow(sum, 1.0/((double)(p)));
   return norm_val;
}

// Method to calculate the infinity norm
double Vector::CalculateInfinityNorm() const
{
   double norm_val = 0;
   for (int i=0; i<mSize; i++)
   {
     double abs_val = fabs(mData[i]);
     if ( norm_val < abs_val )
     {
       norm_val = abs_val;
     }
   }
   return norm_val;
}

// Method to calculate scalar product with another vector
double Vector::ScalarProduct(const Vector& v) const
{
   double scalar_product = 0.0;
   assert(mSize == v.GetSize());
   for (int i=0; i<mSize; i++)
   {
      scalar_product += mData[i]*v.Read(i);
   }
   return scalar_product;
}

// MATLAB style friend to get the size of a vector
int length(const Vector& v)
{
   return v.mSize;
}
//Code from Chapter10.tex line 60 save as Vector.cpp

bool Vector::operator==(const Vector& v1) const
{
    if(mSize != v1.GetSize()){
        return false;
    }
    for(int i = 0; i < mSize; i++){
        if(mData[i] != v1.Read(i)){
            return false;
        }
    }
    return true;
}


// Added by Daniele Avitabile - edited by me
std::ostream& operator<<(std::ostream& output,
                        const Vector& v)
{
   output << "[";
   for (int i=0; i<v.mSize; i++)
   {
      output << std::setprecision(10)
	     << v.Read(i);
      if(i != v.mSize - 1){
            output << ", ";}
   }
   output << "]"<< std::endl;

   return output;
}
