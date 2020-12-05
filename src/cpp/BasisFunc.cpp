#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <sstream>

#include "include/BasisFunc.h"


bool IsPointInTriangle(Vector P, Vector A, Vector B, Vector C);

BasisFunc::BasisFunc(){

}

BasisFunc::~BasisFunc(){

}

void BasisFunc::SetNodes(int n, Matrix Nodes){
    assert(Nodes.mSize_x == 2);
    assert(Nodes.mSize_y == n);

    NodePoints.SetRectangle(2,n);
    NodePoints = Nodes;

}

void BasisFunc::SetupConnectivity(int n, Matrix Connectivity_x, Matrix Connectivity_y){
    Local_Connectivity.SetRectangle(3,n);

    //We want to find the first triangle that is written 0-1-2, 0-2-3, .. 0-(n-1)-n, 0-n-1;

    //For each node find out whether its pairing is part of a triangle or not. If so, add to thingy
    int iterate = 0;

    for(int i = 1; i < NodePoints.mSize_y; i++){
        if(i < NodePoints.mSize_y -1){
            //loop over elements to find if node i is part of the triangle
            for(int j = 0; j < n; j++){
            //Check all indices on that element
                for(int k = 0; k < 3; k++){
                    if(Connectivity_x.Read(k,j) == NodePoints.Read(0,i) && Connectivity_y.Read(k,j) == NodePoints.Read(1,i)){
                        for(int l = 0; l < 3; l++){
                            if(Connectivity_x.Read(l,j) == NodePoints.Read(0,i+1) && Connectivity_y.Read(l,j) == NodePoints.Read(1,i+1)){
                                Local_Connectivity.Write(0,iterate,i);
                                Local_Connectivity.Write(1,iterate,i+1);
                                iterate++;
                            }
                        }
                    }
                }
            }
        }
        else{
            //loop over elements to find if node i is part of the triangle
            for(int j = 0; j < n; j++){
            //Check all indices on that element
                for(int k = 0; k < 3; k++){
                    if(Connectivity_x.Read(k,j) == NodePoints.Read(0,i) && Connectivity_y.Read(k,j) == NodePoints.Read(1,i)){
                        for(int l = 0; l < 3; l++){
                            if(Connectivity_x.Read(l,j) == NodePoints.Read(0,1) && Connectivity_y.Read(l,j) == NodePoints.Read(1,1)){
                                Local_Connectivity.Write(0,iterate,i);
                                Local_Connectivity.Write(1,iterate,1);
                                iterate++;
                            }
                        }
                    }
                }
            }
        }
    }

}

void BasisFunc::dispNodes(){
    cout << NodePoints << endl;
}

void BasisFunc::dispConnectivity(){
    cout << Local_Connectivity << endl;
}

double BasisFunc::getZ(Vector Point){
    //First find what triangle the point is in. By index convention, each triangle in the basis function
    //is defined by an index, the following index (cyclic so eg 6-1 is allowed) and the center node.

    int K = WhichTriangle(Point);
    if(K == -1){cout << "Something has gone wrong with points in triangles. Point = " << Point << endl; return 0;}

    double area = triangleArea(K);


    Vector A(2),B(2);

    A = getNode(Local_Connectivity.Read(0,K));
    B = getNode(Local_Connectivity.Read(1,K));

    double a = (B.Read(0)*A.Read(1) - A.Read(0)*B.Read(1))/(2.0*area);
    double b = (B.Read(1) - A.Read(1))/(2.0*area);
    double c = (A.Read(0) - B.Read(0))/(2.0*area);

    //As from Larson-Benzen (chapter 4, pages 84-85). the value of the basis function (from the center) inside a given triangle is

    return a + b*Point[0] + c*Point[1];

}

Vector BasisFunc::getGrad(int index){
    //identical implementation to getZ but with different return value

    double area = triangleArea(index);
    Vector A(2),B(2);

    A = getNode(Local_Connectivity.Read(0,index));
    B = getNode(Local_Connectivity.Read(1,index));

    double b = (B.Read(1) - A.Read(1))/(2.0*area);
    double c = (A.Read(0) - B.Read(0))/(2.0*area);

    Vector answer(2);

    answer[0] = b;
    answer[1] = c;

    return answer;
}

int BasisFunc::WhichTriangle(Vector P){

    Vector A(2), B(2), C(2);
    C = getNode(0);

    //Iterating over all triangles to check if the point is in the triangle
    for(int i = 0; i < Local_Connectivity.mSize_y; i++){

        A = getNode(Local_Connectivity.Read(0,i));
        B = getNode(Local_Connectivity.Read(1,i));

        //cout << A << B << C << endl;
        if(IsPointInTriangle(P,A,B,C)){
            return i;
        }
    }

    return -1;

}

