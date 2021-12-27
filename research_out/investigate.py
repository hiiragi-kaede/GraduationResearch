import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import os
from pprint import pprint

def input_data(f,times,data,improve):
    for _ in range(4):
            #データや方式についての出力を切り捨てる
            for _ in range(12): f.readline()
            
            for _ in range(8):
                #各スレッドのtime infoまでの行を読み飛ばす
                for _ in range(3): f.readline()
                time_info=f.readline().split()
                #局所探索にかかった時間だけ抽出
                times[id].append(int(time_info[-1].split(":")[-1][:-4]))
                cost_info=f.readline().split(":")[1]
                cost_info=cost_info.split(">")
                data[id].append({"bef":int(cost_info[0][:-3]),"aft":int(cost_info[1][:-1])})
                imp_rate=float(f.readline().split(":")[1])
                improve[id].append(imp_rate)
                #間の改行を読み飛ばす
                f.readline()
            for _ in range(6): f.readline()

path="./"
files=os.listdir(path)
files=[i for i in files if ".py" not in i]
#print(files)

file=files[-1]

file_path="./"+file
outs=["cross_normal.txt","cross_h.txt","cross_l.txt","cross_lh.txt",
        "cross_t.txt","cross_th.txt","cross_tl.txt","cross_tlh.txt"]
data=[[] for _ in range(8)]
times=[[] for _ in range(8)]
improve=[[] for _ in range(8)]
for id,out in enumerate(outs):
    #print("file_name:",file_path+"/"+out)
    with open(file_path+"/"+out) as f:
        #各8スレッド4回分のデータの出力を受け取る
        input_data(f,times,data,improve)

type=["before","after"]
cols=["ILS_type","ILS_ms","type","cost","improve_rate"]
df=pd.DataFrame(index=[],columns=cols)
for i in range(8):
    for j in range(32):
        tmp=pd.DataFrame(data=[[outs[i][6:-4],times[i][j],type[0],
                        data[i][j]["bef"],improve[i][j]]],columns=cols)
        df=df.append(tmp,ignore_index=True)
        tmp=pd.DataFrame(data=[[outs[i][6:-4],times[i][j],type[1],
                        data[i][j]["aft"],improve[i][j]]],columns=cols)
        df=df.append(tmp,ignore_index=True)

df=df.sort_values("type")
sns.boxplot(x="ILS_type",y="cost",hue="type",data=df)
plt.show()