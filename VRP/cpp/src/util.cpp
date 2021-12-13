#include<vector>
#include<algorithm>
#include<iostream>
#include<numeric>
#include<utility>
#include<set>
#include<cmath>
#include"util.hpp"
#include"improve.hpp"

using namespace std;

int TotalWeight(const vector<int>& order,const vector<int>& weights){
    int total=0;
    for(int i=0; i<order.size(); i++){
        total+=weights[order[i]];
    }
    return total;
}

int TotalWeight(vector<int>::iterator first,vector<int>::iterator last,const vector<int>& weights)
{
    int total=0;
    for(auto itr=first; itr!=last; itr++){
        total+=weights[*itr];
    }
    return total;
}

double TotalDistance(const vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat)
{
    double total=0;
    for(const auto& order : orders){
        total+=TotalDistance(order,dis_mat);
    }
    return total;
}

double TotalDistance(const vector<int>& order,
                    const vector<vector<float>>& dis_mat)
{
    double total=0;
    for(auto it=order.begin(); (it+1)!=order.end(); it++){
        total+=dis_mat[*it][*(it+1)];
    }
    return total;
}

void ShowUnvisited(const vector<vector<int>>& orders,const vector<int>& weights,int n){
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

bool IsExistUnvisited(const vector<vector<int>>& orders,const vector<int>& weights,int n){
    vector<bool> visited(n,false);
    for(auto& order: orders){
        for(int id : order) visited[id]=true;
    }
    //訪れていない顧客が存在する
    for(bool check : visited) if(!check) return true;
    return false;
}

void ShowOrdersInfo(const vector<vector<int>>& orders,const vector<int>& weights,
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

vector<vector<int>> ConstructNeighborList(const int n,const vector<vector<float>>& dis_mat){
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

vector<set<int>> ConstructNNList(const vector<vector<float>>& dis_mat,
                                const vector<vector<int>>& neighbor_list)
{
    int n=dis_mat.size();
    int part_size=n*0.1;
    vector<set<int>> nn_list(n);
    for(int i=0; i<n; i++){
        //自分自身との距離が0になり、それが一番近いため、それを飛ばして構築する。
        for(int j=1; j<=part_size; j++){
            nn_list[i].insert(neighbor_list[i][j]);
        }
    }

    return nn_list;
}

void UpdateTruckIds(const vector<vector<int>>& orders,vector<pair<int,int>>& truck_ids){
    for(int i=0; i<orders.size(); i++){
        for(int j=0; j<orders[i].size(); j++){
            int id=orders[i][j];
            truck_ids[id]=make_pair(i,j);
        }
    }
}

void UpdateTruckIds(const vector<int>& order,const int order_num,vector<pair<int,int>>& truck_ids){
    int i=order_num;
    for(int j=0; j<order.size(); j++){
        int id=order[j];
        truck_ids[id]=make_pair(i,j);
    }
}

void ShowTruckIds(const vector<pair<int,int>>& truck_ids){
    for(int i=0; i<truck_ids.size(); i++){
        cout<<i<<":"<<truck_ids[i].first<<"の"<<truck_ids[i].second<<"番目"<<endl;
    }
}

void util::ShowTwoOptStarDiffs(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    for(int i=0; i<truck_size-1; i++){
        for(int j=i+1; j<truck_size; j++){
            util::SubTwoOptStar(weights,orders,dis_mat,truck_capacity,i,j,truck_ids);
        }
    }
}

void util::SubTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                const int i,const int j,vector<pair<int,int>>& truck_ids)
{
    auto construct=orders;

    int fst_size=orders[i].size();
    int sec_size=orders[j].size();
    for(int i_id=fst_size-2; i_id>=1; i_id--){
        for(int j_id=sec_size-2; j_id>=1; j_id--){
            orders=construct;
            double bef_cost,aft_cost;

            int i_dif=fst_size-i_id,j_dif=sec_size-j_id;
            vector<int> fst_ord(i_dif),sec_ord(j_dif);
            copy(orders[i].begin()+i_id,orders[i].end(),fst_ord.begin());
            copy(orders[j].begin()+j_id,orders[j].end(),sec_ord.begin());
            int fst_weights=TotalWeight(fst_ord,weights);
            int sec_weights=TotalWeight(sec_ord,weights);

            if(IsValidWeight(orders[i],orders[j],weights,fst_weights,sec_weights,truck_capacity)){
                double dif=GetTwoOptStarDiff(dis_mat,orders,i,j,i_id,j_id);
                
                if(dif<0 && abs(dif)>0.0001){
                    bef_cost=TotalDistance(orders,dis_mat);
                    UpdateTwoOptStarOrders(orders,dis_mat,i,j,fst_size,sec_size,
                                        i_dif,j_dif,i_id,j_id);
                    aft_cost=TotalDistance(orders,dis_mat);

                    double change_cost_dif=bef_cost-aft_cost;
                    ShowDifInfos(i,j,change_cost_dif,construct,orders,true);
                }
            }
        }
    }
}

void util::ShowDifInfos(int i,int j,double change_cost_dif,
                        const vector<vector<int>>& construct,
                        const vector<vector<int>>& orders,
                        bool isSimpled)
{
    if(isSimpled){
        cout<<i<<" "<<j<<endl;
        cout<<change_cost_dif<<endl;
    }
    else{
        cout<<"change truck"<<i<<" and truck"<<j<<endl;
        cout<<"total move cost change:"<<change_cost_dif<<endl;
        cout<<"truck "<<i<<"'s root:\nbefore:";
        for_each(construct[i].begin(),construct[i].end(),[](int x){
            cout<<x<<",";
        });
        cout<<"\nafter:";
        for_each(orders[i].begin(),orders[i].end(),[](int x){
            cout<<x<<",";
        });
        cout<<endl;
        cout<<"truck "<<j<<"'s root:\nbefore:";
        for_each(construct[j].begin(),construct[j].end(),[](int x){
            cout<<x<<",";
        });
        cout<<"\nafter:";
        for_each(orders[j].begin(),orders[j].end(),[](int x){
            cout<<x<<",";
        });
        cout<<"\n\n";
    }
}

void util::ShowFastTwoOptStarDiffs(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);

    util::SubFastTwoOptStar(weights,orders,dis_mat,truck_capacity,
                            truck_ids,nn_list);
}

void util::SubFastTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list)
{
    int n=orders.size();
    auto construct=orders;

    for(int i=0; i<n; i++){
        for(int cus : construct[i]){
            for(int cus_j : nn_list[cus]){
                auto j_locate=truck_ids[cus_j];
                int j=j_locate.first;
                //所属するトラックが一緒ならスキップ
                if(i==j) continue;

                orders=construct;
                double bef_cost,aft_cost;

                int fst_id=truck_ids[cus].second,sec_id=j_locate.second;
                int fst_size=orders[i].size(),sec_size=orders[j].size();

                int i_dif=fst_size-fst_id,j_dif=sec_size-sec_id;
                if(i_dif<2 || j_dif<2) continue;

                vector<int> fst_ord(i_dif),sec_ord(j_dif);
                copy(orders[i].begin()+fst_id,orders[i].end(),fst_ord.begin());
                copy(orders[j].begin()+sec_id,orders[j].end(),sec_ord.begin());
                int fst_weights=TotalWeight(fst_ord,weights);
                int sec_weights=TotalWeight(sec_ord,weights);

                if(IsValidWeight(orders[i],orders[j],weights,fst_weights,sec_weights,truck_capacity)){
                    double dif=GetTwoOptStarDiff(dis_mat,orders,i,j,fst_id,sec_id);
                    
                    if(dif<0 && abs(dif)>0.0001){
                        bef_cost=TotalDistance(orders,dis_mat);
                        UpdateTwoOptStarOrders(orders,dis_mat,i,j,fst_size,sec_size,
                                                i_dif,j_dif,fst_id,sec_id);
                        aft_cost=TotalDistance(orders,dis_mat);

                        double change_cost_dif=bef_cost-aft_cost;
                        ShowDifInfos(i,j,change_cost_dif,construct,orders,true);
                    }
                }
            }
        }
    }
}

vector<int> ConstructContainingLatticeList(const vector<int>& cus_x,const vector<int>& cus_y,
                                                int LatticeSize)
{
    int n=cus_x.size();
    int min_x=*min_element(cus_x.begin(),cus_x.end());
    int min_y=*min_element(cus_y.begin(),cus_y.end());
    int max_x=*max_element(cus_x.begin(),cus_x.end());
    int max_y=*max_element(cus_y.begin(),cus_y.end());

    vector<int> ret(n);    
    for(int i=0; i<n; i++){
        double tmp_x=(cus_x[i]-min_x)/(double)(max_x-min_x)*LatticeSize;
        double tmp_y=(cus_y[i]-min_y)/(double)(max_y-min_y)*LatticeSize;

        int x=floor(tmp_x),y=floor(tmp_y);
        ret[i]=x+LatticeSize*y;
    }

    return ret;
}