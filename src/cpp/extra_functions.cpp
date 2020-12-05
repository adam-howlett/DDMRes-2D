#include <iostream>
#include <cmath>
#include "assert.h"

#include "include/BasisFunc.h"

#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795028841971693993751058
#endif // M_PI

//F term
double source_data(double x, double y, int selection){
    switch(selection){
    case 1:
        return 0;
    case 2:
        return exp(x)*(sin(y));
    case 3:
        return -M_PI*x*sin(M_PI*x*y);
    case 4:
        return 0;
    }
    return 0;
}

//G Term
double boundary_function(double x, double y,int selection){
    switch(selection){
    case 1:
        return sin(M_PI*x);
    case 2:
        return 0;
    case 3:
        return cos(M_PI*x*y);
    case 4:
        return exp(x)*cos(M_PI*y);
    }
    return 0;//exp(x)*cos(M_PI*y);//;exp(1)*(1-cos(y));
}

//Exact Solution
double u_exact(double x, double y, int selection, Vector Beta){
    //return exp(x)*(1-cos(y));//sin(M_PI*x);//sin(M_PI*y);//

    switch(selection){
    case 1:
        return sin(M_PI*x);
    case 2:
        return exp(x)*(1-cos(y));
    case 3:
        return cos(M_PI*x*y);
    case 4:

        //Find out the direction of beta, first check is for cardinal directions
        if(Beta[0] == 0){
            if(Beta[1] > 0){
                //Straight up, cast down to y = 0 only
                return boundary_function(x,0,selection);
            }
            else{
                //Straight Down
                return boundary_function(x,1,selection);
            }

        }
        else if(Beta[1] == 0){
            if(Beta[0] > 0){
                //Straight Right
                return boundary_function(0,y,selection);
            }
            else{
                //Straight Left
                return boundary_function(1,y,selection);
            }
        }
        else{
            double gradient = Beta[1]/Beta[0];
            double angle = atan(gradient);
            double y_intersect = y - gradient*x;
            double x_intersect = -y_intersect/gradient;
            double right_boundary_intersect = y_intersect + gradient;
            double top_boundary_intersect = (1-y_intersect)/gradient;

            if(angle > 0){
                //Up right or Down left
                if(Beta[0] < 0 || Beta[1] < 0){
                    //cout << "Down Left" << endl;

                    if(right_boundary_intersect > 1){
                        //Line Traced back hits the top boundary
                        return boundary_function(top_boundary_intersect,1,selection);
                    }
                    else{
                        //Line traced back hits the right boundary
                        return boundary_function(1,right_boundary_intersect,selection);
                    }
                }
                else{
                    //cout << "Up right" << endl;
                    if(y_intersect <= 0){
                        //Line Traced back hits the bottom boundary
                        return boundary_function(x_intersect,0,selection);
                    }
                    else{
                        //Line Traced back hits the left

                        return boundary_function(0,y_intersect,selection);
                    }
                }

            }

            else{
                //Up left or Down Right
                if(Beta[0] < 0){
                    //cout << "Up left" << endl;
                    if(right_boundary_intersect > 0){
                        //Line traced back hits the right boundary
                        return boundary_function(1,right_boundary_intersect,selection);
                    }
                    else{
                        //Line traced back hits the bottom boundary
                        return boundary_function(x_intersect,0,selection);
                    }
                }
                else{
                    //cout << "Down Right" << endl;

                    if(y_intersect > 1){
                        //Line traced back hits the top boundary
                        return boundary_function(top_boundary_intersect,1,selection);
                    }
                    else{
                        //Line traced back hits the left boundary
                        return boundary_function(0,y_intersect,selection);
                    }


                }
            }
        }
    }

    return 0;


}

double u_squared(double x, double y, int selection, Vector Beta){
    return pow(u_exact(x,y,selection, Beta),2);
}

double squaredError(double x, double y, double uh, int selection, Vector Beta){
    return pow(u_exact(x,y,selection,Beta) - uh,2);
}

