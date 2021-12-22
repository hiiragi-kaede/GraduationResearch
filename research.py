import subprocess

#CVRP_Data以下のディレクトリやファイル名を指定
fnames=["vrplib/X-n228-k23.txt",
        "vrplib/X-n331-k15.txt"]
#batファイルに入力データ名を渡してコンパイルから書き出しまでを自動で行わせる
for fname in fnames:
    res=subprocess.run(['research.bat',fname])