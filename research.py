import subprocess

#CVRP_Data以下のディレクトリやファイル名を指定
fnames=["vrplib/X-n110-k13.txt",
        "vrplib/X-n125-k30.txt",
        "vrplib/X-n139-k10.txt",
        "vrplib/X-n176-k26.txt"]
#batファイルに入力データ名を渡してコンパイルから書き出しまでを自動で行わせる
for fname in fnames:
    res=subprocess.run(['task.bat',fname])
    print(fname,":finish")