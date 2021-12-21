#include<vector>
#include<iostream>
#include<string>
#include<stdio.h>
#include<cmath>
#include<thread>
#include<set>
#include"src/util.hpp"
#include"src/tools.hpp"
#include"src/improve.hpp"

/*
compile command
g++ -Wno-format-security -O3 -o main main.cpp src/construct.cpp src/util.cpp src/improve.cpp src/tools.cpp -pthread
./main < "source data file name"
*/
using namespace std;

static const int CONSTRUCT_LIMIT_MS=500;
static const int THREAD_SIZE=8;
static const int ITERATED_SIZE=10;
//どれだけ顧客の存在する範囲を分割して管理するか。この値の2乗個のマスで管理。
static const int LATTICE_SIZE=10;
static MethodType method_type=MethodType::TwoOptStar;
KickType kick_type=KickType::DoubleBridge;
IteratedType iterated_type=IteratedType::Improved;
bool use_tabulist=true;
bool use_lattice=true;
bool use_hull=true;

extern bool show_search_cnt;

vector<int> cus_x,cus_y;

int main(int argc,char *argv[]){
    /*==========data input==========*/
    int n,capacity;
    vector<int> weights;
    InputData(n,capacity,cus_x,cus_y,weights);

    SetArgs(argc,argv,method_type,kick_type,iterated_type,
            use_tabulist,use_lattice,use_hull);

    /*==========prepare for calculation==========*/
    vector<vector<int>> dis_mat(n,vector<int>(n,0));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            dis_mat[i][j]=round(hypot(cus_x[i]-cus_x[j],cus_y[i]-cus_y[j]));
        }
    }

    vector<vector<int>> neighbor_list=ConstructNeighborList(n,dis_mat);
    vector<set<int>> nn_list=ConstructNNList(dis_mat,neighbor_list);

    vector<int> all;
    for(int i=0; i<n; i++) all.push_back(i);
    int total_weight=TotalWeight(all,weights);
    cout<<"total_weight:"<<total_weight<<endl;
    int truck_size=ceil((double)total_weight/capacity);
    cout<<"truck_size:"<<truck_size<<endl;
    if(method_type==MethodType::IteratedCross||
        method_type==MethodType::IteratedTwo)
    {
        cout<<"iterated times:"<<ITERATED_SIZE<<endl;
    }
    vector<int> Lattice=ConstructContainingLatticeList(cus_x,cus_y,LATTICE_SIZE);
    if(method_type==MethodType::ImprovedCross||
        method_type==MethodType::ImprovedTwoOptStar||
        (method_type==MethodType::IteratedCross&&iterated_type==IteratedType::Improved)||
        (method_type==MethodType::IteratedTwo&&iterated_type==IteratedType::Improved))
    {
        cout<<"lattice size:"<<LATTICE_SIZE<<endl;
        cout<<"use tabu,lattice,hull:"<<use_tabulist<<","<<use_lattice<<","<<use_hull<<endl;
    }

    cout<<"neighbor type:"<<GetTypeNames()[static_cast<int>(method_type)]<<endl;
    if(method_type==MethodType::IteratedCross || method_type==MethodType::IteratedTwo){
        cout<<"kick method:"<<GetKickTypes()[static_cast<int>(kick_type)]<<endl;
        cout<<"iterated type:"<<GetIteratedTypes()[static_cast<int>(iterated_type)]<<endl;
    }

    if(THREAD_SIZE!=1) show_search_cnt=false;

    /*==========check difference between normal method and fast method==========*/
    // vector<vector<int>> test_orders;
    // vector<pair<int,int>> truck_ids(n);
    
    // TrialInsertConstruct(dis_mat,weights,capacity,truck_size,test_orders,truck_ids,
    //                     CONSTRUCT_LIMIT_MS,THREAD_SIZE);
    // for(auto& order: test_orders) if(IsCCW(order)) reverse(order.begin(),order.end());
    // for(int i=0; i<test_orders.size()-1; i++){
    //     for(int id=1; id<test_orders[i+1].size()-1; id++){
    //         if(IsCustomerInArea(test_orders[i],test_orders[i+1][id])){
    //             cout<<"contain\n";
    //             exit(1);
    //         }
    //     }
    // }
    // cout<<"construct\n";
    // ShowOrdersInfo(test_orders,weights,capacity,n);
    // FourOptStar(test_orders,weights,capacity);
    // cout<<"four opt\n";
    // ShowOrdersInfo(test_orders,weights,capacity,n);
    // exit(1);
    // cout<<test_orders.size()<<endl;
    // double const_score=TotalDistance(test_orders,dis_mat);
    // CrossExchangeNeighbor(weights,test_orders,dis_mat,capacity,truck_ids);
    // double cross_score=TotalDistance(test_orders,dis_mat);
    // //util::ShowTwoOptStarDiffs(weights,test_orders,dis_mat,capacity,truck_ids);
    // util::ShowFastTwoOptStarDiffs(weights,test_orders,dis_mat,capacity,truck_ids,nn_list);
    // cout<<"construct:"<<const_score<<"   cross25seconds:"<<cross_score<<endl;

    /*==========solve problem using multiple thread==========*/
    vector<thread> threads(THREAD_SIZE);
    vector<long long> constructs(THREAD_SIZE);
    vector<long long> local_searches(THREAD_SIZE);
    vector<int> befs(THREAD_SIZE),afts(THREAD_SIZE);
    vector<vector<vector<int>>> thread_orders(THREAD_SIZE);
    
    //スレッドに求解させる
    for(int i=0; i<THREAD_SIZE; i++){
        threads[i]=thread(ThreadProcess,ref(dis_mat),ref(weights),capacity,truck_size,
                        ref(constructs[i]),ref(local_searches[i]),ref(befs[i]),ref(afts[i]),
                        ref(thread_orders[i]),ref(nn_list),ref(Lattice),method_type,
                        THREAD_SIZE,ITERATED_SIZE,CONSTRUCT_LIMIT_MS);
    }
    //すべてのスレッドの処理が終わるのを待つ
    for(int i=0; i<THREAD_SIZE; i++) threads[i].join();

    //一番改善後の総移動距離が短いスレッドの解を採用する
    int minid=0;
    int min_dist=afts[0];
    for(int i=1; i<THREAD_SIZE; i++){
        if(min_dist>afts[i]){
            minid=i;
            min_dist=afts[i];
        }
    }
    if(THREAD_SIZE!=1){
        cout<<"thread size:"<<THREAD_SIZE<<endl;
        cout<<"use thread"<<minid+1<<"'s answer\n\n";
    }
    auto orders=thread_orders[minid];

    //各スレッドの解の性質などをログ出力する
    ShowThreadsInfos(THREAD_SIZE,minid,thread_orders,constructs,local_searches,
                    befs,afts,capacity,n,weights);
    if(THREAD_SIZE!=1)
        ShowThreadsAves(THREAD_SIZE,local_searches,befs,afts,minid);

    /*==========output for python==========*/
    string data_fname="tmp/data.txt";
    string order_fname="tmp/order.txt";
    OutputData(n,cus_x,cus_y,weights,orders,data_fname,order_fname);

    string lattice_fname="tmp/lattice.txt";
    OutputLattice(Lattice,LATTICE_SIZE,lattice_fname);

    /*==========matplotlib show in python==========*/
    FILE* p = popen("~/../../mnt/c/users/ASUS-DESKTOP/anaconda3/python.exe plot.py","w");
    fprintf(p,data_fname.c_str());
    fprintf(p,"\n");
    fprintf(p,order_fname.c_str());
    fprintf(p,"\n");
    pclose(p);
    
    return 0;
}