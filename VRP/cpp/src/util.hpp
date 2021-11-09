#pragma once

#include<vector>

using std::vector;

void two_opt(vector<int>& order,const vector<vector<double>> dis_mat);

void cross_exchange_neighbor(const vector<int> weights,vector<vector<int>>& orders,
                            const vector<vector<double>> dis_mat,const int truck_capacity);

bool sub_cross(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<double>> dis_mat,const int truck_capacity,
                const int i,const int j);

bool check_weight(const vector<int> order_i,const vector<int> order_j,
                const vector<int> weights,const int fst_weight,const int sec_weight,
                const int truck_capacity);

double calc_total_dist(const vector<vector<int>> orders,
                        const vector<vector<double>> dis_mat);

int calc_total_weight(const vector<int> order,const vector<int> weights);

void check_unvisited(const vector<vector<int>>orders,const vector<int> weights,int n);

void show_orders(const vector<vector<int>> orders,const vector<int> weights,
                const int capacity,const int n);

vector<vector<int>> comb(int n, int r);