#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <string>

#include "Vector.hpp"

using namespace std;

class Matrix
{
    public:
        Matrix();
        Matrix(int x);
        Matrix(int x, int y);
        void Clear();
        void SetSquare(int n);
        void SetRectangle(int x, int y);
        virtual ~Matrix();

        //Read/Write/Access Operators
        void Insert(int x, int y, const Matrix& M);
        void Write(int i, int j, double value);
        double &Read (int i, int j) const;
        void Read_From_File(const char* str);

        //Matrix Manipulation
        void Row_Add(int row_to_add, int row_to_add_to, double multiple);
        void Transpose();
        void Gauss_Eliminate(Vector *v);
        void Back_Substitute(Vector v, Vector *Solution);
        void RemoveIndex(int index);
        void RemoveRow(int index);
        void RemoveColumn(int index);
        void Divide(double a);
        double Determinant();

        Matrix operator+(const Matrix& M1) const;
        Matrix operator-(const Matrix& M1) const;
        Matrix& operator=(const Matrix& M1);


        int mSize_x, mSize_y;

    protected:

    private:
        void Construct_Matrix();
        double** mData;

        friend std::ostream& operator<<(std::ostream& output,
                       const Matrix& M);

};

#endif // MATRIX_H
