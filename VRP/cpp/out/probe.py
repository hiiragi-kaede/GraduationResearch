import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

def isInt(s):
    try:
        int(s,10)
    except ValueError:
        return False
    else:
        return True

fname="n1001/two.txt"
data1=[]
with open(fname) as f:
    truck_size=int(f.readline())
    tmp=[[] for _ in range(truck_size)]
    while True:
        trucks=f.readline().split()
        if(not isInt(trucks[0][0])): break
        
        trucks=list(map(int,trucks))
        score_dif=float(f.readline())
        tmp[trucks[0]].append(score_dif)
        tmp[trucks[1]].append(score_dif)
    data1=tmp

# fnames=["n228/10p_twofast_simple.txt",
#         "n228/15p_twofast.txt",
#         "n228/20p_twofast.txt"]
# types=["10% twofast","15% twofast","20% twofast"]
fnames=["n1001/two_fast.txt"]
types=["two fast"]
datas=[]
for fname in fnames:
    with open(fname) as f:
        truck_size=int(f.readline())
        tmp=[[] for _ in range(truck_size)]
        while True:
            trucks=f.readline().split()
            if(not isInt(trucks[0][0])): break
            
            trucks=list(map(int,trucks))
            score_dif=float(f.readline())
            tmp[trucks[0]].append(score_dif)
            tmp[trucks[1]].append(score_dif)

        datas.append(tmp)

cols=['type','truck','order','diff']
df=pd.DataFrame(index=[],columns=cols)
for i in range(len(data1)):
    ele=data1[i]
    for order in range(len(ele)):
        tmp=pd.DataFrame(data=[['two',i,order,ele[order]]],columns=cols)
        df=df.append(tmp,ignore_index=True)
for j in range(len(types)):
    for i in range(len(datas[j])):
        ele=datas[j][i]
        for order in range(len(ele)):
            tmp=pd.DataFrame(data=[[types[j],i,order,ele[order]]],columns=cols)
            df=df.append(tmp,ignore_index=True)

df = df.sort_values('truck')
sns.boxenplot(x='truck',y='diff',hue='type',data=df)
labels=['two']+types
plt.legend(labels=labels,loc='best')
plt.title("construct")
plt.show()