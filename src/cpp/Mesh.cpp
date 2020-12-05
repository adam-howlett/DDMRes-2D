#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "include/Mesh.h"

Mesh::Mesh()
{
    Num_Elements = 0;
    Num_Vertices = 0;
    //ctor
}

Mesh::~Mesh()
{
    //dtor
}

void Mesh::Peterson(int n){

    Num_Elements = round(4*pow(n,2) + 2*n);
    Num_Vertices = round(2*pow(n,2) + 4*n + 1);
    triangulation_degree = n;

    Vertices.SetRectangle(2,Num_Vertices);
    Connectivity_Array.SetRectangle(3,Num_Elements);

    //Setting Up Vertex Array (Co-ordinates of each vertex in order)
    double h = 1/(double)n;
    int k = 0;
    for(int j = 0; j < ((2*n)+1); j++){
        for(int i = 0; i < (n+1); i++){
            if( j % 2 == 0 || i == 0){
                Vertices.Write(0,k,i*h);
            }
            else{
                Vertices.Write(0,k,h/2.0 + (i-1)*h);
            }
            Vertices.Write(1,k,j*h/2.0);
            k++;
        }
        if(j % 2 != 0){
            Vertices.Write(0,k,1.0);
            Vertices.Write(1,k,j*h/2.0);
            k++;
        }
    }

    //Setting up Connectivity Array (the indices in the vertex array associated with each triangle)

    int hold, row_no;
    for(int i = 1; i < Num_Elements + 1; i++){
        row_no = floor((double)(i-1)/((2.0*(double)n)+1));
        hold = n + 2 + floor(i/2) + row_no;
        if(i % 2 == 1){
            Connectivity_Array.Write(0,i-1,row_no + ceil(i/2.0));
            Connectivity_Array.Write(1,i-1,hold);
            Connectivity_Array.Write(2,i-1,hold+1);
        }
        else{
            Connectivity_Array.Write(0,i-1,i/2 + row_no);
            Connectivity_Array.Write(1,i-1,i/2 + row_no + 1);
            Connectivity_Array.Write(2,i-1,hold);
        }

    }


}

void Mesh::dispElements(){

    cout << "Connectivity Array for n = " << triangulation_degree << endl;
    for(int i = 0; i < Num_Elements; i++){
        cout << i+1 << ": " << Connectivity_Array.Read(0,i) << " " << Connectivity_Array.Read(1,i) << " " << Connectivity_Array.Read(2,i) << endl;
    }
    cout << endl;
}

void Mesh::dispVertices(){
    cout << "Vertices for for n = " << triangulation_degree << endl;
    for(int i = 0; i < Num_Vertices; i++){
        cout << i+1 << ": " << Vertices.Read(0,i) << " " << Vertices.Read(1,i) << endl;
    }
    cout << endl;
}

void Mesh::Output_Data(){
    ofstream outfile;
    outfile.open("NodePoints.txt");
    outfile << Vertices;
    outfile.close();

    outfile.open("ConnectivityArray.txt");
    outfile << Connectivity_Array;
    outfile.close();

}

