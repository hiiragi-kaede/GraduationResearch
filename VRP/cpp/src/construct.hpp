#pragma once

#include<vector>
using std::vector;

vector<vector<int>> InsertConstruct(vector<vector<double>> dis_mat,vector<int> weights,
                                    int truck_capacity,int truck_size,vector<int>& truck_ids);