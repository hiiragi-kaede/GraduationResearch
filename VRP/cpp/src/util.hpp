#pragma once

#include<vector>
#include<utility>
#include<set>

using std::vector;
using std::pair;
using std::set;

int TotalDistance(const vector<vector<int>>& orders,
                        const vector<vector<int>>& dis_mat);

int TotalDistance(const vector<int>& order,
                    const vector<vector<int>>& dis_mat);

int TotalWeight(const vector<int>& order,const vector<int>& weights);

int TotalWeight(vector<int>::iterator first,vector<int>::iterator last,const vector<int>& weights);

void ShowUnvisited(const vector<vector<int>>& orders,const vector<int>& weights,int n);

bool IsExistUnvisited(const vector<vector<int>>& orders,const vector<int>& weights,int n);

void ShowOrdersInfo(const vector<vector<int>>& orders,const vector<int>& weights,
                const int capacity,const int n);

vector<vector<int>> comb(int n, int r);

vector<vector<int>> ConstructNeighborList(const int n,const vector<vector<int>>& dis_mat);

vector<set<int>> ConstructNNList(const vector<vector<int>>& dis_mat,
                                const vector<vector<int>>& neighbor_list);

void UpdateTruckIds(const vector<vector<int>>& orders,vector<pair<int,int>>& truck_ids);

void UpdateTruckIds(const vector<int>& order,const int order_num,vector<pair<int,int>>& truck_ids);

void ShowTruckIds(const vector<pair<int,int>>& truck_ids);

namespace util{
    void ShowTwoOptStarDiffs(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids);
    
    void SubTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                const int i,const int j,vector<pair<int,int>>& truck_ids);
    
    void ShowDifInfos(int i,int j,double change_cost_dif,
                        const vector<vector<int>>& construct,
                        const vector<vector<int>>& orders,
                        bool isSimpled=false);

    void ShowFastTwoOptStarDiffs(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<int>>& dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list);
    
    void SubFastTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<int>>& dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list);
}

vector<int> ConstructContainingLatticeList(const vector<int>& cus_x,const vector<int>& cus_y,
                                                int LatticeSize);

bool IsCCW(const vector<int>& order);

bool IsCustomerInArea(const vector<int>& order,int cus_id);

bool IsOverlapOrders(const vector<vector<int>>& orders,int i,int j,
                    vector<vector<int>>& order_xs,vector<vector<int>>& order_ys);

bool IsHitBoundingBox(const vector<vector<int>>& orders,int i,int j,
                    const vector<vector<int>>& order_xs,const vector<vector<int>>& order_ys);

void UpdateOrderPos(const vector<vector<int>>& orders,vector<vector<int>>& order_xs,
                    vector<vector<int>>& order_ys);

void UpdateOrderPos(const vector<vector<int>>& orders,int i,int j,
                    vector<vector<int>>& order_xs,vector<vector<int>>& order_ys);