void Mesh::Refine(){

    //loop over elements
    //take two vertices
    //check they haven't been paired before
    //loop over elements up to that number and check they aren't both in one

    vector<double> vertex_x;
    vector<double> vertex_y;

    vector<double> used_one;
    vector<double> used_two;


    Matrix NewNodesForOldElement_x(3,Num_Elements);
    Matrix NewNodesForOldElement_y(3,Num_Elements);

    Matrix OldVertices(2,Num_Vertices);

    for(int i = 0; i < Num_Vertices; i++){
        for(int j = 0; j < 2; j++){
            OldVertices.Write(j,i,Vertices.Read(j,i));
        }
    }

    int index_one, index_two;
    bool used = false;


    double x_avg, y_avg;

    for(int i = 0; i < vertex_x.size(); i++){
        cout << vertex_x[i] << " " << vertex_y[i] << endl;
    }

    //at the end, num vertices is just added to vertex_x/y.length

    //Loop generates new node points, stores them in vectors vertex_x/y in correct places on mesh
    double x1,x2,y1,y2;
    for(int i = 0; i < Num_Elements; i++){

        for(int combination = 0; combination < 3; combination++){
            used = false;

            if(combination == 0){
                index_one = 1;
                index_two = 2;
            }
            else{
                index_one = 0;
                index_two = combination;
            }

            //Check if these indices have been used before
            for(int j = 0; j < used_one.size(); j++){
                if(Connectivity_Array.Read(index_one,i) == used_one[j] && Connectivity_Array.Read(index_two,i) == used_two[j]){
                    used = true;
                }
                if(Connectivity_Array.Read(index_two,i) == used_one[j] && Connectivity_Array.Read(index_one,i) == used_two[j]){
                    used = true;
                }
            }

            x1 = Vertices.Read(0,Connectivity_Array.Read(index_one,i)-1);
            x2 = Vertices.Read(0,Connectivity_Array.Read(index_two,i)-1);
            y1 = Vertices.Read(1,Connectivity_Array.Read(index_one,i)-1);
            y2 = Vertices.Read(1,Connectivity_Array.Read(index_two,i)-1);

            x_avg = (x1 + x2) * 0.5;
            y_avg = (y1 + y2) * 0.5;

            NewNodesForOldElement_x.Write(combination,i,x_avg);
            NewNodesForOldElement_y.Write(combination,i,y_avg);

            if(!used){
                vertex_x.push_back(x_avg);
                vertex_y.push_back(y_avg);
                used_one.push_back(Connectivity_Array.Read(index_one,i));
                used_two.push_back(Connectivity_Array.Read(index_two,i));

            }

        }
    }



    // // // // //
    //Building the new Vertex Matrix
    for(int i = 0; i < Num_Vertices; i++){
        vertex_x.push_back(Vertices.Read(0,i));
        vertex_y.push_back(Vertices.Read(1,i));
    }


    //now vectors vertex_x/y have all the nodes (old and new), but in the wrong order. We order in terms of lowest y, then lowest x
    Vertices.Clear();
    Num_Vertices = vertex_x.size();
    Vertices.SetRectangle(2,Num_Vertices);

    double lowest_y;
    double lowest_x;
    int ind;
    for(int i = 0; i < Num_Vertices; i++){
        lowest_y = 1;
        lowest_x = 1;

        //look for the the lowest y value left
        for(int j = 0; j < vertex_y.size(); j++){
            if(vertex_y[j] < lowest_y){
                lowest_y = vertex_y[j];
            }
        }
        //look for the lowest x value associated with the lowest y value
        for(int j = 0; j < vertex_x.size(); j++){
            if(vertex_y[j] == lowest_y && vertex_x[j] <= lowest_x){
                lowest_x = vertex_x[j];
                ind = j;
            }
        }

        Vertices.Write(0,i,vertex_x[ind]);
        Vertices.Write(1,i,vertex_y[ind]);
        vertex_x.erase(vertex_x.begin() + ind );
        vertex_y.erase(vertex_y.begin() + ind );


    }





    //Need to generate new Matrix for new elements - can do this differently because the size of the Matrix is always 4 times the previous one
    //For general mesh - so don't need to add until we're done, can just redefine new Matrix as such

    Matrix New_Elements(3,Num_Elements*4);

    //Now we loop over all the previous elements and assign new ones in a structured way

    int writeto = -1;
    for(int i = 0; i < Num_Elements; i++){

        //Each Element has 4 new elements to make
        for(int j = 0; j < 4; j++){
            writeto++;
            //Each new element (apart from the middle one) uses exactly one of the old nodes, so we will use this fact to construct them, and order them
            //in terms of the size of the old indexing
            //0,1,2 are the correct indices of the old triangles to reference, we'll take 3 to mean the 'all new nodes' triangle

            //so we have that the old index is the jth node of the ith element in the OLD connectivity array

            //BUT, we want to store this node in terms of the new vertex array -> we have to loop over all vertices until we find a match for x AND y, then take its index
            if(j != 3){

                //Find the point in terms of its vertex in the new vertex array
                for(int k = 0; k < Num_Vertices; k++){
                    if(Vertices.Read(0,k) == OldVertices.Read(0,Connectivity_Array.Read(j,i)-1)
                            && Vertices.Read(1,k) == OldVertices.Read(1,Connectivity_Array.Read(j,i)-1)){
                            ind = k;
                       }

                }
                New_Elements.Write(0,writeto,ind + 1);

                //now we need the two new points that are relevant
                //luckily we can do the same iteration, referring to them by their places in the NewNodesForOldElement Matrix
                //but we first need to find the two CLOSEST points, so exclude the irrelevant one

                //This is AWFULLLLLLLLL
                double dist_1, dist_2, dist_3;
                double x_new1, y_new1, x_new2, y_new2;


                dist_1 = sqrt(pow(NewNodesForOldElement_x.Read(0,i) - Vertices.Read(0,ind),2) + pow(NewNodesForOldElement_y.Read(0,i) - Vertices.Read(1,ind),2));
                dist_2 = sqrt(pow(NewNodesForOldElement_x.Read(1,i) - Vertices.Read(0,ind),2) + pow(NewNodesForOldElement_y.Read(1,i) - Vertices.Read(1,ind),2));
                dist_3 = sqrt(pow(NewNodesForOldElement_x.Read(2,i) - Vertices.Read(0,ind),2) + pow(NewNodesForOldElement_y.Read(2,i) - Vertices.Read(1,ind),2));

                if(dist_1 < dist_2 ){
                    x_new1 = NewNodesForOldElement_x.Read(0,i);
                    y_new1 = NewNodesForOldElement_y.Read(0,i);

                    if(dist_2 < dist_3){
                        x_new2 = NewNodesForOldElement_x.Read(1,i);
                        y_new2 = NewNodesForOldElement_y.Read(1,i);
                    }
                    else{
                        x_new2 = NewNodesForOldElement_x.Read(2,i);
                        y_new2 = NewNodesForOldElement_y.Read(2,i);
                    }
                }
                else{
                    x_new1 = NewNodesForOldElement_x.Read(1,i);
                    y_new1 = NewNodesForOldElement_y.Read(1,i);

                    if(dist_1 < dist_3){
                        x_new2 = NewNodesForOldElement_x.Read(0,i);
                        y_new2 = NewNodesForOldElement_y.Read(0,i);
                    }
                    else{
                        x_new2 = NewNodesForOldElement_x.Read(2,i);
                        y_new2 = NewNodesForOldElement_y.Read(2,i);
                    }
                }


                //closest two indices found, now need to match them to vertices on the new array

                int ind2,ind3;
                for(int k = 0; k < Num_Vertices; k++){
                    if(Vertices.Read(0,k) == x_new1 && Vertices.Read(1,k) == y_new1){
                        ind2 = k;
                    }
                    if(Vertices.Read(0,k) == x_new2 && Vertices.Read(1,k) == y_new2){
                        ind3 = k;
                    }
                }

                New_Elements.Write(1,writeto,ind2 + 1);
                New_Elements.Write(2,writeto,ind3 + 1);




            }
            else{
                //Just Add in all the new nodes as the final triangle
                for(int k = 0; k < Num_Vertices; k++){
                    if(NewNodesForOldElement_x.Read(0,i) == Vertices.Read(0,k) && NewNodesForOldElement_y.Read(0,i) == Vertices.Read(1,k)){
                        New_Elements.Write(0,writeto,k + 1);
                    }
                    if(NewNodesForOldElement_x.Read(1,i) == Vertices.Read(0,k) && NewNodesForOldElement_y.Read(1,i) == Vertices.Read(1,k)){
                        New_Elements.Write(1,writeto,k + 1);
                    }
                    if(NewNodesForOldElement_x.Read(2,i) == Vertices.Read(0,k) && NewNodesForOldElement_y.Read(2,i) == Vertices.Read(1,k)){
                        New_Elements.Write(2,writeto,k + 1);
                    }
                }

            }


        }


    }

    Connectivity_Array.Clear();
    Num_Elements *= 4;
    Connectivity_Array.SetRectangle(3,Num_Elements);

    //Now need to order the indices in size
    int hold;
    for(int i = 0; i < Num_Elements; i++){

        for(int j = 0; j < 3; j++){
            Connectivity_Array.Write(j,i,New_Elements.Read(j,i));
        }

        while(!(Connectivity_Array.Read(0,i) < Connectivity_Array.Read(1,i) && Connectivity_Array.Read(1,i) < Connectivity_Array.Read(2,i))){
            if(Connectivity_Array.Read(0,i) > Connectivity_Array.Read(1,i)){
                hold = Connectivity_Array.Read(0,i);
                Connectivity_Array.Write(0,i,Connectivity_Array.Read(1,i));
                Connectivity_Array.Write(1,i,hold);
            }
            if(Connectivity_Array.Read(1,i) > Connectivity_Array.Read(2,i)){
                hold = Connectivity_Array.Read(1,i);
                Connectivity_Array.Write(1,i,Connectivity_Array.Read(2,i));
                Connectivity_Array.Write(2,i,hold);
            }

        }


    }
}

