#include<vector>
#include<algorithm>
#include"util.hpp"

using std::vector;

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