double one_integral(double x, double y){
return 1;
}

bool isOnBoundary(Vector center_node){
    if(center_node[0] == 1 || center_node[1] == 1 || center_node[0] < 0.0001 || center_node[1]  < 0.0001){return true;}
    else return false;
}

Vector getUnitNormal(Vector check){
    Vector unit_normal(2);
    check[0] = round(check[0] * 10000) /10000;
    check[1] = round(check[1] * 10000) /10000;
    assert(isOnBoundary(check));
    if(check[0] == 0){
        if(check[1] == 0){
            unit_normal[0] = -0.7071;
            unit_normal[1] = -0.7071;
        }
        else if(check[1] == 1){
            unit_normal[0] = -0.7071;
            unit_normal[1] = 0.7071;
        }
        else{
            unit_normal[0] = -1;
            unit_normal[1] = 0;
        }
    }
    else if(check[0] == 1){
        if(check[1] == 0){
            unit_normal[0] = 0.7071;
            unit_normal[1] = -0.7071;
        }
        else if(check[1] == 1){
            unit_normal[0] = 0.7071;
            unit_normal[1] = 0.7071;
            }
        else{
            unit_normal[0] = 1;
            unit_normal[1] = 0;
        }
    }
    else if(check[1] == 1){
        if(check[0] != 0 && check[0] !=1){
            unit_normal[0] = 0;
            unit_normal[1] = 1;
        }
    }
    else if(check[1] == 0){
        if(check[0] != 0 && check[0] !=1){
            unit_normal[0] = 0;
            unit_normal[1] = -1;
        }
    }

    return unit_normal;

}

double Flow_Normal_Product(BasisFunc V, Vector Beta){
    Vector center = V.getNode(0);
    return Beta.ScalarProduct(getUnitNormal(center));
}

double trapezium_on_boundary(Vector Start, Vector End, double (*g)(double,double,int selection), BasisFunc V, Vector Beta, Vector unit_normal, int selection){

    int n_trap = 10;
    Vector Line = End - Start;
    double h_trap = Line.CalculateNorm(2)/n_trap;

    //need angle between the line and the x axis
    Vector x_axis(2); x_axis[0] = 1; x_axis[1] = 0;

    double theta = acos(Line.ScalarProduct(x_axis) / (Line.CalculateNorm(2) * x_axis.CalculateNorm(2)));

    Vector MidPoint(2);
    double integral = 0;
    for(int i = 0; i < n_trap ; i++){

        MidPoint[0] = Start[0] + i*h_trap*cos(theta) + h_trap*cos(theta)/2;
        MidPoint[1] = Start[1] + i*h_trap*sin(theta) + h_trap*sin(theta)/2;

        integral += abs(Beta.ScalarProduct(getUnitNormal(MidPoint)))*V.getZ(MidPoint)*g(MidPoint[0],MidPoint[1],selection);
    }
    return h_trap*integral;


}

bool isPositiveDefinite(Matrix Test){
    Vector TestVector(Test.mSize_x);

    for(int i = 0; i < TestVector.GetSize();i++){
        TestVector[i] = 1.0;
    }
    Vector *pTest= &TestVector;

    Test.Gauss_Eliminate(pTest);
    for(int i = 0; i < Test.mSize_x; i++){
        if(Test.Read(i,i) < 0){
            return false;
        }
    }
    return true;

}

double triangleArea(Vector A, Vector B, Vector C){
    double quot = (B-A).CalculateNorm(2)*(C-A).CalculateNorm(2);
    double theta = acos((B-A).ScalarProduct(C-A)/quot);

    return 0.5*quot*sin(theta);
}

