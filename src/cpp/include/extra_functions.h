#ifndef EXTRA_FUNCTIONS_H_INCLUDED
#define EXTRA_FUNCTIONS_H_INCLUDED

void remove(std::vector<int> &v);
double boundary_function(double x, double y,int selection);
double squaredError(double x, double y, double uh,int selection);
double trapezium_on_boundary(Vector Start, Vector End, double (*g)(double,double,int), BasisFunc V,Vector Beta, Vector unit_normal,int selection);
bool isPositiveDefinite(Matrix Test);
Vector DDMRes_Peterson(int n,int selection, int refinement_scheme, Vector Beta);
double triangleArea(Vector A, Vector B, Vector C);
double triangleIntegral_MidpointRule(Vector A, Vector B, Vector C, double (*u)(double,double,double,int),double uh,int selection, Vector Beta);
double L2Norm_Error(Mesh trial_space, Vector u_approx, int selection,Vector Beta);
double u_exact(double x, double y, int selection, Vector Beta);
double L2_drdy(Vector Solution, Matrix Square, int u_size);
double Flow_Normal_Product(BasisFunc V, Vector Beta);
bool isOnBoundary(Vector center_node);
Vector getUnitNormal(Vector check);
double source_integrand(double x, double y, BasisFunc v, int selection);
double source_integral(BasisFunc v, double (*f)(double,double,BasisFunc,int),int selection);
double L2Norm_drdy(Vector Solution, Matrix Square, int u_size);
bool isSpecialCase(Vector Beta, Vector center);

#endif // EXTRA_FUNCTIONS_H_INCLUDED
