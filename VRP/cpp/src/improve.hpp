#pragma once

#include<vector>
#include<set>

using namespace std;

void TwoOpt(vector<int>& order,const vector<vector<double>> dis_mat);

void CrossExchangeNeighbor(const vector<int> weights,vector<vector<int>>& orders,
                            const vector<vector<double>> dis_mat,const int truck_capacity,
                            vector<pair<int,int>>& truck_ids);

bool SubCross(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<double>> dis_mat,const int truck_capacity,
                const int i,const int j,vector<pair<int,int>>& truck_ids);

void TwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<double>> dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids);

bool SubTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                    const vector<vector<double>> dis_mat,const int truck_capacity,
                    const int i,const int j,vector<pair<int,int>>& truck_ids);

void FastTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<double>> dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list);

bool SubFastTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                    const vector<vector<double>> dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list);

bool IsValidWeight(const vector<int> order_i,const vector<int> order_j,
                const vector<int> weights,const int fst_weight,const int sec_weight,
                const int truck_capacity);