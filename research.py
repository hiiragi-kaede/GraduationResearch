import subprocess

#CVRP_Data以下のディレクトリやファイル名を指定
fname="vrplib/X-n856-k95.txt"
#batファイルに入力データ名を渡してコンパイルから書き出しまでを自動で行わせる
subprocess.run(['research.bat',fname])