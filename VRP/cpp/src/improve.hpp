#pragma once

#include<vector>

using std::vector;

void two_opt(vector<int>& order,const vector<vector<double>> dis_mat);

void cross_exchange_neighbor(const vector<int> weights,vector<vector<int>>& orders,
                            const vector<vector<double>> dis_mat,const int truck_capacity,
                            vector<int>& truck_ids);

bool sub_cross(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<double>> dis_mat,const int truck_capacity,
                const int i,const int j,vector<int>& truck_ids);

bool is_valid_weight(const vector<int> order_i,const vector<int> order_j,
                const vector<int> weights,const int fst_weight,const int sec_weight,
                const int truck_capacity);