#include<iostream>
#include<vector>
#include<chrono>
#include<fstream>
#include<string>
#include"tools.hpp"
#include"improve.hpp"
#include"construct.hpp"
#include"util.hpp"
#include"improve.hpp"

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

void OutputLattice(const vector<int> lattice,int LatticeSize,
                    const string lattice_fname)
{
    ofstream output(lattice_fname);
    output<<LatticeSize<<endl;
    vector<vector<int>> cnt(LatticeSize,vector<int>(LatticeSize,0));
    for(int i=0; i<lattice.size(); i++){
        int x=lattice[i]%LatticeSize;
        int y=lattice[i]/LatticeSize;
        cnt[y][x]++;
    }
    for(int y=0; y<LatticeSize; y++){
        for(int x=0; x<LatticeSize; x++){
            output<<cnt[y][x]<<" ";
        }
        output<<endl;
    }

    output.close();
}

void SetArgs(int argc,char *argv[],MethodType& method_type,KickType& kick_type,
            IteratedType& iterated_type,bool& use_tabulist,bool& use_lattice)
{
    if(argc>1){
        for(int i=1; i<argc; i++){
            string type=string(argv[i]);
            const string neighbor=type.substr(0,3);
            const string kick=type.substr(0,3);
            const string iterate=type.substr(0,3);
            const string use_t=type.substr(0,3);
            const string use_l=type.substr(0,3);

            if(neighbor=="-n="){
                type=type.substr(3,type.size()-3);
                if(type=="t")    method_type=MethodType::TwoOptStar;
                else if(type=="ft")  method_type=MethodType::FastTwoOptStar;
                else if(type=="it")  method_type=MethodType::ImprovedTwoOptStar;
                else if(type=="c")  method_type=MethodType::Cross;
                else if(type=="fc")  method_type=MethodType::FastCross;
                else if(type=="ic")  method_type=MethodType::ImprovedCross;
                else if(type=="ilst") method_type=MethodType::IteratedTwo;
                else if(type=="ilsc") method_type=MethodType::IteratedCross;
                else{
                    cout<<"正しい近傍タイプを指定してください\n";
                    exit(1);
                }
            }
            else if(kick=="-k="){
                type=type.substr(3,type.size()-3);
                if(type=="d") kick_type=KickType::DoubleBridge;
                else if(type=="f") kick_type=KickType::FourOpt;
                else{
                    cout<<"キックの手法を正しく指定してください\n";
                    exit(1);
                }
            }
            else if(iterate=="-i="){
                type=type.substr(3,type.size()-3);
                if(type=="n") iterated_type=IteratedType::Normal;
                else if(type=="i") iterated_type=IteratedType::Improved;
                else{
                    cout<<"ILSの関数タイプを正しく指定してください\n";
                    exit(1);
                }
            }
            else if(use_t=="-t="){
                if(type[3]=='0') use_tabulist=false;
                else use_tabulist=true;
            }
            else if(use_l=="-l="){
                if(type[3]=='0') use_lattice=false;
                else use_lattice=true;
            }
        }
        
    }
}