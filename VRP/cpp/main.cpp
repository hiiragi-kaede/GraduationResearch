#include<vector>
#include<iostream>
#include<string>
#include<fstream>
#include<stdio.h>
#include<cmath>
#include"src/construct.hpp"
#include"src/util.hpp"
#include<chrono>

/*
compile command
g++ -O3 -o main main.cpp src/construct.cpp src/util.cpp
./main < "source data file name"
*/
using namespace std;

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

    vector<int> all;
    for(int i=0; i<n; i++) all.push_back(i);
    int total_weight=calc_total_weight(all,weights);
    //cout<<"total_weight:"<<total_weight<<endl;
    int truck_size=ceil((double)total_weight/capacity);
    //cout<<"truck_size:"<<truck_size<<endl;

    /*==========construct initial answer==========*/
    auto st=chrono::system_clock::now();
    auto orders=insert_construct(dis_mat,weights,capacity,truck_size);
    auto end=chrono::system_clock::now();
    auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
    cout<<"construct elapsed time:"<<msec<<"ms"<<endl;
    for(auto& order: orders){
        two_opt(order,dis_mat);
    }
    cout<<"total move cost:"<<calc_total_dist(orders,dis_mat)<<endl;

    st=chrono::system_clock::now();
    cross_exchange_neighbor(weights,orders,dis_mat,capacity);
    end=chrono::system_clock::now();
    auto sec=chrono::duration_cast<chrono::seconds>(end-st).count();
    cout<<"exchange elapsed time:"<<sec<<"s"<<endl;

    cout<<"cross exchanged:"<<calc_total_dist(orders,dis_mat)<<endl;

    /*==========output for python==========*/
    string data_fname="tmp/data.txt";
    ofstream output(data_fname);
    for(int i=0; i<n; i++){
        output<<cus_x[i]<<" "<<cus_y[i]<<" "<<weights[i]<<endl;
    }
    output.close();

    string order_fname="tmp/order.txt";
    ofstream ord_output(order_fname);
    for(auto order: orders){
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