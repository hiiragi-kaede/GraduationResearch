#include"improve.hpp"
#include"util.hpp"
#include<vector>
#include<chrono>
#include<algorithm>
#include<utility>
#include<set>
#include<iostream>
#include<queue>
#include<random>

using namespace std;

static const int limit_time_millisec=50*1000;
static random_device seed_gen;
static mt19937 engine(seed_gen());

void TwoOpt(vector<int>& order,const vector<vector<float>>& dis_mat){
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

void DoubleBridge(vector<vector<int>>& orders){
    for(auto& order : orders){
        int size=order.size();
        if(size<6) continue;
        uniform_int_distribution<> fst_dist(1,size-5);
        int first=fst_dist(engine);
        uniform_int_distribution<> sec_dist(first+2,size-3);
        int second=sec_dist(engine);

        int tmp[]={order[first],order[first+1]};
        order[first]=order[second];
        order[first+1]=order[second+1];
        order[second]=tmp[0];
        order[second+1]=tmp[1];
    }
}

bool FourOptStar(vector<vector<int>>& orders,const vector<int>& weights,int truck_capacity){
    int truck_size=orders.size();
    for(int fst=0; fst<truck_size-3; fst++){
        for(int sec=fst+1; sec<truck_size-2; sec++){
            for(int third=sec+1; third<truck_size-1; third++){
                for(int fourth=third+1; fourth<truck_size; fourth++){
                    bool isOk=SubFourOptStar(orders,weights,fst,sec,third,fourth,truck_capacity);
                    if(isOk) return true;
                }
            }
        }
    }
    return false;
}

bool SubFourOptStar(vector<vector<int>>& orders,const vector<int>& weights,
                    int fst,int sec,int third,int fourth,int truck_capacity)
{
    uniform_int_distribution<> dist(1+orders[fst].size()*0.4,orders[fst].size()-2);
    int fst_id=dist(engine);
    int fst_weight=TotalWeight(orders[fst].begin()+fst_id,orders[fst].end(),weights);
    int fst_total=TotalWeight(orders[fst].begin(),orders[fst].end(),weights);
    int sec_total=TotalWeight(orders[sec].begin(),orders[sec].end(),weights);
    int third_total=TotalWeight(orders[third].begin(),orders[third].end(),weights);
    int fourth_total=TotalWeight(orders[fourth].begin(),orders[fourth].end(),weights);
    
    int sec_weight=0;
    int sec_id=orders[sec].size()-1;
    for(; sec_id>0; sec_id--){
        if(sec_weight+weights[orders[sec][sec_id]]<=truck_capacity+fst_weight-fst_total){
            sec_weight+=weights[orders[sec][sec_id]];
        }
        else break;
    }
    sec_id++;
    if(sec_total-sec_weight+fst_weight>truck_capacity) return false;
    
    int third_weight=0;
    int third_id=orders[third].size()-1;
    for(; third_id>0; third_id--){
        if(third_weight+weights[orders[third][third_id]]<=truck_capacity+sec_weight-sec_total){
            third_weight+=weights[orders[third][third_id]];
        }
        else break;
    }
    third_id++;
    if(third_total-third_weight+sec_weight>truck_capacity) return false;

    int fourth_weight=0;
    int fourth_id=orders[fourth].size()-1;
    for(; fourth_id>0; fourth_id--){
        if(fourth_weight+weights[orders[fourth][fourth_id]]<=truck_capacity+third_weight-third_total){
            fourth_weight+=weights[orders[fourth][fourth_id]];
        }
        else break;
    }
    fourth_id++;
    if(fourth_total-fourth_weight+third_weight>truck_capacity) return false;

    if(fst_total-fst_weight+fourth_weight>truck_capacity) return false;

    UpdateFourOpt(orders,fst,sec,third,fourth,fst_id,sec_id,third_id,fourth_id);
    return true;
}

void UpdateFourOpt(vector<vector<int>>& orders,int fst,int sec,
                int third,int fourth,int fst_id,int sec_id,
                int third_id,int fourth_id)
{
    vector<int> tmp_fst(fst_id+orders[fourth].size()-fourth_id);
    vector<int> tmp_sec(sec_id+orders[fst].size()-fst_id);
    vector<int> tmp_third(third_id+orders[sec].size()-sec_id);
    vector<int> tmp_fourth(fourth_id+orders[third].size()-third_id);

    copy(orders[fst].begin(),orders[fst].begin()+fst_id,tmp_fst.begin());
    copy(orders[sec].begin(),orders[sec].begin()+sec_id,tmp_sec.begin());
    copy(orders[third].begin(),orders[third].begin()+third_id,tmp_third.begin());
    copy(orders[fourth].begin(),orders[fourth].begin()+fourth_id,tmp_fourth.begin());

    copy(orders[fourth].begin()+fourth_id,orders[fourth].end(),tmp_fst.begin()+fst_id);
    copy(orders[fst].begin()+fst_id,orders[fst].end(),tmp_sec.begin()+sec_id);
    copy(orders[sec].begin()+sec_id,orders[sec].end(),tmp_third.begin()+third_id);
    copy(orders[third].begin()+third_id,orders[third].end(),tmp_fourth.begin()+fourth_id);

    orders[fst]=tmp_fst;
    orders[sec]=tmp_sec;
    orders[third]=tmp_third;
    orders[fourth]=tmp_fourth;
}

double GetCrossExDiff(const vector<vector<float>>& dis_mat,const vector<vector<int>>& orders,
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

double GetTwoOptStarDiff(const vector<vector<float>>& dis_mat,const vector<vector<int>>& orders,
                        int i,int j,int i_id,int j_id)
{
    double dif=-dis_mat[orders[i][i_id-1]][orders[i][i_id]]
                -dis_mat[orders[j][j_id-1]][orders[j][j_id]]
                +dis_mat[orders[i][i_id-1]][orders[j][j_id]]
                +dis_mat[orders[j][j_id-1]][orders[i][i_id]];
    return dif;
}

void UpdateCrossOrders(vector<vector<int>>& orders,const vector<vector<float>>& dis_mat,int i,int j,
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

void UpdateTwoOptStarOrders(vector<vector<int>>& orders,const vector<vector<float>>& dis_mat,
                            int i,int j,int fst_size,int sec_size,int i_dif,int j_dif,
                            int i_id,int j_id)
{
    vector<int> new_fst(fst_size-i_dif+j_dif),new_sec(sec_size-j_dif+i_dif);
    copy(orders[i].begin(),orders[i].begin()+i_id,new_fst.begin());
    copy(orders[j].begin(),orders[j].begin()+j_id,new_sec.begin());
    copy(orders[j].begin()+j_id,orders[j].end(),new_fst.begin()+i_id);
    copy(orders[i].begin()+i_id,orders[i].end(),new_sec.begin()+j_id);

    orders[i]=new_fst;
    orders[j]=new_sec;

    TwoOpt(orders[i],dis_mat);
    TwoOpt(orders[j],dis_mat);
}

void CrossExchangeNeighbor(const vector<int>& weights,vector<vector<int>>& orders,
                            const vector<vector<float>>& dis_mat,const int truck_capacity)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    auto c=comb(truck_size,2);
    auto st=chrono::system_clock::now();

    while(1){
        auto end=chrono::system_clock::now();
        auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        if(msec>limit_time_millisec) break;

        bool is_changed=false;
        for(const auto& ids : c){
            int i=ids[0]-1,j=ids[1]-1;
            is_changed=SubCross(weights,orders,dis_mat,truck_capacity,i,j);
            if(is_changed) break;
        }

        if(!is_changed) break;
    }
}

bool SubCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                const int i,const int j)
{
    int fst_size=orders[i].size();
    int sec_size=orders[j].size();
    for(int i_st=1; i_st<fst_size-1; i_st++){
        for(int i_end=i_st+1; i_end<fst_size; i_end++){
            for(int j_st=1; j_st<sec_size-1; j_st++){
                for(int j_end=j_st+1; j_end<sec_size; j_end++){
                    int fst_weight=TotalWeight(orders[i].begin()+i_st,orders[i].begin()+i_end,weights);
                    int sec_weight=TotalWeight(orders[j].begin()+j_st,orders[j].begin()+j_end,weights);

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

void ImprovedCrossExchangeNeighbor(const vector<int>& weights,vector<vector<int>>& orders,
                            const vector<vector<float>>& dis_mat,const int truck_capacity)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    vector<vector<bool>> tabu_list(truck_size,vector<bool>(truck_size,false));
    auto c=comb(truck_size,2);
    reverse(c.begin(),c.end());
    auto st=chrono::system_clock::now();

    while(1){
        auto end=chrono::system_clock::now();
        auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        if(msec>limit_time_millisec) break;

        bool is_changed=false;
        for(const auto& ids : c){
            int i=ids[0]-1,j=ids[1]-1;
            if(tabu_list[i][j]) continue;
            is_changed=SubImprovedCross(weights,orders,dis_mat,truck_capacity,i,j);
            if(is_changed){
                for(int id=0; id<truck_size; id++){
                    tabu_list[i][id]=false;
                    tabu_list[id][j]=false;
                }
                tabu_list[i][j]=true;
                break;
            } 
        }

        if(!is_changed) break;
    }
}

bool SubImprovedCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                const int i,const int j)
{
    int fst_size=orders[i].size();
    int sec_size=orders[j].size();
    int total_truck_i=TotalWeight(orders[i],weights);
    int total_truck_j=TotalWeight(orders[j],weights);
    for(int i_st=1; i_st<fst_size-1; i_st++){
        //変化分だけ重量を足すことで高速化。最初のi_endのループでi_stの重量が追加されるため
        //ここは0で、足すのは添字番号i_end-1のものになる
        int fst_weight=0;
        for(int i_end=i_st+1; i_end<fst_size; i_end++){
            fst_weight+=weights[orders[i][i_end-1]];
            for(int j_st=1; j_st<sec_size-1; j_st++){
                int sec_weight=0;
                for(int j_end=j_st+1; j_end<sec_size; j_end++){
                    sec_weight+=weights[orders[j][j_end-1]];

                    bool is_valid_truck_i=total_truck_i-fst_weight+sec_weight<=truck_capacity;
                    bool is_valid_truck_j=total_truck_j-sec_weight+fst_weight<=truck_capacity;
                    if(!is_valid_truck_i) break;
                    if(!is_valid_truck_j) continue;
                    else{
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

void FastCrossExchange(const vector<int>& weights,vector<vector<int>>& orders,
                        const vector<vector<float>>& dis_mat,const int truck_capacity,
                        vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    auto st=chrono::system_clock::now();
    while(1){
        bool is_changed=false;
        for(int i=0; i<truck_size; i++){
            for(int cus : orders[i]){
                auto end=chrono::system_clock::now();
                auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
                if(msec>limit_time_millisec) return;

                is_changed=SubFastCross(weights,orders,dis_mat,truck_capacity,
                                                truck_ids,nn_list,i,cus);
                if(is_changed) break;
            }
        }
        if(!is_changed) return;
    }
}

bool SubFastCross(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list,
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
                int fst_weights=TotalWeight(orders[i].begin()+i_st,orders[i].begin()+i_end,weights);
                int sec_weights=TotalWeight(orders[j].begin()+j_st,orders[j].begin()+j_end,weights);

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

void TwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    auto c=comb(truck_size,2);
    auto st=chrono::system_clock::now();

    while(1){
        auto end=chrono::system_clock::now();
        auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        if(msec>limit_time_millisec) break;

        bool is_changed=false;
        for(const auto& ids : c){
            int i=ids[0]-1,j=ids[1]-1;
            is_changed=SubTwoOptStar(weights,orders,dis_mat,truck_capacity,i,j);
            if(is_changed) break;
        }

        if(!is_changed) break;
    }
}

bool SubTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    const int i,const int j)
{
    int fst_size=orders[i].size();
    int sec_size=orders[j].size();
    for(int i_id=fst_size-2; i_id>=1; i_id--){
        for(int j_id=sec_size-2; j_id>=1; j_id--){
            int i_dif=fst_size-i_id,j_dif=sec_size-j_id;
            int fst_weights=TotalWeight(orders[i].begin()+i_id,orders[i].end(),weights);
            int sec_weights=TotalWeight(orders[j].begin()+j_id,orders[j].end(),weights);

            if(IsValidWeight(orders[i],orders[j],weights,fst_weights,sec_weights,truck_capacity)){
                double dif=GetTwoOptStarDiff(dis_mat,orders,i,j,i_id,j_id);
                
                if(dif<0 && abs(dif)>0.0001){
                    UpdateTwoOptStarOrders(orders,dis_mat,i,j,fst_size,sec_size,
                                        i_dif,j_dif,i_id,j_id);
                    return true;
                }
            }
        }
    }
    return false;
}

void FastTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    auto st=chrono::system_clock::now();
    while(1){
        auto end=chrono::system_clock::now();
        auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        if(msec>limit_time_millisec) break;

        bool is_changed=SubFastTwoOptStar(weights,orders,dis_mat,truck_capacity,
                                        truck_ids,nn_list);
        if(!is_changed) break;
    }
}

bool SubFastTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    vector<pair<int,int>>& truck_ids,const vector<set<int>>& nn_list)
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

                int fst_weights=TotalWeight(orders[i].begin()+fst_id,orders[i].end(),weights);
                int sec_weights=TotalWeight(orders[j].begin()+sec_id,orders[j].end(),weights);

                if(IsValidWeight(orders[i],orders[j],weights,fst_weights,sec_weights,truck_capacity)){
                    double dif=GetTwoOptStarDiff(dis_mat,orders,i,j,fst_id,sec_id);
                    
                    if(dif<0 && abs(dif)>0.0001){
                        UpdateTwoOptStarOrders(orders,dis_mat,i,j,fst_size,sec_size,
                                                i_dif,j_dif,fst_id,sec_id);

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

void ImprovedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=TotalWeight(orders[i],weights);
    
    vector<vector<bool>> tabu_list(truck_size,vector<bool>(truck_size,false));
    auto c=comb(truck_size,2);
    auto st=chrono::system_clock::now();

    while(1){
        auto end=chrono::system_clock::now();
        auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        if(msec>limit_time_millisec) break;

        bool is_changed=false;
        for(const auto& ids : c){
            int i=ids[0]-1,j=ids[1]-1;
            if(tabu_list[i][j]) continue;
            is_changed=SubImprovedTwoOptStar(weights,orders,dis_mat,truck_capacity,i,j);
            if(is_changed){
                for(int id=0; id<truck_size; id++){
                    tabu_list[i][id]=false;
                    tabu_list[id][j]=false;
                }
                tabu_list[i][j]=true;
                break;
            } 
        }

        if(!is_changed) break;
    }
}

bool SubImprovedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                    const vector<vector<float>>& dis_mat,const int truck_capacity,
                    const int i,const int j)
{
    int fst_size=orders[i].size();
    int sec_size=orders[j].size();
    int total_truck_i=TotalWeight(orders[i],weights);
    int total_truck_j=TotalWeight(orders[j],weights);
    int fst_weights=0;
    for(int i_id=fst_size-2; i_id>=1; i_id--){
        int i_dif=fst_size-i_id;
        fst_weights+=weights[orders[i][i_id]];
        int sec_weights=0;

        for(int j_id=sec_size-2; j_id>=1; j_id--){
            int j_dif=sec_size-j_id;
            sec_weights+=weights[orders[j][j_id]];

            bool is_valid_truck_i=total_truck_i-fst_weights+sec_weights<=truck_capacity;
            bool is_valid_truck_j=total_truck_j-sec_weights+fst_weights<=truck_capacity;
            if(!is_valid_truck_i) break;
            if(!is_valid_truck_j) continue;
            else{
                double dif=GetTwoOptStarDiff(dis_mat,orders,i,j,i_id,j_id);
                
                if(dif<0 && abs(dif)>0.0001){
                    UpdateTwoOptStarOrders(orders,dis_mat,i,j,fst_size,sec_size,
                                        i_dif,j_dif,i_id,j_id);
                    return true;
                }
            }
        }
    }
    return false;
}

void IteratedTwoOptStar(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                int iterated_size,bool out)
{
    vector<long long> times(iterated_size);
    vector<double> scores(iterated_size);
    for(int i=0; i<iterated_size; i++){
        auto st=chrono::system_clock::now();
        ImprovedTwoOptStar(weights,orders,dis_mat,truck_capacity);
        auto end=chrono::system_clock::now();
        auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        times[i]=msec;
        scores[i]=TotalDistance(orders,dis_mat);
        if(i!=iterated_size-1){
            Kick(orders,weights,truck_capacity);
        }  
    }
    if(out){
        cout<<"times(ms):";
        for_each(times.begin(),times.end(),[](long long x){cout<<x<<"->";});
        cout<<"\nscores:";
        for_each(scores.begin(),scores.end(),[](double x){cout<<x<<"->";});
        cout<<endl;
    }
}

void IteratedCross(const vector<int>& weights,vector<vector<int>>& orders,
                const vector<vector<float>>& dis_mat,const int truck_capacity,
                int iterated_size,bool out)
{
    vector<long long> times(iterated_size);
    vector<double> scores(iterated_size);
    for(int i=0; i<iterated_size; i++){
        auto st=chrono::system_clock::now();
        ImprovedCrossExchangeNeighbor(weights,orders,dis_mat,truck_capacity);
        auto end=chrono::system_clock::now();
        auto msec=chrono::duration_cast<chrono::milliseconds>(end-st).count();
        times[i]=msec;
        scores[i]=TotalDistance(orders,dis_mat);
        if(i!=iterated_size-1){
            Kick(orders,weights,truck_capacity);
        }
    }
    if(out){
        cout<<"times(ms):";
        for_each(times.begin(),times.end(),[](long long x){cout<<x<<"->";});
        cout<<"\nscores:";
        for_each(scores.begin(),scores.end(),[](double x){cout<<x<<"->";});
        cout<<endl;
    }
}

bool IsValidWeight(const vector<int>& order_i,const vector<int>& order_j,
                const vector<int>& weights,const int fst_weight,const int sec_weight,
                const int truck_capacity)
{
    bool ret=(TotalWeight(order_i,weights)-fst_weight+sec_weight<=truck_capacity)
            &&(TotalWeight(order_j,weights)-sec_weight+fst_weight<=truck_capacity);
    return ret;
}

void Kick(vector<vector<int>>& orders,const vector<int>& weights,int capacity){
    DoubleBridge(orders);
    //FourOptStar(orders,weights,capacity);
}