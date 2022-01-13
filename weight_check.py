import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns

fnames=["vrplib/X-n110-k13.txt",
        "vrplib/X-n125-k30.txt",
        "vrplib/X-n176-k26.txt",
        "vrplib/X-n204-k19.txt",
        "vrplib/X-n270-k35.txt",
        "vrplib/X-n303-k21.txt",
        "vrplib/X-n359-k29.txt",
        "vrplib/X-n401-k29.txt",
        "vrplib/X-n411-k19.txt",
        "vrplib/X-n449-k29.txt",
        "vrplib/X-n480-k70.txt",
        "vrplib/X-n524-k153.txt",
        "vrplib/X-n536-k96.txt",
        "vrplib/X-n561-k42.txt",
        "vrplib/X-n613-k62.txt",
        "vrplib/X-n641-k35.txt",
        "vrplib/X-n685-k75.txt",
        "vrplib/X-n716-k35.txt",
        "vrplib/X-n766-k71.txt",
        "vrplib/X-n801-k40.txt",
        "vrplib/X-n837-k142.txt",
        "vrplib/X-n876-k59.txt",
        "vrplib/X-n895-k37.txt",
        "vrplib/X-n936-k151.txt",
        "vrplib/X-n979-k58.txt",
        "vrplib/X-n1001-k43.txt",
        "large/Leuven_3001.txt"]

caps=[66,188,142,836,585,794,68,745,216,777,52,125,371,74,
    523,1381,408,1007,166,20,44,764,1816,138,998,131,25]

file_offset="VRP/CVRP_Data/"
cols=["data","median","mean","std","min","max"]
df=pd.DataFrame(index=[],columns=cols)
weight_data=[]

for fname in fnames:    
    with open(file_offset+fname) as f:
        n=int(f.readline())
        cap=int(f.readline())
        for _ in range(n): f.readline()
        f.readline()
        
        data=[0 for _ in range(n)]
        for i in range(n): data[i]=int(f.readline().split()[1])
        
        data_name=fname.split("/")[1]
        if "_" not in data_name:
            data_name=data_name.split("-")[1]
        else:
            data_name=data_name.split("_")[1].split(".")[0]
        weights=pd.Series(data)
        weight_data.append([data_name,data])
        tmp=pd.DataFrame(data=[[data_name,weights.median(),weights.mean(),
                                weights.std(),weights.min(),weights.max()]]
                                ,columns=cols)
        df=df.append(tmp,ignore_index=True)

xs,ys=[],[]
for i in range(len(fnames)):
    for j in range(len(weight_data[i][1])):
        xs.append(weight_data[i][0])
        ys.append(weight_data[i][1][j])

fig=plt.figure(figsize=(10,7.5))
ax=fig.add_subplot(111)
sns.boxplot(x=xs,y=ys,ax=ax)
ax.set_xlabel("data")
ax.set_xticklabels([i[0] for i in weight_data],rotation=30)
ax.set_ylabel("demand")
ax.set_title("demand comparison")
for i in range(len(fnames)):
    ax.plot(0,0,label=weight_data[i][0]+":"+str(caps[i]))
fig.subplots_adjust(left=0.1,right=0.85)
ax.legend(loc="upper left",bbox_to_anchor=(1.01,1.0))
# handler,label=ax.get_legend_handles_labels()
# ax.legend(handler,[])
fig.savefig("demand_comparison.png")