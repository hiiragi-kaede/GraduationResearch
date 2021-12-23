#pragma once

#include<vector>
#include<set>
#include<string>
#include<random>

using namespace std;

enum class KickType{
    DoubleBridge,
    FourOpt
};

enum class IteratedType{
    Normal,
    Improved
};

vector<string> GetKickTypes();

vector<string> GetIteratedTypes();

void TwoOpt(vector<int>& order,const vector<vector<int>>& dis_mat);

void DoubleBridge(vector<vector<int>>& orders,mt19937 engine);

bool FourOptStar(vector<vector<int>>& orders,const vector<int>& weights,int truck_capacity,mt19937 engine);

bool SubFourOptStar(vector<vector<int>>& orders,const vector<int>& weights,
                    int fst,int sec,int third,int fourth,int truck_capacity,mt19937 engine);

void UpdateFourOpt(vector<vector<int>>& orders,int fst,int sec,
                int third,int fourth,int fst_id,int sec_id,
                int third_id,int fourth_id);

double GetCrossExDiff(const vector<vector<int>>& dis_mat,const vector<vector<int>>& orders,
                    int i,int j,int i_st,int i_end,int j_st,int j_end);

double GetTwoOptStarDiff(const vector<vector<int>>& dis_mat,const vector<vector<int>>& orders,
                        int i,int j,int i_id,int j_id);

void UpdateCrossOrders(vector<vector<int>>& orders,const vector<vector<int>>& dis_mat,int i,int j,
                        int i_st,int i_end,int j_st,int j_end,int fst_size,int sec_size);

void UpdateTwoOptStarOrders(vector<vector<int>>& orders,const vector<vector<int>>& dis_mat,
                            int i,int j,int fst_size,int sec_size,int i_dif,int j_dif,
                            int i_id,int j_id);

void CrossExchangeNeighbor(const vector<int>& weights,vector<vector<int>>& orders,
                            const vector<vector<int>>& dis_mat,const int truck_capacity);

bool SubCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                const int i,const int j);

void ImprovedCrossExchangeNeighbor(const vector<int>& weights,vector<vector<int>>& orders,
                            const vector<vector<int>>& dis_mat,const int truck_capacity,
                            const vector<int>& lattice);

bool SubImprovedCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                const int i,const int j);

void FastCrossExchange(const vector<int>& weights,vector<vector<int>>& orders,
                        const vector<vector<int>>& dis_mat,const int truck_capacity,
                        vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list);

bool SubFastCross(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<int>>& dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list,
                    int i,int cus);

void TwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity);

bool SubTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<int>>& dis_mat,const int truck_capacity,
                    const int i,const int j);

void FastTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list);

bool SubFastTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<int>>& dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list);

void ImprovedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                const vector<int>& lattice);

bool SubImprovedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<int>>& dis_mat,const int truck_capacity,
                    const int i,const int j);

void IteratedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                int iterated_size,const vector<int>& lattice,int seed,bool out=true);

void IteratedCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                int iterated_size,const vector<int>& lattice,int seed,bool out=true);

bool IsValidWeight(const vector<int>& order_i,const vector<int>& order_j,
                const vector<int>& weights,const int fst_weight,const int sec_weight,
                const int truck_capacity);

void Kick(vector<vector<int>>& orders,const vector<int>& weights,int capacity,mt19937 engine);

vector<set<int>> ConstructTruckLatticeList(const vector<vector<int>>& orders,const vector<int>& lattice);

void UpdateLatticeList(const vector<vector<int>>& orders,int id,const vector<int>& lattice,
                        vector<set<int>>& lattice_list);