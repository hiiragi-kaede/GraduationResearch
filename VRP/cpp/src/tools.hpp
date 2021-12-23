#pragma once

#include<vector>
#include<set>
#include"improve.hpp"

using namespace std;

enum class MethodType{
    TwoOptStar,
    FastTwoOptStar,
    ImprovedTwoOptStar,
    Cross,
    FastCross,
    ImprovedCross,
    IteratedTwo,
    IteratedCross
};

void TrialInsertConstruct(const vector<vector<int>>& dis_mat,const vector<int>& weights,
                        const int capacity,const int truck_size,
                        vector<vector<int>>& orders,vector<pair<int,int>>& truck_ids,
                        int CONSTRUCT_LIMIT_MS,int THREAD_SIZE,int seed);

void ThreadProcess(const vector<vector<int>>& dis_mat,const vector<int>& weights,
                    const int capacity,const int truck_size,
                    long long& construct_ms,long long& local_search_sec,int& bef_dist,int& aft_dist,
                    vector<vector<int>>& ans_orders,const vector<set<int>>& nn_list,vector<int>& lattice,
                    MethodType method_type,int THREAD_SIZE,int ITERATED_SIZE,int CONSTRUCT_LIMIT_MS,int seed);

void ShowThreadsInfos(int THREAD_SIZE,int minid,const vector<vector<vector<int>>>& thread_orders,
                    const vector<long long>& constructs,const vector<long long>& local_searches,
                    const vector<int>& befs,const vector<int>& afts,int capacity,int n,
                    const vector<int>& weights);

void ShowThreadsAves(int THREAD_SIZE,const vector<long long>& local_searches,
                    const vector<int>& befs,const vector<int>& afts,int minid);

vector<string> GetTypeNames();

void InputData(int& n,int& capacity,vector<int>& cus_x,vector<int>& cus_y,vector<int>& weights);

void OutputData(int n,const vector<int>& cus_x,const vector<int>& cus_y,
                const vector<int>& weights,const vector<vector<int>>& orders,
                string data_fname,string order_fname);

void OutputLattice(const vector<int> lattice,int LatticeSize,
                    const string lattice_fname);

void SetArgs(int argc,char *argv[],MethodType& method_type,KickType& kick_type,
            IteratedType& iterated_type,bool& use_tabulist,bool& use_lattice,bool& use_hull,
            int& seed_offset);