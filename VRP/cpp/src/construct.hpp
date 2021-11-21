#pragma once

#include<vector>
#include<utility>
using std::vector;
using std::pair;

vector<vector<int>> InsertConstruct(vector<vector<float>> dis_mat,vector<int> weights,
                                    int truck_capacity,int truck_size,vector<pair<int,int>>& truck_ids);