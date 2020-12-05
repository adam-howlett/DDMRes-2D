#include "assert.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cassert>

#include "include/Matrix.h"


using namespace std;

double determinant_func(const Matrix &A);

Matrix::Matrix(){
}

//Only to be used if Matrix has been default constructed
void Matrix::SetSquare(int n){
    assert(n > 0);
    mSize_x = n; mSize_y = n;
    Construct_Matrix();
}

void Matrix::SetRectangle(int x, int y){
    assert(x > 0); assert(y > 0);
    mSize_x = x; mSize_y = y;
    Construct_Matrix();
}

Matrix::Matrix(int x, int y)
{
    assert(x > 0); assert(y > 0);
    mSize_x = x; mSize_y = y;
    Construct_Matrix();

}

void Matrix::Clear(){
    for(int i = 0; i < mSize_x; i++){
        delete[] mData[i];
    }
}

Matrix::Matrix(int x)
{
   assert(x > 0);
   mSize_x = x; mSize_y = x;
   Construct_Matrix();
}
Matrix::~Matrix()
{

}

void Matrix::Construct_Matrix(){
   mData = new double *[mSize_x];

   for (int i=0; i<mSize_x; i++)
   {
      mData[i] =  new double[mSize_y];
   }
   for(int i = 0; i < mSize_x; i++){
        for(int j = 0; j < mSize_y; j++){
            mData[i][j] = 0;
        }
   }
}

void Matrix::Write(int i, int j, double value){
    //i is row number, j is column number
    //Down then across
    assert (i < mSize_x ); assert(j < mSize_y);
    mData[i][j] = value;
}

double &Matrix::Read(int i, int j) const{
    return mData[i][j];
}

std::ostream& operator<<(std::ostream& output,
                        const Matrix& M)
{
   for(int j = 0; j < M.mSize_y; j++){
    for(int i = 0; i < M.mSize_x; i++){
        output << std::setw(7) << M.Read(i,j) << "  ";
    }
    output << "\n \n";
   }

   return output;
}

void Matrix::Insert(int x, int y, const Matrix& M){
    for(int i = 0; i < M.mSize_x; i++){
        for(int j = 0; j< M.mSize_y; j++){
            assert(x+i < mSize_x); assert(x+i >= 0);
            assert(y+j < mSize_y); assert(y+j >= 0);
            mData[x+i][y+j] = M.Read(i,j);
        }
    }
}

void Matrix::Gauss_Eliminate(Vector *v){
    assert(v->GetSize() == mSize_y);

    double multiple;
    for(int i = 0; i < mSize_y-1; i++){
        for(int j = i+1; j<mSize_y; j++){
            //cout << i << " " << j << endl;
            //cout << mData[i][i] << endl;
            multiple = -mData[j][i]/mData[i][i];
            //cout << "Multiple: " << multiple << endl;
            Row_Add(i,j,multiple);

            v->operator[](j) += multiple * v->Read(i);
        }
    }

}

//Back Substitution to solve matrix now works
void Matrix::Back_Substitute(Vector v, Vector *Solution){
    assert(v.GetSize() == mSize_y);
    assert(Solution->GetSize() == mSize_y);

    Solution->operator[](mSize_y - 1) = v.Read(mSize_y - 1)/ mData[mSize_y - 1][mSize_x - 1];
    for(int i = mSize_y - 2; i >=0; i--){
        Solution->operator[](i) = v.Read(i)/ mData[i][i];
        double divisor = Solution->Read(i);
        for(int j = i + 1; j < mSize_y; j++){
            Solution->operator[](i) = Solution->Read(i) - mData[i][j]*Solution->Read(j)/mData[i][i];
        }
    }

}

// Overloading the binary + operator
Matrix Matrix::operator+(const Matrix& M1) const
{
   assert(mSize_x == M1.mSize_x); assert(mSize_y == M1.mSize_y);
   Matrix M(mSize_x, mSize_y);
   for (int i=0; i<mSize_x; i++){
       for(int j = 0; j<mSize_y; j++){
            M.Write(i,j,(mData[i][j] + M1.mData[i][j]));
       }
   }
   return M;
}

