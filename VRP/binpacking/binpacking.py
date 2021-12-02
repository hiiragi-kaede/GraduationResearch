from matplotlib import pyplot as plt
import numpy as np
import pandas as pd
import pulp
import math
import pprint

def solve_bin_packing(B, s):
    n, r = len(s), list(range(1, len(s)+1))
    x = np.array([pulp.LpVariable('x%d,%d' %(i,j), cat='Binary')
                for i in r for j in r]).reshape(n, n)
    y = [pulp.LpVariable('y%d' %i, cat='Binary') for i in r]
    lp = pulp.LpProblem()
    lp += pulp.lpSum(y)
    for i in range(n):
        lp += pulp.lpSum(x[i]) == 1
    for i in range(n):
        lp += pulp.lpSum(pulp.lpDot(s, x[:,i])) <= B * y[i]
    lp.writeLP('m1.lp')
    solver=pulp.PULP_CBC_CMD(msg=0)
    lp.solve(solver)
    #print(pulp.LpStatus[lp.status], pulp.value(lp.objective))
    res = [[_.varValue * s[i] for _ in x[i]] for i in range(x.shape[0])]
    mat=[]
    for i in range(x.shape[0]):
        tmp=[d for d in range(len(res[i])) if res[d][i]!=0]
        mat.append(tmp)
    ret=[mat[i] for i in range(len(mat)) if len(mat[i])!=0]
    return ret


def draw_solution(s, x):
    res = {i+1: [_.varValue * s[i] for _ in x[i]] for i in range(x.shape[0])}
    df = pd.DataFrame(res)
    df.plot.bar(stacked=True, rot=True)
    plt.savefig('colgen_m1.png', bbox_inches='tight')
    plt.show()

if __name__ == '__main__':
    fname="../CVRP_Data/vrplib/X-n228-k23.txt"
    data=[]
    with open(fname) as f:
        size=int(f.readline())
        TRUCK_CAPACITY=int(f.readline())
        for i in range(size):
            tmp=list(map(float,f.readline().split()[1:]))
            data.append({"x":tmp[0],"y":tmp[1]})
            
        #DEMAND_SECTIONの行を取り除く
        _=f.readline()
        for i in range(size):
            buf=f.readline().split()
            data[i]["weight"]=int(buf[1])

        print("data file:",fname)
        total_weight=sum([i["weight"] for i in data])
        print("total weight:",total_weight)
        TRUCK_SIZE=math.ceil(total_weight/TRUCK_CAPACITY)
        print("truck size:",TRUCK_SIZE)
        B, s = TRUCK_CAPACITY,[i["weight"] for i in data]
        ret=solve_bin_packing(B, s)
        for i in range(len(ret)):
            #print("truck",str(i+1),":",sum([data[j]["weight"] for j in ret[i]]),"/",TRUCK_CAPACITY)
            print("truck",str(i+1),":",ret[i])