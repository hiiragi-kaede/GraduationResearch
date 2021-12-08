#include<vector>
#include<iostream>
#include<string>
#include<fstream>
#include<stdio.h>
#include<cmath>
#include<chrono>
#include<thread>
#include<utility>
#include<set>
#include"src/construct.hpp"
#include"src/util.hpp"
#include"src/improve.hpp"

/*
compile command
g++ -Wno-format-security -O3 -o main main.cpp src/construct.cpp src/util.cpp src/improve.cpp -pthread
./main < "source data file name"
*/
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

static const vector<string> TypeName{
    "TwoOptStar","FastTwoOptStar","ImprovedTwoOptStar",
    "Cross","FastCross","ImprovedCross",
    "IteratedTwoOptStar","IteratedCross"
};
static const int CONSTRUCT_LIMIT_MS=500;
static const int THREAD_SIZE=1;
static const int ITERATED_SIZE=10;
static MethodType method_type=MethodType::TwoOptStar;

void TrialInsertConstruct(const vector<vector<float>>& dis_mat,const vector<int>& weights,
                        const int capacity,const int truck_size,
                        vector<vector<int>>& orders,vector<pair<int,int>>& truck_ids);

void ThreadProcess(const vector<vector<float>>& dis_mat,const vector<int>& weights,
                    const int capacity,const int truck_size,
                    long long& construct_ms,long long& local_search_sec,double& bef_dist,double& aft_dist,
                    vector<vector<int>>& ans_orders,const vector<set<int>>& nn_list);

void ShowThreadsInfos(int THREAD_SIZE,int minid,const vector<vector<vector<int>>>& thread_orders,
                    const vector<long long>& constructs,const vector<long long>& local_searches,
                    const vector<double>& befs,const vector<double>& afts,int capacity,int n,
                    const vector<int>& weights);

void ShowThreadsAves(int THREAD_SIZE,const vector<long long>& local_searches,
                    const vector<double>& befs,const vector<double>& afts,int minid);

int main(int argc,char *argv[]){
    /*==========data input==========*/
    int n; cin>>n;
    int capacity; cin>>capacity;
    vector<int> cus_x,cus_y;
    int idx;
    for(int i=0; i<n; i++){
        int tmpx,tmpy;
        cin>>idx>>tmpx>>tmpy;
        cus_x.push_back(tmpx);
        cus_y.push_back(tmpy);
    }
    //"DEMAND_SECTION"の文字を取り除く
    string buff;
    cin>>buff;
    vector<int> weights;
    for(int i=0; i<n; i++){
        int tmp;
        cin>>idx>>tmp;
        weights.push_back(tmp);
    }

    if(argc>1){
        string type=string(argv[1]);
        if(type=="t")    method_type=MethodType::TwoOptStar;
        else if(type=="ft")  method_type=MethodType::FastTwoOptStar;
        else if(type=="it")  method_type=MethodType::ImprovedTwoOptStar;
        else if(type=="c")  method_type=MethodType::Cross;
        else if(type=="fc")  method_type=MethodType::FastCross;
        else if(type=="ic")  method_type=MethodType::ImprovedCross;
        else if(type=="ilst") method_type=MethodType::IteratedTwo;
        else if(type=="ilsc") method_type=MethodType::IteratedCross;
        else{
            cout<<"正しい近傍タイプを指定してください\n";
            exit(1);
        }
    }

    /*==========prepare for calculation==========*/
    vector<vector<float>> dis_mat(n,vector<float>(n,0));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            dis_mat[i][j]=hypot(cus_x[i]-cus_x[j],cus_y[i]-cus_y[j]);
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

    /*==========check difference between normal method and fast method==========*/
    // vector<vector<int>> test_orders;
    // vector<pair<int,int>> truck_ids(n);
    
    // TrialInsertConstruct(dis_mat,weights,capacity,truck_size,test_orders,truck_ids);
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
    vector<double> befs(THREAD_SIZE),afts(THREAD_SIZE);
    vector<vector<vector<int>>> thread_orders(THREAD_SIZE);
    
    //スレッドに求解させる
    for(int i=0; i<THREAD_SIZE; i++){
        threads[i]=thread(ThreadProcess,ref(dis_mat),ref(weights),capacity,truck_size,
                        ref(constructs[i]),ref(local_searches[i]),ref(befs[i]),ref(afts[i]),
                        ref(thread_orders[i]),ref(nn_list));
    }
    //すべてのスレッドの処理が終わるのを待つ
    for(int i=0; i<THREAD_SIZE; i++) threads[i].join();

    //一番改善後の総移動距離が短いスレッドの解を採用する
    int minid=0;
    double min_dist=afts[0];
    for(int i=1; i<THREAD_SIZE; i++){
        if(min_dist>afts[i]){
            minid=i;
            min_dist=afts[i];
        }
    }
    cout<<"neighbor type:"<<TypeName[static_cast<int>(method_type)]<<endl;
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
    ofstream output(data_fname);
    for(int i=0; i<n; i++){
        output<<cus_x[i]<<" "<<cus_y[i]<<" "<<weights[i]<<endl;
    }
    output.close();

    string order_fname="tmp/order.txt";
    ofstream ord_output(order_fname);
    for(auto& order: orders){
        for(int cus: order){
            ord_output<<cus<<" ";
        }
        ord_output<<endl;
    }
    ord_output.close();

    /*==========matplotlib show in python==========*/
    FILE* p = popen("~/../../mnt/c/users/ASUS-DESKTOP/anaconda3/python.exe plot.py","w");
    fprintf(p,data_fname.c_str());
    fprintf(p,"\n");
    fprintf(p,order_fname.c_str());
    fprintf(p,"\n");
    pclose(p);
    
    return 0;
}