Matrix Matrix::operator-(const Matrix& M1) const
{
   assert(mSize_x == M1.mSize_x); assert(mSize_y == M1.mSize_y);
   Matrix M(mSize_x, mSize_y);
   for (int i=0; i<mSize_x; i++){
       for(int j = 0; j<mSize_y; j++){
            M.Write(i,j,(mData[i][j] - M1.mData[i][j]));
       }
   }
   return M;
}

void Matrix::Read_From_File(const char* str){

    ifstream infile;
    infile.open(str);

    int z;
    int column = 0;
    int row = 0;
    while(infile >> z){
        mData[row][column] = z;
        column++;
        if(column == mSize_x){
            row++;
            column = 0;
        }
    }

    infile.close();

}

void Matrix::Row_Add(int row_to_add, int row_to_add_to, double multiple){

    //cout << "Adding row " << row_to_add << " to row " << row_to_add_to << " with multiple " << multiple << endl;
    for(int i = 0;  i < mSize_x; i++){
        mData[row_to_add_to][i] += multiple*mData[row_to_add][i];
    }
}

void Matrix::Divide(double a){
       for(int i = 0; i < mSize_x; i++){
        for(int j = 0; j < mSize_y; j++){
            mData[i][j] = mData[i][j]/a;
        }
   }
}


Matrix& Matrix::operator=(const Matrix& M1)
{
   assert(mSize_x == M1.mSize_x); assert(mSize_y == M1.mSize_y);
   for (int i=0; i<mSize_x; i++){
        for(int j = 0; j < mSize_y; j++){
            mData[i][j] = M1.Read(i,j);
        }
    }
   return *this;
}

void Matrix::Transpose(){
    int i = mSize_x, j = mSize_y;
    Matrix Trans(j,i);

    for(int i = 0; i < mSize_x; i++){
        for(int j = 0; j < mSize_y; j++){
            Trans.Write(j,i,Read(i,j));
        }
    }
    Clear();
    mSize_x = j; mSize_y = i;
    Construct_Matrix();

    for(int i = 0; i < mSize_x; i++){
        for(int j = 0; j < mSize_y; j++){
            Write(i,j,Trans.Read(i,j));
        }
    }
}

void Matrix::RemoveIndex(int index){
    Matrix hold(mSize_x,mSize_y);
    for(int i = 0; i < mSize_x; i++){
        for(int j = 0; j < mSize_y; j++){
            hold.Write(i,j,Read(i,j));
        }
    }

    Clear();
    SetRectangle(mSize_x-1,mSize_y-1);

    int i = 0;int j = 0;
    int k = 0;int l = 0;
    while(j < mSize_y){
        if(l == index){
            l++;
        }
        while(i < mSize_x){
            if(k != index){
                Write(i,j,hold.Read(k,l));
                i++;
                k++;
            }
            else{
                k++;
            }
        }
        i = 0;
        k = 0;
        j++;
        l++;
    }

}

void Matrix::RemoveRow(int index){

    assert(index < mSize_y);

    Matrix hold(mSize_x,mSize_y);
    for(int i = 0; i < mSize_x; i++){
        for(int j = 0; j < mSize_y; j++){
            hold.Write(i,j,Read(i,j));
        }
    }

    Clear();
    SetRectangle(mSize_x,mSize_y-1);

    int i = 0;int j = 0;
    int k = 0;
    while(j < mSize_y){
        if (k == index){
            k++;
        }
        while(i < mSize_x){
            Write(i,j,hold.Read(i,k));
            i++;
        }
        i = 0;
        k++;
        j++;
    }

}

void Matrix::RemoveColumn(int index){

    assert(index < mSize_x);

    Matrix hold(mSize_x,mSize_y);
    for(int i = 0; i < mSize_x; i++){
        for(int j = 0; j < mSize_y; j++){
            hold.Write(i,j,Read(i,j));
        }
    }

    Clear();
    SetRectangle(mSize_x-1,mSize_y);

    int i = 0;int j = 0;
    int k = 0;
    while(j < mSize_y){
        while(i < mSize_x){
        if (k != index){
            Write(i,j,hold.Read(k,j));
            i++;
            k++;
        }
        else{
            k++;
        }

        }
        i = 0;
        k = 0;
        j++;
    }

}
