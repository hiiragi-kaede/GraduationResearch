#pragma once

#include<vector>
#include<set>

using namespace std;

void TwoOpt(vector<int>& order,const vector<vector<float>>& dis_mat);

void DoubleBridge(vector<vector<int>>& orders);

bool FourOptStar(vector<vector<int>>& orders,const vector<int>& weights,int truck_capacity);

bool SubFourOptStar(vector<vector<int>>& orders,const vector<int>& weights,
                    int fst,int sec,int third,int fourth,int truck_capacity);

void UpdateFourOpt(vector<vector<int>>& orders,int fst,int sec,
                int third,int fourth,int fst_id,int sec_id,
                int third_id,int fourth_id);

double GetCrossExDiff(const vector<vector<float>>& dis_mat,const vector<vector<int>>& orders,
                    int i,int j,int i_st,int i_end,int j_st,int j_end);

double GetTwoOptStarDiff(const vector<vector<float>>& dis_mat,const vector<vector<int>>& orders,
                        int i,int j,int i_id,int j_id);

void UpdateCrossOrders(vector<vector<int>>& orders,const vector<vector<float>>& dis_mat,int i,int j,
                        int i_st,int i_end,int j_st,int j_end,int fst_size,int sec_size);

void UpdateTwoOptStarOrders(vector<vector<int>>& orders,const vector<vector<float>>& dis_mat,
                            int i,int j,int fst_size,int sec_size,int i_dif,int j_dif,
                            int i_id,int j_id);

void CrossExchangeNeighbor(const vector<int>& weights,vector<vector<int>>& orders,
                            const vector<vector<float>>& dis_mat,const int truck_capacity);

bool SubCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                const int i,const int j);

void ImprovedCrossExchangeNeighbor(const vector<int>& weights,vector<vector<int>>& orders,
                            const vector<vector<float>>& dis_mat,const int truck_capacity,
                            const vector<int>& lattice);

bool SubImprovedCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                const int i,const int j);

void FastCrossExchange(const vector<int>& weights,vector<vector<int>>& orders,
                        const vector<vector<float>>& dis_mat,const int truck_capacity,
                        vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list);

bool SubFastCross(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list,
                    int i,int cus);

void TwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity);

bool SubTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    const int i,const int j);

void FastTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list);

bool SubFastTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list);

void ImprovedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                const vector<int>& lattice);

bool SubImprovedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    const int i,const int j);

void IteratedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                int iterated_size,const vector<int>& lattice,bool out=true);

void IteratedCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                int iterated_size,const vector<int>& lattice,bool out=true);

bool IsValidWeight(const vector<int>& order_i,const vector<int>& order_j,
                const vector<int>& weights,const int fst_weight,const int sec_weight,
                const int truck_capacity);

void Kick(vector<vector<int>>& orders,const vector<int>& weights,int capacity);

vector<set<int>> ConstructTruckLatticeList(const vector<vector<int>>& orders,const vector<int>& lattice);

void UpdateLatticeList(const vector<vector<int>>& orders,int id,const vector<int>& lattice,
                        vector<set<int>>& lattice_list);