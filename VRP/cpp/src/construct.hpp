#pragma once

#include<vector>
#include<utility>
using std::vector;
using std::pair;

vector<vector<int>> InsertConstruct(const vector<vector<int>>& dis_mat,const vector<int>& weights,
                                    int truck_capacity,int truck_size,vector<pair<int,int>>& truck_ids,
                                    int seed,bool construct_randomly=false);

vector<vector<int>> RandomConstruct(const vector<int>& weights,int truck_capacity);