import subprocess
import time

#CVRP_Data以下のディレクトリやファイル名を指定
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
#batファイルに入力データ名を渡してコンパイルから書き出しまでを自動で行わせる
for id,fname in enumerate(fnames):
    print(fname,end="",flush=True)
    res=subprocess.run(['task.bat',fname])
    print(":finish",id+1,"/",len(fnames))
    time.sleep(1)