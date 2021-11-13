#pragma once

#include<vector>

using std::vector;

double calc_total_dist(const vector<vector<int>> orders,
                        const vector<vector<double>> dis_mat);

int calc_total_weight(const vector<int> order,const vector<int> weights);

void show_unvisited(const vector<vector<int>>orders,const vector<int> weights,int n);

bool is_exist_unvisited(const vector<vector<int>>orders,const vector<int> weights,int n);

void show_orders_info(const vector<vector<int>> orders,const vector<int> weights,
                const int capacity,const int n);

vector<vector<int>> comb(int n, int r);

vector<vector<int>> construct_neighbor_list(const int n,const vector<vector<double>> dis_mat);

void show_truck_ids(vector<int> truck_ids);