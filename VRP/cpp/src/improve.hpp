#pragma once

#include<vector>

using std::vector;
using std::pair;

void TwoOpt(vector<int>& order,const vector<vector<double>> dis_mat);

void CrossExchangeNeighbor(const vector<int> weights,vector<vector<int>>& orders,
                            const vector<vector<double>> dis_mat,const int truck_capacity,
                            vector<pair<int,int>>& truck_ids);

bool SubCross(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<double>> dis_mat,const int truck_capacity,
                const int i,const int j);

bool IsValidWeight(const vector<int> order_i,const vector<int> order_j,
                const vector<int> weights,const int fst_weight,const int sec_weight,
                const int truck_capacity);