double triangleIntegral_MidpointRule(Vector A, Vector B, Vector C, double (*u)(double,double,double,int,Vector),double uh,int selection,Vector Beta){
    double answer = 0;

    answer += u((B[0] + A[0])/2.0,(B[1]+A[1])/2.0,uh,selection,Beta);
    answer += u((C[0] + A[0])/2.0,(C[1]+A[1])/2.0,uh,selection,Beta);
    answer += u((C[0] + B[0])/2.0,(C[1]+B[1])/2.0,uh,selection,Beta);

    return answer * triangleArea(A,B,C) / 3.0;

}

double L2Norm_Error(Mesh trial_space, Vector u_approx, int selection, Vector Beta){

    double total = 0;
    Vector A(2), B(2),C(2);

    for(int i = 0; i < trial_space.getNum_Elements(); i++){

        A = trial_space.getNode(trial_space.getConnectivity(0,i)-1);
        B = trial_space.getNode(trial_space.getConnectivity(1,i)-1);
        C = trial_space.getNode(trial_space.getConnectivity(2,i)-1);

        total += triangleIntegral_MidpointRule(A,B,C,squaredError,u_approx.Read(i),selection,Beta);
    }

    return sqrt(total);
}

double L2Norm_drdy(Vector Solution, Matrix Square, int u_size){
    double total = 0;

    for(int i = 0; i < Solution.GetSize() - u_size; i++){

        for(int j = 0; j < Solution.GetSize() - u_size; j++){
            total += Square.Read(i,j)*Solution.Read(i)*Solution.Read(j);
        }
    }
    return sqrt(total);
}

double source_integrand(double x, double y, BasisFunc v, int selection){
    Vector P(2); P[0] = x; P[1] = y;

    return source_data(x,y,selection)*v.getZ(P);
}

double triangleIntegral_MidpointRule_FOR_F(Vector A, Vector B, Vector C, double (*u)(double,double,BasisFunc,int),BasisFunc v,int selection){
    double answer = 0;

    answer += u((B[0] + A[0])/2.0,(B[1]+A[1])/2.0,v,selection);
    answer += u((C[0] + A[0])/2.0,(C[1]+A[1])/2.0,v,selection);
    answer += u((C[0] + B[0])/2.0,(C[1]+B[1])/2.0,v,selection);

    return answer * triangleArea(A,B,C) / 3.0;

}

double source_integral(BasisFunc v, double (*f)(double,double,BasisFunc,int),int selection){
    double total = 0;

    Vector A(2), B(2), C(2);

    for(int i = 0; i < v.getNumElements(); i++){
        A = v.getNode(v.getElementIndex(0,i));
        B = v.getNode(v.getElementIndex(1,i));
        C = v.getNode(v.getElementIndex(2,i));

        total += triangleIntegral_MidpointRule_FOR_F(A,B,C,source_integrand,v,selection);
    }

    return total;

}

//this function does the weird things with corners right - see page 40 in report for an explanation
bool isSpecialCase(Vector Beta, Vector center){
    if(Beta[0] != 0 && Beta[1] != 0){
        if(center[0] == 0 && center[1] == 0){
            if(Beta.Read(0) < 0.001 && Beta.Read(1) > 0.001){
                return true;
            }
            if(Beta.Read(0) > 0.001 && Beta.Read(1) < 0.001){
                return true;
            }
        }
        if(center[0] == 0 && center[1] == 1){
            if(Beta.Read(0) < 0.001 && Beta.Read(1) < 0.001){
                return true;
            }
            if(Beta.Read(0) > 0.001 && Beta.Read(1) > 0.001){
                return true;
            }
        }
        if(center[0] == 1 && center[1] == 0){
            if(Beta.Read(0) < 0.001 && Beta.Read(1) < 0.001){
                return true;
            }
            if(Beta.Read(0) > 0.001 && Beta.Read(1) > 0.001){
                return true;
            }
        }
        if(center[0] == 1 && center[1] == 1){
            if(Beta.Read(0) < 0.001 && Beta.Read(1) > 0.001){
                return true;
            }
            if(Beta.Read(0) > 0.001 && Beta.Read(1) < 0.001){
                return true;
            }
        }
    }
    return false;
}
