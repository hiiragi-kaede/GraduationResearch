#include"improve.hpp"
#include"util.hpp"
#include<vector>
#include<chrono>
#include<algorithm>
#include<utility>
#include<set>
#include<iostream>
#include<queue>

using namespace std;

static const int limit_time=300;
static const int tabu_iterate_size=3;

void TwoOpt(vector<int>& order,const vector<vector<float>> dis_mat){
    int n=order.size();
    
    while(true){
        bool is_changed=false;
        for(int i=0; i<n-2; i++){
            int i_next=i+1;
            for(int j=i+2; j<n-1; j++){
                int j_next=(j+1)%n;

                double l1 = dis_mat[order[i]][order[i_next]];
                double l2 = dis_mat[order[j]][order[j_next]];
                double l3 = dis_mat[order[i]][order[j]];
                double l4 = dis_mat[order[i_next]][order[j_next]];

                if(l1+l2 > l3+l4){
                    vector<int> new_root;
                    for(int st=i_next; st<=j; st++) new_root.push_back(order[st]);
                    std::reverse(new_root.begin(),new_root.end());
                    for(int st=i_next; st<=j; st++) order[st]=new_root[st-i_next];
                    is_changed=true;
                }
            }
        }

        if(!is_changed) break;
    }
}

double GetCrossExDiff(const vector<vector<float>> dis_mat,const vector<vector<int>> orders,
                    int i,int j,int i_st,int i_end,int j_st,int j_end)
{
    double dif=-dis_mat[orders[i][i_st-1]][orders[i][i_st]]
                                    -dis_mat[orders[i][i_end-1]][orders[i][i_end]]
                                    -dis_mat[orders[j][j_st-1]][orders[j][j_st]]
                                    -dis_mat[orders[j][j_end-1]][orders[j][j_end]]
                                    +dis_mat[orders[i][i_st-1]][orders[j][j_st]]
                                    +dis_mat[orders[i][i_end-1]][orders[j][j_end]]
                                    +dis_mat[orders[j][j_st-1]][orders[i][i_st]]
                                    +dis_mat[orders[j][j_end-1]][orders[i][i_end]];
    return dif;
}

void UpdateCrossOrders(vector<vector<int>>& orders,vector<vector<float>> dis_mat,int i,int j,
                        int i_st,int i_end,int j_st,int j_end,int fst_size,int sec_size)
{
    int i_dif=i_end-i_st,j_dif=j_end-j_st;
    vector<int> new_i(fst_size-i_dif+j_dif),new_j(sec_size-j_dif+i_dif);

    copy(orders[i].begin(),orders[i].begin()+i_st,new_i.begin());
    copy(orders[j].begin(),orders[j].begin()+j_st,new_j.begin());

    copy(orders[j].begin()+j_st,orders[j].begin()+j_end,new_i.begin()+i_st);
    copy(orders[i].begin()+i_st,orders[i].begin()+i_end,new_j.begin()+j_st);
    
    copy(orders[i].begin()+i_end,orders[i].end(),new_i.begin()+i_st+j_dif);
    copy(orders[j].begin()+j_end,orders[j].end(),new_j.begin()+j_st+i_dif);

    orders[i]=new_i;
    orders[j]=new_j;

    TwoOpt(orders[i],dis_mat);
    TwoOpt(orders[j],dis_mat);
}

void CrossExchangeNeighbor(const vector<int> weights,vector<vector<int>>& orders,
                            const vector<vector<float>> dis_mat,const int truck_capacity,
                            vector<pair<int,int>>& truck_ids)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    auto st=chrono::system_clock::now();

    while(1){
        auto end=chrono::system_clock::now();
        auto sec=chrono::duration_cast<chrono::seconds>(end-st).count();
        if(sec>limit_time) break;

        bool is_changed=false;
        for(int i=0; i<truck_size-1; i++){
            if(is_changed) break;
            for(int j=i+1; j<truck_size; j++){
                is_changed=SubCross(weights,orders,dis_mat,truck_capacity,i,j,truck_ids);
                if(is_changed) break;
            }
        }

        if(!is_changed) break;
    }
}

