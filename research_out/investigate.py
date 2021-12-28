import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import os

def input_data(f,id,times,data,improve):
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

def make_data_frame(file):
    file_path="./"+file
    outs=["cross_normal.txt","cross_h.txt","cross_l.txt","cross_lh.txt",
            "cross_t.txt","cross_th.txt","cross_tl.txt","cross_tlh.txt"]
    data=[[] for _ in range(8)]
    times=[[] for _ in range(8)]
    improve=[[] for _ in range(8)]
    for id,out in enumerate(outs):
        with open(file_path+"/"+out) as f:
            #各8スレッド4回分のデータの出力を受け取る
            input_data(f,id,times,data,improve)

    cols=["ILS_type","ILS_ms","before_cost","after_cost","improve_rate"]
    df=pd.DataFrame(index=[],columns=cols)
    for i in range(8):
        for j in range(32):
            tmp=pd.DataFrame(data=[[outs[i][6:-4],times[i][j],data[i][j]["bef"],
                            data[i][j]["aft"],improve[i][j]]],columns=cols)
            df=df.append(tmp,ignore_index=True)
    
    return df

def image_output(file,df):
    img_path="img"
    os.makedirs(img_path+"/"+file,exist_ok=True)
    out_path=img_path+"/"+file
    sns.boxplot(x="ILS_type",y="before_cost",data=df)
    plt.title(file[:-4]+"'s cost before ILS")
    plt.savefig(out_path+"/before.png")
    plt.clf()
    sns.boxplot(x="ILS_type",y="after_cost",data=df)
    plt.title(file[:-4]+"'s cost after ILS")
    plt.savefig(out_path+"/after.png")
    plt.clf()
    sns.boxplot(x="ILS_type",y="ILS_ms",data=df)
    plt.title(file[:-4]+"'s ILS millisec")
    plt.savefig(out_path+"/ms.png")
    plt.clf()
    sns.boxplot(x="ILS_type",y="improve_rate",data=df)
    plt.title(file[:-4]+"'s improvement rate")
    plt.savefig(out_path+"/imp_rate.png")
    plt.clf()

def info_output(file):
    fname="info/"+file[:-4]+".txt"
    with open(fname,"w") as f:
        types=["normal","h","l","lh","t","th","tl","tlh"]
        cols=["type","1st_quantile","median","3rd_quantile","mean","std","min","max"]
        print("ILS msec infos:",file=f)
        ms_df=pd.DataFrame(index=[],columns=cols)
        for i in range(8):
            data=df[df.ILS_type == types[i]]
            ms=data["ILS_ms"]
            tmp=pd.DataFrame(data=[[types[i],ms.quantile(0.25),ms.median(),ms.quantile(0.75),
                                    ms.mean(),ms.std(),ms.min(),ms.max()]],columns=cols)
            ms_df=ms_df.append(tmp,ignore_index=True)
        print(ms_df,end="\n\n",file=f)
        
        print("ILS before costs:",file=f)
        data=df[df.ILS_type == types[i]]
        bef=data["before_cost"]    
        bef_df=pd.DataFrame(index=[],columns=cols[1:])
        bef_df=bef_df.append(pd.DataFrame(data=[[bef.quantile(0.25),bef.median(),bef.quantile(0.75),
                                bef.mean(),bef.std(),bef.min(),bef.max()]],columns=cols[1:]))
        print(bef_df,end="\n\n",file=f)
        
        print("ILS after costs:",file=f)
        aft_df=pd.DataFrame(index=[],columns=cols)
        for i in range(8):
            data=df[df.ILS_type == types[i]]
            aft=data["after_cost"]
            tmp=pd.DataFrame(data=[[types[i],aft.quantile(0.25),aft.median(),aft.quantile(0.75),
                                    aft.mean(),aft.std(),aft.min(),aft.max()]],columns=cols)
            aft_df=aft_df.append(tmp,ignore_index=True)
        print(aft_df,end="\n\n",file=f)
        
        print("ILS improvement rates:",file=f)
        imp_df=pd.DataFrame(index=[],columns=cols)
        for i in range(8):
            data=df[df.ILS_type == types[i]]
            imp=data["improve_rate"]
            tmp=pd.DataFrame(data=[[types[i],imp.quantile(0.25),imp.median(),imp.quantile(0.75),
                                    imp.mean(),imp.std(),imp.min(),imp.max()]],columns=cols)
            imp_df=imp_df.append(tmp,ignore_index=True)
        print(imp_df,end="",file=f)


#main関数
path="./"
files=os.listdir(path)
#出力フォルダのみに限定(img,infoフォルダやこのpythonファイル自体を対象にしないように)
files=[i for i in files if "_out" in i]

for file in files:
    df=make_data_frame(file)
    info_output(file)