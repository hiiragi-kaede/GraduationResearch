#pragma once

#include<vector>
#include<utility>
using std::vector;
using std::pair;

vector<vector<int>> InsertConstruct(const vector<vector<float>>& dis_mat,const vector<int>& weights,
                                    int truck_capacity,int truck_size,vector<pair<int,int>>& truck_ids);