bool SubCross(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<float>> dis_mat,const int truck_capacity,
                const int i,const int j,vector<pair<int,int>>& truck_ids)
{
    int fst_size=orders[i].size();
    int sec_size=orders[j].size();
    for(int i_st=1; i_st<fst_size-1; i_st++){
        for(int i_end=i_st+1; i_end<fst_size; i_end++){
            for(int j_st=1; j_st<sec_size-1; j_st++){
                for(int j_end=j_st+1; j_end<sec_size; j_end++){
                    vector<int> fst_ord(i_end-i_st),sec_ord(j_end-j_st);
                    copy(orders[i].begin()+i_st,orders[i].begin()+i_end,fst_ord.begin());
                    copy(orders[j].begin()+j_st,orders[j].begin()+j_end,sec_ord.begin());

                    int fst_weight=TotalWeight(fst_ord,weights);
                    int sec_weight=TotalWeight(sec_ord,weights);

                    if(IsValidWeight(orders[i],orders[j],weights,
                        fst_weight,sec_weight,truck_capacity))
                    {
                        double dif=GetCrossExDiff(dis_mat,orders,i,j,i_st,i_end,j_st,j_end);
                        if(dif<0 && abs(dif)>0.0005){
                            UpdateCrossOrders(orders,dis_mat,i,j,i_st,i_end,j_st,j_end,fst_size,sec_size);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void FastCrossExchange(const vector<int> weights,vector<vector<int>>& orders,
                        const vector<vector<float>> dis_mat,const int truck_capacity,
                        vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    auto st=chrono::system_clock::now();
    while(1){
        bool is_changed=false;
        loop_st:
        for(int i=0; i<truck_size; i++){
            for(int ord=1,n=orders[i].size(); ord<n; ord++){
                auto end=chrono::system_clock::now();
                auto sec=chrono::duration_cast<chrono::seconds>(end-st).count();
                if(sec>limit_time) return;

                int cus=orders[i][ord];
                is_changed=SubFastCross(weights,orders,dis_mat,truck_capacity,
                                                truck_ids,nn_list,i,cus);
                if(is_changed) break;
            }
        }
        if(!is_changed) return;
    }
}

bool SubFastCross(const vector<int> weights,vector<vector<int>>& orders,
                    const vector<vector<float>> dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list,
                    int i,int cus)
{
    for(int neighbor : nn_list[cus]){
        int j=truck_ids[neighbor].first;
        //所属するトラックが一緒ならスキップ
        if(i==j) continue;

        int i_st=truck_ids[cus].second,j_st=truck_ids[neighbor].second;
        int fst_size=orders[i].size(),sec_size=orders[j].size();

        int i_dif=fst_size-i_st,j_dif=sec_size-j_st;
        if(i_dif<2 || j_dif<2) return false;

        for(int i_end=i_st+1; i_end<fst_size-1; i_end++){
            for(int j_end=j_st+1; j_end<sec_size-1; j_end++){
                vector<int> fst_ord(i_end-i_st),sec_ord(j_end-j_st);
                copy(orders[i].begin()+i_st,orders[i].begin()+i_end,fst_ord.begin());
                copy(orders[j].begin()+j_st,orders[j].begin()+j_end,sec_ord.begin());
                int fst_weights=TotalWeight(fst_ord,weights);
                int sec_weights=TotalWeight(sec_ord,weights);

                if(IsValidWeight(orders[i],orders[j],weights,fst_weights,sec_weights,truck_capacity))
                {
                    double dif=GetCrossExDiff(dis_mat,orders,i,j,i_st,i_end,j_st,j_end);
                    if(dif<0){
                        UpdateCrossOrders(orders,dis_mat,i,j,i_st,i_end,j_st,j_end,fst_size,sec_size);

                        UpdateTruckIds(orders[i],i,truck_ids);
                        UpdateTruckIds(orders[j],j,truck_ids);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void TwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<float>> dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    auto c=comb(truck_size,2);
    auto st=chrono::system_clock::now();

    while(1){
        auto end=chrono::system_clock::now();
        auto sec=chrono::duration_cast<chrono::seconds>(end-st).count();
        if(sec>limit_time) break;

        bool is_changed=false;
        for(const auto& ids : c){
            int i=ids[0]-1,j=ids[1]-1;
            is_changed=SubTwoOptStar(weights,orders,dis_mat,truck_capacity,i,j,truck_ids);
            if(is_changed) break;
        }

        if(!is_changed) break;
    }
}

bool SubTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                    const vector<vector<float>> dis_mat,const int truck_capacity,
                    const int i,const int j,vector<pair<int,int>>& truck_ids)
{
    int fst_size=orders[i].size();
    int sec_size=orders[j].size();
    for(int i_id=fst_size-2; i_id>=1; i_id--){
        for(int j_id=sec_size-2; j_id>=1; j_id--){
            int i_dif=fst_size-i_id,j_dif=sec_size-j_id;
            vector<int> fst_ord(i_dif),sec_ord(j_dif);
            copy(orders[i].begin()+i_id,orders[i].end(),fst_ord.begin());
            copy(orders[j].begin()+j_id,orders[j].end(),sec_ord.begin());
            int fst_weights=TotalWeight(fst_ord,weights);
            int sec_weights=TotalWeight(sec_ord,weights);

            if(IsValidWeight(orders[i],orders[j],weights,fst_weights,sec_weights,truck_capacity)){
                double dif=-dis_mat[orders[i][i_id-1]][orders[i][i_id]]
                            -dis_mat[orders[j][j_id-1]][orders[j][j_id]]
                            +dis_mat[orders[i][i_id-1]][orders[j][j_id]]
                            +dis_mat[orders[j][j_id-1]][orders[i][i_id]];
                
                if(dif<0 && abs(dif)>0.0001){
                    vector<int> new_fst(fst_size-i_dif+j_dif),new_sec(sec_size-j_dif+i_dif);
                    copy(orders[i].begin(),orders[i].begin()+i_id,new_fst.begin());
                    copy(orders[j].begin(),orders[j].begin()+j_id,new_sec.begin());
                    copy(orders[j].begin()+j_id,orders[j].end(),new_fst.begin()+i_id);
                    copy(orders[i].begin()+i_id,orders[i].end(),new_sec.begin()+j_id);

                    orders[i]=new_fst;
                    orders[j]=new_sec;

                    TwoOpt(orders[i],dis_mat);
                    TwoOpt(orders[j],dis_mat);
                    return true;
                }
            }
        }
    }
    return false;
}

void FastTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<float>> dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    auto st=chrono::system_clock::now();
    while(1){
        auto end=chrono::system_clock::now();
        auto sec=chrono::duration_cast<chrono::seconds>(end-st).count();
        if(sec>limit_time) break;

        bool is_changed=SubFastTwoOptStar(weights,orders,dis_mat,truck_capacity,
                                        truck_ids,nn_list);
        if(!is_changed) break;
    }
}

bool SubFastTwoOptStar(const vector<int> weights,vector<vector<int>>& orders,
                    const vector<vector<float>> dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,vector<set<int>> nn_list)
{
    int n=orders.size();
    for(int i=0; i<n; i++){
        for(int cus : orders[i]){
            for(int cus_j : nn_list[cus]){
                auto j_locate=truck_ids[cus_j];
                int j=j_locate.first;
                //所属するトラックが一緒ならスキップ
                if(i==j) continue;

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
                    double dif=-dis_mat[orders[i][fst_id-1]][orders[i][fst_id]]
                                -dis_mat[orders[j][sec_id-1]][orders[j][sec_id]]
                                +dis_mat[orders[i][fst_id-1]][orders[j][sec_id]]
                                +dis_mat[orders[j][sec_id-1]][orders[i][fst_id]];
                    if(dif<0 && abs(dif)>0.0001){
                        vector<int> new_fst(fst_size-i_dif+j_dif),new_sec(sec_size-j_dif+i_dif);
                        copy(orders[i].begin(),orders[i].begin()+fst_id,new_fst.begin());
                        copy(orders[j].begin(),orders[j].begin()+sec_id,new_sec.begin());
                        copy(orders[j].begin()+sec_id,orders[j].end(),new_fst.begin()+fst_id);
                        copy(orders[i].begin()+fst_id,orders[i].end(),new_sec.begin()+sec_id);

                        orders[i]=new_fst;
                        orders[j]=new_sec;

                        TwoOpt(orders[i],dis_mat);
                        TwoOpt(orders[j],dis_mat);

                        UpdateTruckIds(orders[i],i,truck_ids);
                        UpdateTruckIds(orders[j],j,truck_ids);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool IsValidWeight(const vector<int> order_i,const vector<int> order_j,
                const vector<int> weights,const int fst_weight,const int sec_weight,
                const int truck_capacity)
{
    bool ret=(TotalWeight(order_i,weights)-fst_weight+sec_weight<truck_capacity)
            &&(TotalWeight(order_j,weights)-sec_weight+fst_weight<truck_capacity);
    return ret;
}