bool IsPointInTriangle(Vector P, Vector A, Vector B, Vector C){


    //neat implementation taken from https://blackpawn.com/texts/pointinpoly/

    Vector v0(2), v1(2), v2(2);

    v0 = C - A;
    v1 = B - A;
    v2 = P - A;

    double dot00,dot01,dot02,dot11,dot12;

    dot00 = v0.ScalarProduct(v0);
    dot01 = v0.ScalarProduct(v1);
    dot02 = v0.ScalarProduct(v2);
    dot11 = v1.ScalarProduct(v1);
    dot12 = v1.ScalarProduct(v2);

    double invDenom = 1/ (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    double precision = 10000;

//    if(P[0] == 0.26 && P[1] == 0){
//        cout << fixed << setprecision(30) << u << " " << v << " " << trunc((u + v)*precision) << endl;
//    }
    return (trunc(u*precision)>=0) && (trunc(v*precision)>=0) && (trunc((u + v)*precision) <= precision);
}

double BasisFunc::triangleArea(int index){
    Vector A(2),B(2),C(2);

    C = getNode(0);
    A = getNode(index + 1);

    A = getNode(Local_Connectivity.Read(0,index));
    B = getNode(Local_Connectivity.Read(1,index));

    double quot = (B-A).CalculateNorm(2)*(C-A).CalculateNorm(2);
    double theta = acos((B-A).ScalarProduct(C-A)/quot);

    return 0.5*quot*sin(theta);

}

double BasisFunc::squareIntegral(Vector Beta, BasisFunc OtherV){

    //if 3 points or less, no closure so 1 less integral to compute
    Vector A(2), B(2), C(2), A1(2), B1(2), C1(2);


    double totalsum = 0;
    bool compare_first;
    bool compare_second;

    //Loop over both sets of elements, add to sum if the triangle is shared

    C = getNode(0);
    C1 = OtherV.getNode(0);

    //The two basis functions do not overlap anywhere if the center of one is not part of the other - this is the first check
    bool nonzero = false;
    for(int i = 0; i < OtherV.getNumNodes(); i++){
        if(OtherV.getNode(i) == C){
            nonzero = true;
        }
    }
    if(!nonzero){
        return 0;
    }

    for(int i = 0; i < getNumElements(); i++){
        A = getNode(Local_Connectivity.Read(0,i));
        B = getNode(Local_Connectivity.Read(1,i));

        for(int j = 0; j < OtherV.getNumElements(); j++){
            A1 = OtherV.getNode(OtherV.getElementIndex(0,j));
            B1 = OtherV.getNode(OtherV.getElementIndex(1,j));

            if(A == A1 || A == B1 || A == C1){
                if(B == A1 || B == B1 || B == C1){
                    if(C == A1 || C == B1 || C == C1){
                        totalsum+= triangleArea(i)*Beta.ScalarProduct(getGrad(i))*Beta.ScalarProduct(OtherV.getGrad(j));
                    }
                }
            }

        }

    }

    return totalsum;
}

double BasisFunc::rectangleIntegral(Vector Beta, Matrix Uh_Element){
    //As a result of the test space mesh being a refinement of the trial space mesh, basis function elements are either wholly inside a test space element or wholly outside of it
    //We need some way of determining which of these triangles are in the trial space element in question

    Vector CenterPoint(2);
    double totalsum = 0;
    Vector A(2),B(2),C(2);
    A[0] = Uh_Element.Read(0,0); A[1] = Uh_Element.Read(1,0);
    B[0] = Uh_Element.Read(0,1); B[1] = Uh_Element.Read(1,1);
    C[0] = Uh_Element.Read(0,2); C[1] = Uh_Element.Read(1,2);


    //Loop over all elements to see if they are contained within the Uh Element. If they are, the integration is performed
    for(int i = 0; i < getNumElements(); i++){
        //Generate the center point of the test function triangle, then call isPointInTriangle for that point and the Uh_Element
        CenterPoint[0] = (NodePoints.Read(0,Local_Connectivity.Read(0,i)) + NodePoints.Read(0,Local_Connectivity.Read(1,i)) + NodePoints.Read(0,Local_Connectivity.Read(2,i)))/3.0;
        CenterPoint[1] = (NodePoints.Read(1,Local_Connectivity.Read(0,i)) + NodePoints.Read(1,Local_Connectivity.Read(1,i)) + NodePoints.Read(1,Local_Connectivity.Read(2,i)))/3.0;

        if(IsPointInTriangle(CenterPoint,A,B,C)){
            totalsum += -triangleArea(i)*Beta.ScalarProduct(getGrad(i));
        }

    }

    return totalsum;
}

Vector BasisFunc::getNode(int i){
    Vector answer(2);
    answer[0] = NodePoints.Read(0,i);
    answer[1] = NodePoints.Read(1,i);
    return answer;
}

int BasisFunc::getNumNodes(){
    return NodePoints.mSize_y;
}

int BasisFunc::getNumElements(){
    return Local_Connectivity.mSize_y;
}

int BasisFunc::getElementIndex(int i, int j){
    return Local_Connectivity.Read(i,j);
}
