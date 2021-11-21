#pragma once

#include<vector>
#include<utility>
#include<set>

using std::vector;
using std::pair;
using std::set;

double TotalDistance(const vector<vector<int>> orders,
                        const vector<vector<float>> dis_mat);

int TotalWeight(const vector<int> order,const vector<int> weights);

void ShowUnvisited(const vector<vector<int>>orders,const vector<int> weights,int n);

bool IsExistUnvisited(const vector<vector<int>>orders,const vector<int> weights,int n);

void ShowOrdersInfo(const vector<vector<int>> orders,const vector<int> weights,
                const int capacity,const int n);

vector<vector<int>> comb(int n, int r);

vector<vector<int>> ConstructNeighborList(const int n,const vector<vector<float>> dis_mat);

vector<set<int>> ConstructNNList(const vector<vector<float>> dis_mat,
                                const vector<vector<int>> neighbor_list);

void UpdateTruckIds(const vector<vector<int>> orders,vector<pair<int,int>>& truck_ids);

void UpdateTruckIds(const vector<int> order,const int order_num,vector<pair<int,int>>& truck_ids);

void ShowTruckIds(const vector<pair<int,int>> truck_ids);