#include<vector>
#include<algorithm>
#include<iostream>
#include<numeric>
#include<utility>
#include<set>
#include"util.hpp"

using namespace std;

int TotalWeight(vector<int> order,vector<int> weights){
    int total=0;
    for(int i=0; i<order.size(); i++){
        total+=weights[order[i]];
    }
    return total;
}

double TotalDistance(const vector<vector<int>> orders,
                    const vector<vector<double>> dis_mat)
{
    double total=0;
    for(auto& order : orders){
        for(auto it=order.begin(); (it+1)!=order.end(); it++){
            total+=dis_mat[*it][*(it+1)];
        }
    }
    return total;
}

void ShowUnvisited(const vector<vector<int>>orders,const vector<int> weights,int n){
    vector<bool> visited(n,false);
    bool out=false;
    cout<<"\e[31munvisited:";
    for(auto& order: orders){
        for(int id: order) visited[id]=true;
    }
    for(int i=0; i<n; i++){
        if(!visited[i]){
            if(out) cout<<"          "<<i<<",weight:"<<weights[i]<<endl;
            else cout<<i<<",weight:"<<weights[i]<<endl;
            out=true;
        }
    }
    if(!out) cout<<"\033[1K";
    cout<<"\e[0m";
}

bool IsExistUnvisited(const vector<vector<int>>orders,const vector<int> weights,int n){
    vector<bool> visited(n,false);
    for(auto& order: orders){
        for(int id : order) visited[id]=true;
    }
    //訪れていない顧客が存在する
    for(bool check : visited) if(!check) return true;
    return false;
}

void ShowOrdersInfo(const vector<vector<int>> orders,const vector<int> weights,
                const int capacity,const int n){
    for(auto& order: orders){
        //容量オーバーは合計容量を赤く塗る
        int total_weight=TotalWeight(order,weights);
        if(total_weight>capacity) cout<<"\e[31m";
        cout<<total_weight<<"\e[0m";

        cout<<"/capacity:"<<capacity<<"    route:";

        for(int id: order) cout<<id<<",";
        
        cout<<endl;
    }
    ShowUnvisited(orders,weights,n);
}

//https://scrapbox.io/rustacean/combination%E3%81%AE%E5%AE%9F%E8%A3%85
vector<vector<int>> comb(int n, int r) {
    vector<bool> b(n);
    fill(b.end() - r, b.end(), true);

    vector<vector<int>> combs = {};
    do {
        vector<int> c = {};
        for (int i=0; i<n; i++) if (b[i]) c.push_back(i+1);
        combs.push_back(c);
    } while (next_permutation(b.begin(), b.end()));
    return combs;
}

vector<vector<int>> ConstructNeighborList(const int n,const vector<vector<double>> dis_mat){
    vector<vector<int>> neighbor_list(n,vector<int>(n));
    for(int i=0; i<n; i++){
        iota(neighbor_list[i].begin(),neighbor_list[i].end(),0);
        sort(neighbor_list[i].begin(),neighbor_list[i].end(),
        [&i,&dis_mat](const int &a,const int &b)
        {
            return dis_mat[i][a]<dis_mat[i][b];
        });
    }

    return neighbor_list;
}

vector<set<int>> ConstructNNList(const vector<vector<double>> dis_mat,
                                        const vector<vector<int>> neighbor_list)
{
    int n=dis_mat.size();
    int part_size=n/10;
    vector<set<int>> nn_list(n);
    for(int i=0; i<n; i++){
        //自分自身との距離が0になり、それが一番近いため、それを飛ばして構築する。
        for(int j=1; j<=part_size; j++){
            nn_list[i].insert(neighbor_list[i][j]);
        }
    }

    return nn_list;
}

void UpdateTruckIds(const vector<vector<int>> orders,vector<pair<int,int>>& truck_ids){
    for(int i=0; i<orders.size(); i++){
        for(int j=0; j<orders[i].size(); j++){
            int id=orders[i][j];
            truck_ids[id]=make_pair(i,j);
        }
    }
}

void UpdateTruckIds(const vector<int> order,const int order_num,vector<pair<int,int>>& truck_ids){
    int i=order_num;
    for(int j=0; j<order.size(); j++){
        int id=order[j];
        truck_ids[id]=make_pair(i,j);
    }
}

void ShowTruckIds(const vector<pair<int,int>> truck_ids){
    for(int i=0; i<truck_ids.size(); i++){
        cout<<i<<":"<<truck_ids[i].first<<"の"<<truck_ids[i].second<<"番目"<<endl;
    }
}