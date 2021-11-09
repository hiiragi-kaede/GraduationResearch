#include<vector>
#include<algorithm>
#include<iostream>
#include"util.hpp"
#include<chrono>

using namespace std;

static int limit_time=60;

void two_opt(vector<int>& order,const vector<vector<double>> dis_mat){
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

void cross_exchange_neighbor(const vector<int> weights,vector<vector<int>>& orders,
                            const vector<vector<double>> dis_mat,const int truck_capacity)
{
    int truck_size=orders.size();
    vector<int> total_weight(truck_size,0);
    for(int i=0; i<truck_size; i++) 
        total_weight[i]=calc_total_weight(orders[i],weights);
    
    auto c=comb(truck_size,2);
    auto st=chrono::system_clock::now();

    while(1){
        auto end=chrono::system_clock::now();
        auto sec=chrono::duration_cast<chrono::seconds>(end-st).count();
        if(sec>limit_time) break;

        bool is_changed=false;
        for(auto ids : c){
            int i=ids[0]-1,j=ids[1]-1;
            // cout<<"i:"<<i<<" j:"<<j<<endl;
            // cout<<"order_i:"<<orders[i].size()<<endl;
            // cout<<"order_j"<<orders[j].size()<<endl;
            is_changed=sub_cross(weights,orders,dis_mat,truck_capacity,i,j);
            if(is_changed) break;
        }

        if(!is_changed) break;
    }
}

bool sub_cross(const vector<int> weights,vector<vector<int>>& orders,
                const vector<vector<double>> dis_mat,const int truck_capacity,
                const int i,const int j)
{
    int fst_size=orders[i].size();
    int sec_size=orders[j].size();
    //cout<<"i_size:"<<fst_size<<" j_size:"<<sec_size<<endl;
    for(int i_st=1; i_st<fst_size-2; i_st++){
        for(int i_end=i_st+1; i_end<fst_size-1; i_end++){
            for(int j_st=1; j_st<sec_size-2; j_st++){
                for(int j_end=j_st+1; j_end<sec_size-1; j_end++){
                    // cout<<"i"<<i_st<<"->"<<i_end;
                    // cout<<"   j"<<j_st<<"->"<<j_end<<endl;
                    vector<int> fst_ord,sec_ord;
                    for(int l_i=i_st; l_i<i_end; l_i++) fst_ord.push_back(orders[i][l_i]);
                    for(int l_i=j_st; l_i<j_end; l_i++) sec_ord.push_back(orders[j][l_i]);

                    int fst_weight=calc_total_weight(fst_ord,weights);
                    int sec_weight=calc_total_weight(sec_ord,weights);

                    if(check_weight(orders[i],orders[j],weights,
                        fst_weight,sec_weight,truck_capacity))
                    {
                        double dif=-dis_mat[orders[i][i_st-1]][orders[i][i_st]]
                                    -dis_mat[orders[i][i_end-1]][orders[i][i_end]]
                                    -dis_mat[orders[j][j_st-1]][orders[j][j_st]]
                                    -dis_mat[orders[j][j_end-1]][orders[j][j_end]]
                                    +dis_mat[orders[i][i_st-1]][orders[j][j_st]]
                                    +dis_mat[orders[i][i_end-1]][orders[j][j_end]]
                                    +dis_mat[orders[j][j_st-1]][orders[i][i_st]]
                                    +dis_mat[orders[j][j_end-1]][orders[i][i_end]];
                        if(dif<0){
                            vector<int> new_i,new_j;
                            for(int id=0; id<i_st; id++) new_i.push_back(orders[i][id]);
                            for(int id=0; id<j_st; id++) new_j.push_back(orders[j][id]);

                            for(int id=j_st; id<j_end; id++) new_i.push_back(orders[j][id]);
                            for(int id=i_st; id<i_end; id++) new_j.push_back(orders[i][id]);

                            for(int id=i_end; id<fst_size; id++) new_i.push_back(orders[i][id]);
                            for(int id=j_end; id<sec_size; id++) new_j.push_back(orders[j][id]);

                            orders[i]=new_i;
                            orders[j]=new_j;

                            two_opt(orders[i],dis_mat);
                            two_opt(orders[j],dis_mat);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool check_weight(const vector<int> order_i,const vector<int> order_j,
                const vector<int> weights,const int fst_weight,const int sec_weight,
                const int truck_capacity)
{
    bool ret=(calc_total_weight(order_i,weights)-fst_weight+sec_weight<truck_capacity)
            &&(calc_total_weight(order_j,weights)-sec_weight+fst_weight<truck_capacity);
    return ret;
}

int calc_total_weight(vector<int> order,vector<int> weights){
    int total=0;
    for(int i=0; i<order.size(); i++){
        total+=weights[order[i]];
    }
    return total;
}

double calc_total_dist(const vector<vector<int>> orders,
                    const vector<vector<double>> dis_mat)
{
    double total=0;
    for(auto order : orders){
        for(auto it=order.begin(); (it+1)!=order.end(); it++){
            total+=dis_mat[*it][*(it+1)];
        }
    }
    return total;
}

void check_unvisited(const vector<vector<int>>orders,const vector<int> weights,int n){
    vector<bool> visited(n,false);
    bool out=false;
    cout<<"\e[31munvisited:";
    for(auto order: orders){
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

void show_orders(const vector<vector<int>> orders,const vector<int> weights,
                const int capacity,const int n){
    for(auto order: orders){
        for(int id: order) cout<<id<<",";
        
        //容量オーバーは合計容量を赤く塗る
        int total_weight=calc_total_weight(order,weights);
        if(total_weight>capacity) cout<<"\e[31m";
        cout<<"    "<<total_weight<<"\e[0m";

        cout<<"/capacity:"<<capacity;
        cout<<endl;
    }
    check_unvisited(orders,weights,n);
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