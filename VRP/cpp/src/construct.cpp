#include"construct.hpp"
#include"util.hpp"
#include<vector>
#include<random>
#include<numeric>
#include<algorithm>
#include<iostream>
#include<utility>

using namespace std;

vector<vector<int>> InsertConstruct(vector<vector<float>> dis_mat,vector<int> weights,
                                    int truck_capacity,int truck_size,vector<pair<int,int>>& truck_ids){
    vector<vector<int>> orders(truck_size,vector<int>(2,0));
    vector<int> total_weights(truck_size,0);
    int n=weights.size();

    vector<int> idxs(n-1);
    iota(idxs.begin(),idxs.end(),1);
    //顧客の需要量順に添え字をソート
    sort(idxs.begin(),idxs.end(),[&weights](const int &a,const int &b){return weights[a]>weights[b];});

    // random_device seed_gen;
    // mt19937 engine(seed_gen());
    // //顧客順をランダムシャッフルし、他スタート局所探索法のために初期解を異なったものにさせる。
    // shuffle(idxs.begin(),idxs.end(),engine);

    //各トラックの2番めに種顧客を挿入
    for(int i=0; i<truck_size; i++){
        orders[i].insert(orders[i].begin()+1,idxs[i]);
        total_weights[i]+=weights[idxs[i]];
    }

    for(int i=truck_size; i<n-1; i++){
        int t_id=0;
        int ins_id=1;
        double min_dis=1e10;

        for(int id=0; id<truck_size; id++){
            //容量オーバーなら探索をスキップ
            if(total_weights[id]+weights[idxs[i]]>truck_capacity) continue;

            for(int j=1; j<orders[id].size(); j++){
                double cur_dis=dis_mat[orders[id][j-1]][idxs[i]]+dis_mat[idxs[i]][orders[id][j]];
                if(cur_dis<min_dis){
                    min_dis=cur_dis;
                    t_id=id;
                    ins_id=j;
                }
            }
        }

        //顧客をどこにも挿入できなかった場合はスキップ。後で訪れていない点として注意表示がされる。
        if(min_dis==10000000000) continue;

        orders[t_id].insert(orders[t_id].begin()+ins_id,idxs[i]);
        total_weights[t_id]+=weights[idxs[i]];
    }

    UpdateTruckIds(orders,truck_ids);

    return orders;
}