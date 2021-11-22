#pragma once

#include<vector>
#include<set>

using namespace std;

void TwoOpt(vector<int>& order,const vector<vector<float>> dis_mat);

double GetCrossExDiff(const vector<vector<float>> dis_mat,const vector<vector<int>> orders,
                    int i,int j,int i_st,int i_end,int j_st,int j_end);

void CrossExchangeNeighbor(const vector<int> weights,vector<vector<int>>& orders,
                            const vector<vector<float>> dis_mat,const int truck_capacity,
                            vector<pair<int,int>>& truck_ids);

bool SubCross(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<float>> dis_mat,const int truck_capacity,
                const int i,const int j,vector<pair<int,int>>& truck_ids);

void FastCrossExchange(const vector<int> weights,vector<vector<int>>& orders,
                        const vector<vector<float>> dis_mat,const int truck_capacity,
                        vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list);

bool SubFastCross(const vector<int> weights,vector<vector<int>>& orders,
                    const vector<vector<float>> dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list,
                    int i,int cus);

void TwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<float>> dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids);

bool SubTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                    const vector<vector<float>> dis_mat,const int truck_capacity,
                    const int i,const int j,vector<pair<int,int>>& truck_ids);

void FastTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<float>> dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list);

bool SubFastTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                    const vector<vector<float>> dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list);

bool IsValidWeight(const vector<int> order_i,const vector<int> order_j,
                const vector<int> weights,const int fst_weight,const int sec_weight,
                const int truck_capacity);