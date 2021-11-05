#include<vector>
#include<pybind11/pybind11.h>
#include<pybind11/stl.h>

namespace py = pybind11;
using std::vector;

int add(int x,int y){
    return x+y;
}

vector<vector<int>> get_mat(const int size){
    vector<vector<int>> mat(size+1,vector<int>(size,0));

    for(int i=0; i<size+1; i++){
        for(int j=0; j<size; j++){
            mat[i][j]=i*size+j;
        }
    }
    
    return mat;
}

PYBIND11_MODULE(test, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers");
    m.def("get_mat",&get_mat,"Get 2 dimention vector");
}