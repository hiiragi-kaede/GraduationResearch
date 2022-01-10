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

extern const vector<int> cus_x,cus_y;

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

int TotalDistance(const vector<vector<int>>& orders,
                    const vector<vector<int>>& dis_mat)
{
    int total=0;
    for(const auto& order : orders){
        total+=TotalDistance(order,dis_mat);
    }
    return total;
}

int TotalDistance(const vector<int>& order,
                    const vector<vector<int>>& dis_mat)
{
    int total=0;
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

vector<vector<int>> ConstructNeighborList(const int n,const vector<vector<int>>& dis_mat){
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

vector<set<int>> ConstructNNList(const vector<vector<int>>& dis_mat,
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
                const vector<vector<int>>& dis_mat,const int truck_capacity,
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
                const vector<vector<int>>& dis_mat,const int truck_capacity,
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
                const vector<vector<int>>& dis_mat,const int truck_capacity,
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
                    const vector<vector<int>>& dis_mat,const int truck_capacity,
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
        if(x==LatticeSize) x--;
        if(y==LatticeSize) y--;
        ret[i]=x+LatticeSize*y;
    }

    return ret;
}

bool IsCCW(const vector<int>& order){
    int min_x=cus_x[order[0]];
    vector<int> min_x_ids{0};
    for(int i=1; i<order.size(); i++){
        int x=cus_x[order[i]];
        if(x<min_x){
            min_x_ids={i};
            min_x=x;
        }
        else if(x==min_x){
            min_x_ids.push_back(i);
        }
    }
    int min_y_id=min_x_ids[0];
    int min_y=cus_y[order[min_x_ids[0]]];
    //もしx座標最小の点が複数あるときは、その中でさらにy座標最小の点を求める
    if(min_x_ids.size()>1){
        for(int i=1; i<min_x_ids.size(); i++){
            int y=cus_y[order[min_x_ids[i]]];
            if(y<min_y){
                min_y_id=min_x_ids[i];
                min_y=y;
            }
        }
    }

    int min_prev=min_y_id-1;
    if(min_prev<0) min_prev=order.size()-2;
    int min_next=min_y_id+1;
    if(min_next==order.size()-1) min_next=1;

    int x_ab=cus_x[order[min_next]]-cus_x[order[min_y_id]];
    int y_ab=cus_y[order[min_next]]-cus_y[order[min_y_id]];
    int x_ac=cus_x[order[min_prev]]-cus_x[order[min_y_id]];
    int y_ac=cus_y[order[min_prev]]-cus_y[order[min_y_id]];

    return (x_ab*y_ac-x_ac*y_ab<0);
}

bool IsCustomerInArea(const vector<int>& order,int cus_id)
{
    int wn=0;
    float vt=0;

    for(int i=0; i<order.size()-2; i++){
        int cur=order[i];
        int next=order[i+1];

        //上向きの辺。点がY軸方向に対して、始点と終点の間にある。(終点は含まない)
        if(cus_y[cur]<=cus_y[cus_id] && cus_y[next]>cus_y[cus_id]){
            //辺は点よりも右側にあり、重ならない
            //辺が点と同じ高さになる位置を特定し、その時のXの値と点のXの値を比較する
            vt=(cus_y[cus_id]-cus_y[cur])/(cus_y[next]-cus_y[cur]);

            if(cus_x[cus_id]<(cus_x[cur]+(vt*(cus_x[next]-cus_x[cur])))){
                //上向きの辺と交差した場合は+1
                wn++;
            }
        }
        else if(cus_y[cur]>cus_y[cus_id] && cus_y[next]<=cus_y[cus_id]){
            vt=(cus_y[cus_id]-cus_y[cur])/(cus_y[next]-cus_y[cur]);

            if(cus_x[cus_id]<(cus_x[cur]+(vt*(cus_x[next]-cus_x[cur])))){
                //下向きの辺と交差した場合は-1
                wn--;
            }
        }
    }

    // if(wn!=0){
    //     cout<<"order's route\n";
    //     for(int i=0; i<order.size(); i++){
    //         int id=order[i];
    //         cout<<"("<<cus_x[id]<<","<<cus_y[id]<<") ";
    //     }
    //     cout<<"\ncustomer:("<<cus_x[cus_id]<<","<<cus_y[cus_id]<<")\n";
    // }
    return wn!=0;
}

bool IsOverlapOrders(const vector<vector<int>>& orders,int i,int j,
                    vector<vector<int>>& order_xs,vector<vector<int>>& order_ys){
    //バウンディングボックスが接触していなければ詳しく調査をさせない
    //string status=IsHitBoundingBox(orders,i,j,order_xs,order_ys)?"true":"false";
    //cout<<status;
    //if(!IsHitBoundingBox(orders,i,j,order_xs,order_ys)) return false;

    for(int id=1; id<orders[i].size()-1; id++){
        int cus_id=orders[i][id];
        if(IsCustomerInArea(orders[j],cus_id)){
            //cout<<":true\n";
            return true;
        }
    }
    for(int id=1; id<orders[j].size()-1; id++){
        int cus_id=orders[j][id];
        if(IsCustomerInArea(orders[i],cus_id)){
            //cout<<":true\n";
            return true;
        }
    }
    //cout<<":false\n";
    return false;
}

bool IsHitBoundingBox(const vector<vector<int>>& orders,int i,int j,
                    const vector<vector<int>>& order_xs,const vector<vector<int>>& order_ys){
    int i_min_x=*min_element(order_xs[i].begin()+1,order_xs[i].end()-1);
    int i_max_x=*max_element(order_xs[i].begin()+1,order_xs[i].end()-1);
    int j_min_x=*min_element(order_xs[j].begin()+1,order_xs[j].end()-1);
    int j_max_x=*max_element(order_xs[j].begin()+1,order_xs[j].end()-1);
    int i_min_y=*min_element(order_ys[i].begin()+1,order_ys[i].end()-1);
    int i_max_y=*max_element(order_ys[i].begin()+1,order_ys[i].end()-1);
    int j_min_y=*min_element(order_ys[j].begin()+1,order_ys[j].end()-1);
    int j_max_y=*max_element(order_ys[j].begin()+1,order_ys[j].end()-1);

    // cout<<"i_x:"<<i_min_x<<"~"<<i_max_x<<endl;
    // cout<<"i_y:"<<i_min_y<<"~"<<i_max_y<<endl;
    // cout<<"j_x:"<<j_min_x<<"~"<<j_max_x<<endl;
    // cout<<"j_y:"<<j_min_y<<"~"<<j_max_y<<endl;
    // cout<<"depo:"<<order_xs[i][0]<<","<<order_ys[i][0]<<endl;

    //iにデポを含めた場合
    int tmp_min_x=min(i_min_x,order_xs[i][0]);
    int tmp_max_x=max(i_max_x,order_xs[i][0]);
    int tmp_min_y=min(i_min_y,order_ys[i][0]);
    int tmp_max_y=max(i_max_y,order_ys[i][0]);

    if((tmp_min_x<=j_max_x && tmp_max_x>=j_min_x) && (tmp_min_y<=j_max_y && tmp_max_y>=j_min_y)) return true;
    
    //jにデポを含めた場合
    int tmp_min_x2=min(j_min_x,order_xs[j][0]);
    int tmp_max_x2=max(j_max_x,order_xs[j][0]);
    int tmp_min_y2=min(j_min_y,order_ys[j][0]);
    int tmp_max_y2=max(j_max_y,order_ys[j][0]);
    
    if((tmp_min_x2<=i_max_x && tmp_max_x2>=i_min_x) && (tmp_min_y2<=i_max_y && tmp_max_y2>=i_min_y)) return true;

    return false;
}

//order_xsとorder_ysをordersに合わせて更新する
void UpdateOrderPos(const vector<vector<int>>& orders,vector<vector<int>>& order_xs,
                    vector<vector<int>>& order_ys){
    order_xs.resize(orders.size());
    order_ys.resize(orders.size());

    for(int i=0; i<orders.size(); i++){
        vector<int> tmp_xs(orders[i].size()),tmp_ys(orders[i].size());
        for(int j=0; j<orders[i].size(); j++){
            tmp_xs[j]=cus_x[orders[i][j]];
            tmp_ys[j]=cus_y[orders[i][j]];
        }
        order_xs[i]=tmp_xs;
        order_ys[i]=tmp_ys;
    }
}

//order_xsとorder_ysをordersに合わせて更新する。
//添字で渡したi,jのみを更新する高速版
void UpdateOrderPos(const vector<vector<int>>& orders,int i,int j,
                    vector<vector<int>>& order_xs,vector<vector<int>>& order_ys){
    vector<int> tmp_xs(orders[i].size()),tmp_ys(orders[i].size());
    for(int id=0; id<orders[i].size(); id++){
        tmp_xs[id]=cus_x[orders[i][id]];
        tmp_ys[id]=cus_y[orders[i][id]];
    }
    order_xs[i]=tmp_xs;
    order_ys[i]=tmp_ys;

    vector<int> tmp_xs2(orders[j].size()),tmp_ys2(orders[j].size());
    for(int id=0; id<orders[j].size(); id++){
        tmp_xs2[id]=cus_x[orders[j][id]];
        tmp_ys2[id]=cus_y[orders[j][id]];
    }
    order_xs[j]=tmp_xs2;
    order_ys[j]=tmp_ys2;
}