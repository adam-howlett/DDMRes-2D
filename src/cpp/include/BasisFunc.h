#ifndef BASISFUNC_H
#define BASISFUNC_H
#include "Vector.hpp"
#include "Matrix.h"
#include "Mesh.h"

class BasisFunc
{
    public:

        //Local connectivity (center is 0) goes like 1-2-0, 2-3-0, 3-4-0,... (n-1)-1-0 where n is number of exterior nodes.
        //if the basis function is not complete (cyclic), at least one of the elements in this sequence are missing

        //Construction Methods
        BasisFunc();
        virtual ~BasisFunc();
        void SetNodes(int n, Matrix Nodes);
        void SetupConnectivity(int n, Matrix Connectivity_x, Matrix Connectivity_y);

        //Access Operators
        Vector getNode(int i);
        int getNumNodes();
        int getNumElements();
        int getElementIndex(int i, int j);

        //Display Methods
        void dispNodes();
        void dispConnectivity();

        // Calculation Methods
        double getZ(Vector Point);
        Vector getGrad(int index);
        double squareIntegral(Vector beta,BasisFunc OtherV);
        double rectangleIntegral(Vector Beta, Matrix Uh_Element);



    protected:

    private:
        int WhichTriangle(Vector P);
        double triangleArea(int index);
        Matrix NodePoints;
        Matrix Local_Connectivity;
};

#endif // BASISFUNC_H
