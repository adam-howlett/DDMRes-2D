#ifndef MESH_H
#define MESH_H
#include "Matrix.h"

class Mesh
{
    public:
        Mesh();
        virtual ~Mesh();
        void Peterson(int n);

        void dispElements();
        void dispVertices();
        void Output_Data();
        void Refine();
        void Vertical_Refine(int n);

        //Access Methods
        int getConnectivity(int i, int j);
        double getVertex(int i, int j);
        int getNum_Elements();
        int getNum_Vertices();
        Vector getNode(int i);
        Vector getMidpoint(int i);

    protected:

    private:
        int Num_Elements;
        int Num_Vertices;
        int triangulation_degree;
        Matrix Vertices;
        Matrix Connectivity_Array;
};

#endif // MESH_H