int Mesh::getConnectivity(int i, int j){
    return Connectivity_Array.Read(i,j);
}

double Mesh::getVertex(int i, int j){
    return Vertices.Read(i,j);
}

int Mesh::getNum_Elements(){
    return Num_Elements;
}

int Mesh::getNum_Vertices(){
    return Num_Vertices;
}

Vector Mesh::getMidpoint(int i){
    Vector answer(2);
    answer[0] = (getVertex(0,Connectivity_Array.Read(0,i)-1) + getVertex(0,Connectivity_Array.Read(1,i)-1) + getVertex(0,Connectivity_Array.Read(2,i)-1))/3.0;
    answer[1] = (getVertex(1,Connectivity_Array.Read(0,i)-1) + getVertex(1,Connectivity_Array.Read(1,i)-1) + getVertex(1,Connectivity_Array.Read(2,i)-1))/3.0;

    return answer;
}

Vector Mesh::getNode(int i){
    Vector answer(2);
    answer[0] = getVertex(0,i);
    answer[1] = getVertex(1,i);

    return answer;
}

void Mesh::Vertical_Refine(int n){

    //this is a "refinement" of a general peterson mesh technically, but it's not built on the previous mesh per se -  rather constructed in and of itself.
    //As a result, it can be constructed without building a peterson mesh in the code prior.
    //x gaps every h/2
    //go up in steps of h/2, check if point is a node, if not add as a node
    int strip = 2*n + 1;
    Num_Vertices = pow(strip,2);

    double h = 1.0/n;
    double x = 0, y = 0;

    Vertices.Clear();
    Vertices.SetRectangle(2,Num_Vertices);
    int i = 0;

    while(y <= 1){
        while(x <=1){

            Vertices.Write(0,i,x);
            Vertices.Write(1,i,y);

            x += h/2.0;
            i++;
        }
        x = 0;
        y += h/2.0;
    }

    //Now need to set up connectivity array
    Num_Elements = 8.0*pow(n,2);
    Connectivity_Array.Clear();
    Connectivity_Array.SetRectangle(3,Num_Elements);

    i = 1;
    int j = 0;

    while(i < Num_Vertices - strip +1){
        if(i % strip == 1){
            //The node is on the LHS of the grid
            if( i % 2 == 0){
                Connectivity_Array.Write(0,j,i);
                Connectivity_Array.Write(1,j,i+1);
                Connectivity_Array.Write(2,j,i+strip);
            }
            else{
                Connectivity_Array.Write(0,j,i);
                Connectivity_Array.Write(1,j,i+strip);
                Connectivity_Array.Write(2,j,i+strip+1);
            }
        }
        else if(i%strip == 0){
            //The node is on the RHS of the grid
                if( i % 2 == 0){
                Connectivity_Array.Write(0,j,i-1);
                Connectivity_Array.Write(1,j,i);
                Connectivity_Array.Write(2,j,i+strip);
            }
            else{
                Connectivity_Array.Write(0,j,i);
                Connectivity_Array.Write(1,j,i+strip-1);
                Connectivity_Array.Write(2,j,i+strip);
            }
        }
        else{
            //The node is somewhere in the middle
            if( i % 2 == 0){
                Connectivity_Array.Write(0,j,i-1);
                Connectivity_Array.Write(1,j,i);
                Connectivity_Array.Write(2,j,i+strip);
                j++;
                Connectivity_Array.Write(0,j,i);
                Connectivity_Array.Write(1,j,i+1);
                Connectivity_Array.Write(2,j,i+strip);

            }
            else{
                Connectivity_Array.Write(0,j,i);
                Connectivity_Array.Write(1,j,i+strip-1);
                Connectivity_Array.Write(2,j,i+strip);
                j++;
                Connectivity_Array.Write(0,j,i);
                Connectivity_Array.Write(1,j,i+strip);
                Connectivity_Array.Write(2,j,i+strip+1);
            }
        }

        i++;
        j++;
    }
}

