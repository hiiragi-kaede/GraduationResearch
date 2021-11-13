#include<vector>
#include<iostream>
#include<string>
#include<fstream>
#include<stdio.h>
#include<cmath>
#include<chrono>
#include<thread>
#include"src/construct.hpp"
#include"src/util.hpp"
#include"src/improve.hpp"

/*
compile command
g++ -Wno-format-security -O3 -o main main.cpp src/construct.cpp src/util.cpp src/improve.cpp -pthread
./main < "source data file name"
*/
using namespace std;

static int CONSTRUCT_LIMIT_MS=1000;

void thread_process(const vector<vector<double>> dis_mat,const vector<int> weights,
                    const int capacity,const int truck_size,
                    long long& construct_ms,long long& local_search_sec,double& bef_dist,double& aft_dist,
                    vector<vector<int>>& ans_orders);

int main(void){
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

    /*==========prepare for calculation==========*/
    vector<vector<double>> dis_mat(n,vector<double>(n,0));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            dis_mat[i][j]=hypot(cus_x[i]-cus_x[j],cus_y[i]-cus_y[j]);
        }
    }

    vector<vector<int>> neighbor_list=construct_neighbor_list(n,dis_mat);

    vector<int> all;
    for(int i=0; i<n; i++) all.push_back(i);
    int total_weight=calc_total_weight(all,weights);
    cout<<"total_weight:"<<total_weight<<endl;
    int truck_size=ceil((double)total_weight/capacity);
    cout<<"truck_size:"<<truck_size<<endl;

    /*==========solve problem using multiple thread==========*/
    const int THREAD_SIZE=4;
    vector<thread> threads(THREAD_SIZE);
    vector<long long> constructs(THREAD_SIZE);
    vector<long long> local_searches(THREAD_SIZE);
    vector<double> befs(THREAD_SIZE),afts(THREAD_SIZE);
    vector<vector<vector<int>>> thread_orders(THREAD_SIZE);

    //スレッドに求解させる
    for(int i=0; i<THREAD_SIZE; i++){
        threads[i]=thread(thread_process,dis_mat,weights,capacity,truck_size,
                        ref(constructs[i]),ref(local_searches[i]),ref(befs[i]),ref(afts[i]),
                        ref(thread_orders[i]));
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
    cout<<"use thread"<<minid+1<<"'s answer\n\n";
    auto orders=thread_orders[minid];

    //各スレッドの解の性質などをログ出力する
    for(int i=0; i<THREAD_SIZE; i++){
        if(i==minid) cout<<"\e[31m";
        cout<<"Thread "<<i+1<<"\e[0m"<<endl;
        cout<<"time info"<<endl;
        cout<<"construct:"<<constructs[i]<<"(ms)    local search:"<<local_searches[i]<<"(s)\n";
        cout<<"total move cost change:"<<befs[i]<<"--->"<<afts[i]<<"\n";
        show_orders_info(thread_orders[i],weights,capacity,n);
        cout<<endl;
    }

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

void thread_process(const vector<vector<double>> dis_mat,const vector<int> weights,
                    const int capacity,const int truck_size,
                    long long& construct_ms,long long& local_search_sec,double& bef_dist,double& aft_dist,
                    vector<vector<int>>& ans_orders)
{
    /*==========construct initial answer==========*/
    int n=dis_mat.size();
    int t_size=truck_size;
    auto st=chrono::system_clock::now();
    vector<vector<int>> orders;
    vector<int> truck_ids(n);
    //挿入法構築で実行可能解ができるまで繰り返し生成する。
    do
    {
        auto end=chrono::system_clock::now();
        long long elapsed_ms=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        if(elapsed_ms>CONSTRUCT_LIMIT_MS) t_size++;
        
        orders=insert_construct(dis_mat,weights,capacity,t_size,truck_ids);
    } while (is_exist_unvisited(orders,weights,n));
    
    auto end=chrono::system_clock::now();
    construct_ms=chrono::duration_cast<chrono::milliseconds>(end-st).count();
    for(auto& order: orders){
        two_opt(order,dis_mat);
    }
    bef_dist=calc_total_dist(orders,dis_mat);

    /*==========local search to improve answer==========*/
    st=chrono::system_clock::now();
    cross_exchange_neighbor(weights,orders,dis_mat,capacity,truck_ids);
    end=chrono::system_clock::now();
    local_search_sec=chrono::duration_cast<chrono::seconds>(end-st).count();

    aft_dist=calc_total_dist(orders,dis_mat);
    ans_orders=orders;
}