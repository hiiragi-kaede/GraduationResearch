#pragma once

#include<vector>

using std::vector;

double TotalDistance(const vector<vector<int>> orders,
                        const vector<vector<double>> dis_mat);

int TotalWeight(const vector<int> order,const vector<int> weights);

void ShowUnvisited(const vector<vector<int>>orders,const vector<int> weights,int n);

bool IsExistUnvisited(const vector<vector<int>>orders,const vector<int> weights,int n);

void ShowOrdersInfo(const vector<vector<int>> orders,const vector<int> weights,
                const int capacity,const int n);

vector<vector<int>> comb(int n, int r);

vector<vector<int>> ConstructNeighborList(const int n,const vector<vector<double>> dis_mat);

void ShowTruckIds(vector<int> truck_ids);