void TrialInsertConstruct(const vector<vector<float>>& dis_mat,const vector<int>& weights,
                        const int capacity,const int truck_size,
                        vector<vector<int>>& orders,vector<pair<int,int>>& truck_ids)
{
    int n=dis_mat.size();
    int t_size=truck_size;
    auto st=chrono::system_clock::now();
    //挿入法構築で実行可能解ができるまで繰り返し生成する。
    do
    {
        auto end=chrono::system_clock::now();
        long long elapsed_ms=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        if(elapsed_ms>CONSTRUCT_LIMIT_MS){
            t_size++;
            st=chrono::system_clock::now();
        }
        
        if(THREAD_SIZE==1){
            orders=InsertConstruct(dis_mat,weights,capacity,t_size,truck_ids);
        }
        else{
            orders=InsertConstruct(dis_mat,weights,capacity,t_size,truck_ids,true);
        }
        
    } while (IsExistUnvisited(orders,weights,n));

    for(auto& order: orders){
        TwoOpt(order,dis_mat);
    }
}

void ThreadProcess(const vector<vector<float>>& dis_mat,const vector<int>& weights,
                    const int capacity,const int truck_size,
                    long long& construct_ms,long long& local_search_msec,double& bef_dist,double& aft_dist,
                    vector<vector<int>>& ans_orders,const vector<set<int>>& nn_list)
{
    /*==========construct initial answer==========*/
    int n=dis_mat.size();
    vector<vector<int>> orders;
    vector<pair<int,int>> truck_ids(n);

    auto st=chrono::system_clock::now();
    TrialInsertConstruct(dis_mat,weights,capacity,truck_size,orders,truck_ids);
    auto end=chrono::system_clock::now();
    construct_ms=chrono::duration_cast<chrono::milliseconds>(end-st).count();

    bef_dist=TotalDistance(orders,dis_mat);

    /*==========local search to improve answer==========*/
    st=chrono::system_clock::now();
    switch (method_type)
    {
    case MethodType::TwoOptStar:
        TwoOptStar(weights,orders,dis_mat,capacity);
        break;
    case MethodType::FastTwoOptStar:
        FastTwoOptStar(weights,orders,dis_mat,capacity,truck_ids,nn_list);
        break;
    case MethodType::ImprovedTwoOptStar:
        ImprovedTwoOptStar(weights,orders,dis_mat,capacity);
        break;
    case MethodType::Cross:
        CrossExchangeNeighbor(weights,orders,dis_mat,capacity);
        break;
    case MethodType::FastCross:
        FastCrossExchange(weights,orders,dis_mat,capacity,truck_ids,nn_list);
        break;
    case MethodType::ImprovedCross:
        ImprovedCrossExchangeNeighbor(weights,orders,dis_mat,capacity);
        break;
    case MethodType::IteratedTwo:
        if(THREAD_SIZE!=1)
            IteratedTwoOptStar(weights,orders,dis_mat,capacity,ITERATED_SIZE,false);
        else
            IteratedTwoOptStar(weights,orders,dis_mat,capacity,ITERATED_SIZE);
        break;
    case MethodType::IteratedCross:
        if(THREAD_SIZE!=1)
            IteratedCross(weights,orders,dis_mat,capacity,ITERATED_SIZE,false);
        else
            IteratedCross(weights,orders,dis_mat,capacity,ITERATED_SIZE);
        break;
    default:
        break;
    }
    end=chrono::system_clock::now();
    local_search_msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();

    aft_dist=TotalDistance(orders,dis_mat);
    ans_orders=orders;
}

void ShowThreadsInfos(int THREAD_SIZE,int minid,const vector<vector<vector<int>>>& thread_orders,
                    const vector<long long>& constructs,const vector<long long>& local_searches,
                    const vector<double>& befs,const vector<double>& afts,int capacity,int n,
                    const vector<int>& weights)
{
    for(int i=0; i<THREAD_SIZE; i++){
        if(i==minid) cout<<"\e[31m";
        cout<<"Thread "<<i+1<<"\e[0m"<<endl;
        cout<<"truck size:"<<thread_orders[i].size()<<endl;
        cout<<"time info"<<endl;
        cout<<"construct:"<<constructs[i]<<"(ms)    local search:"<<local_searches[i]<<"(ms)\n";
        cout<<"total move cost change:"<<befs[i]<<"--->"<<afts[i]<<"\n";
        //ShowOrdersInfo(thread_orders[i],weights,capacity,n);
        cout<<"improve rate:"<<100-(afts[i]/befs[i]*100)<<endl;
        cout<<endl;
    }
}

void ShowThreadsAves(int THREAD_SIZE,const vector<long long>& local_searches,
                    const vector<double>& befs,const vector<double>& afts,int minid)
{
    double bef_ave=0,aft_ave=0,imp_rate_ave=0,ls_ave=0;
    for(int i=0; i<THREAD_SIZE; i++){
        ls_ave+=(double)local_searches[i]/THREAD_SIZE;
        bef_ave+=befs[i]/THREAD_SIZE;
        aft_ave+=afts[i]/THREAD_SIZE;
        imp_rate_ave+=(100-afts[i]/befs[i]*100)/THREAD_SIZE;
    }
    cout<<"construct average score:"<<bef_ave<<endl;
    cout<<"improved average score:"<<aft_ave<<endl;
    cout<<"local search average milliseconds:"<<ls_ave<<endl;
    cout<<"improve rate average:"<<imp_rate_ave<<endl;
    cout<<"minimum:"<<afts[minid]<<endl;
}