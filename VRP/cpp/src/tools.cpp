#include<iostream>
#include<vector>
#include<chrono>
#include<fstream>
#include"tools.hpp"
#include"improve.hpp"
#include"construct.hpp"
#include"util.hpp"

using namespace std;

static const vector<string> TypeName{
    "TwoOptStar","FastTwoOptStar","ImprovedTwoOptStar",
    "Cross","FastCross","ImprovedCross",
    "IteratedTwoOptStar","IteratedCross"
};

void TrialInsertConstruct(const vector<vector<int>>& dis_mat,const vector<int>& weights,
                        const int capacity,const int truck_size,
                        vector<vector<int>>& orders,vector<pair<int,int>>& truck_ids,
                        int CONSTRUCT_LIMIT_MS,int THREAD_SIZE)
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

void ThreadProcess(const vector<vector<int>>& dis_mat,const vector<int>& weights,
                    const int capacity,const int truck_size,
                    long long& construct_ms,long long& local_search_msec,int& bef_dist,int& aft_dist,
                    vector<vector<int>>& ans_orders,const vector<set<int>>& nn_list,vector<int>& lattice,
                    MethodType method_type,int THREAD_SIZE,int ITERATED_SIZE,int CONSTRUCT_LIMIT_MS)
{
    /*==========construct initial answer==========*/
    int n=dis_mat.size();
    vector<vector<int>> orders;
    vector<pair<int,int>> truck_ids(n);

    auto st=chrono::system_clock::now();
    TrialInsertConstruct(dis_mat,weights,capacity,truck_size,orders,truck_ids,CONSTRUCT_LIMIT_MS,THREAD_SIZE);
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
        ImprovedTwoOptStar(weights,orders,dis_mat,capacity,lattice);
        break;
    case MethodType::Cross:
        CrossExchangeNeighbor(weights,orders,dis_mat,capacity);
        break;
    case MethodType::FastCross:
        FastCrossExchange(weights,orders,dis_mat,capacity,truck_ids,nn_list);
        break;
    case MethodType::ImprovedCross:
        ImprovedCrossExchangeNeighbor(weights,orders,dis_mat,capacity,lattice);
        break;
    case MethodType::IteratedTwo:
        if(THREAD_SIZE!=1)
            IteratedTwoOptStar(weights,orders,dis_mat,capacity,ITERATED_SIZE,lattice,false);
        else
            IteratedTwoOptStar(weights,orders,dis_mat,capacity,ITERATED_SIZE,lattice);
        break;
    case MethodType::IteratedCross:
        if(THREAD_SIZE!=1)
            IteratedCross(weights,orders,dis_mat,capacity,ITERATED_SIZE,lattice,false);
        else
            IteratedCross(weights,orders,dis_mat,capacity,ITERATED_SIZE,lattice);
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
                    const vector<int>& befs,const vector<int>& afts,int capacity,int n,
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
        cout<<"improve rate:"<<100-((double)afts[i]/befs[i]*100)<<endl;
        cout<<endl;
    }
}

void ShowThreadsAves(int THREAD_SIZE,const vector<long long>& local_searches,
                    const vector<int>& befs,const vector<int>& afts,int minid)
{
    double bef_ave=0,aft_ave=0,imp_rate_ave=0,ls_ave=0;
    for(int i=0; i<THREAD_SIZE; i++){
        ls_ave+=(double)local_searches[i]/THREAD_SIZE;
        bef_ave+=(double)befs[i]/THREAD_SIZE;
        aft_ave+=(double)afts[i]/THREAD_SIZE;
        imp_rate_ave+=(100-(double)afts[i]/befs[i]*100)/THREAD_SIZE;
    }
    cout<<"construct average score:"<<bef_ave<<endl;
    cout<<"improved average score:"<<aft_ave<<endl;
    cout<<"local search average milliseconds:"<<ls_ave<<endl;
    cout<<"improve rate average:"<<imp_rate_ave<<endl;
    cout<<"minimum:"<<afts[minid]<<endl;
}

vector<string> GetTypeNames(){
    return TypeName;
}

void InputData(int& n,int& capacity,vector<int>& cus_x,vector<int>& cus_y,vector<int>& weights)
{
    cin>>n;
    cin>>capacity;
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
    
    for(int i=0; i<n; i++){
        int tmp;
        cin>>idx>>tmp;
        weights.push_back(tmp);
    }
}

void OutputData(int n,const vector<int>& cus_x,const vector<int>& cus_y,
                const vector<int>& weights,const vector<vector<int>>& orders,
                string data_fname,string order_fname)
{
    
    ofstream output(data_fname);
    for(int i=0; i<n; i++){
        output<<cus_x[i]<<" "<<cus_y[i]<<" "<<weights[i]<<endl;
    }
    output.close();

    ofstream ord_output(order_fname);
    for(auto& order: orders){
        for(int cus: order){
            ord_output<<cus<<" ";
        }
        ord_output<<endl;
    }
    ord_output.close();
}