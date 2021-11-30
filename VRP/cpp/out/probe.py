import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

fname="n228_two_simple.txt"
data1,data2=[],[]
with open(fname) as f:
    truck_size=int(f.readline())
    tmp=[[] for _ in range(truck_size)]
    while True:
        trucks=f.readline().split()
        if(trucks[0][0]=="t"): break
        
        trucks=list(map(int,trucks))
        score_dif=float(f.readline())
        tmp[trucks[0]].append(score_dif)
        tmp[trucks[1]].append(score_dif)
    data1=tmp

fname="n228_twofast_simple.txt"
with open(fname) as f:
    truck_size=int(f.readline())
    tmp=[[] for _ in range(truck_size)]
    while True:
        trucks=f.readline().split()
        if(trucks[0][0]=="t"): break
        
        trucks=list(map(int,trucks))
        score_dif=float(f.readline())
        tmp[trucks[0]].append(score_dif)
        tmp[trucks[1]].append(score_dif)

    data2=tmp
    cols=['type','truck','order','diff']
    df=pd.DataFrame(index=[],columns=cols)
    for i in range(len(data1)):
        ele=data1[i]
        for order in range(len(ele)):
            tmp=pd.DataFrame(data=[['two',i,order,ele[order]]],columns=cols)
            df=df.append(tmp,ignore_index=True)
    for i in range(len(data2)):
        ele=data2[i]
        for order in range(len(ele)):
            tmp=pd.DataFrame(data=[['two_fast',i,order,ele[order]]],columns=cols)
            df=df.append(tmp,ignore_index=True)
    
    df = df.sort_values('truck')
    sns.boxenplot(x='truck',y='diff',hue='type',data=df)
    plt.show()