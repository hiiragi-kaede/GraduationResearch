import os
from decimal import Decimal,ROUND_HALF_UP

file_path="./info"

files=os.listdir(file_path)
files=[i for i in files if ".txt" in i]

out_dir=file_path+"/tex_out/"

for file in files:
    with open(file_path+"/"+file) as f:
        for _ in range(2): f.readline()
        msecs=[]
        for _ in range(8):
            msecs.append(f.readline().split()[1:])
        
        for _ in range(3): f.readline()
        bef=f.readline().split()[1:]
        
        for _ in range(3): f.readline()
        afts=[]
        for _ in range(8):
            afts.append(f.readline().split()[1:])
        
        for _ in range(3): f.readline()
        imps=[]
        for _ in range(8):
            imps.append(f.readline().split()[1:])

    out_file=out_dir+file[:-3]+"tex"
    with open(out_file,"w") as f:
        #探索前出力
        print("\\begin{table}[htbp]",file=f)
        tmp=file[:-4]
        if "_" in tmp: tmp=tmp.replace("_","\_")
        print("    \\caption{"+tmp+" 探索前評価値}",file=f)
        print("""\
    \\begin{tabular}{|l|l|l|l|l|l|l|l|}\hline
    \multicolumn{1}{|c|}{\\textbf{1st}}
    &\multicolumn{1}{c|}{\\textbf{median}}
    &\multicolumn{1}{c|}{\\textbf{3rd}}
    &\multicolumn{1}{c|}{\\textbf{mean}}
    &\multicolumn{1}{c|}{\\textbf{std}}
    &\multicolumn{1}{c|}{\\textbf{min}}
    &\multicolumn{1}{c|}{\\textbf{max}}\\\\\\hline""",file=f)
        print("\t",end="",file=f)
        for i in range(3):
            print(bef[i],end=" & ",file=f)
        for i in range(3,5):
            print(Decimal(bef[i]).quantize(Decimal('0.001'),
                rounding=ROUND_HALF_UP),end=" & ",file=f)
        for i in range(5,len(bef)-1):
            print(bef[i],end=" & ",file=f)
        print(bef[-1]+"\\\\\\hline",file=f)
        print("\t\\end{tabular}",file=f)
        print("\\end{table}",file=f)
        
        #探索後出力
        print("\\begin{table}[htbp]",file=f)
        print("    \\caption{"+tmp+" 探索後評価値}",file=f)
        print("""\
    \\begin{tabular}{|l|l|l|l|l|l|l|l|l|}\hline
    \multicolumn{1}{|c|}{\\textbf{type}}
    &\multicolumn{1}{|c|}{\\textbf{1st}}
    &\multicolumn{1}{c|}{\\textbf{median}}
    &\multicolumn{1}{c|}{\\textbf{3rd}}
    &\multicolumn{1}{c|}{\\textbf{mean}}
    &\multicolumn{1}{c|}{\\textbf{std}}
    &\multicolumn{1}{c|}{\\textbf{min}}
    &\multicolumn{1}{c|}{\\textbf{max}}\\\\\\hline""",file=f)
        for j in range(8):
            print("\t",end="",file=f)
            for i in range(4):
                print(afts[j][i],end=" & ",file=f)
            for i in range(4,6):
                print(Decimal(afts[j][i]).quantize(Decimal('0.001'),
                    rounding=ROUND_HALF_UP),end=" & ",file=f)
            for i in range(6,len(afts[j])-1):
                print(afts[j][i],end=" & ",file=f)
            print(afts[j][-1]+"\\\\\\hline",file=f)
        print("\t\\end{tabular}",file=f)
        print("\\end{table}",file=f)
        
        #改善率出力
        print("\\begin{table}[htbp]",file=f)
        print("    \\caption{"+tmp+" 改善率(\\%)}",file=f)
        print("""\
    \\begin{tabular}{|l|l|l|l|l|l|l|l|l|}\hline
    \multicolumn{1}{|c|}{\\textbf{type}}
    &\multicolumn{1}{|c|}{\\textbf{1st}}
    &\multicolumn{1}{c|}{\\textbf{median}}
    &\multicolumn{1}{c|}{\\textbf{3rd}}
    &\multicolumn{1}{c|}{\\textbf{mean}}
    &\multicolumn{1}{c|}{\\textbf{std}}
    &\multicolumn{1}{c|}{\\textbf{min}}
    &\multicolumn{1}{c|}{\\textbf{max}}\\\\\\hline""",file=f)
        for j in range(8):
            print("\t",end="",file=f)
            print(imps[j][0],end=" & ",file=f)
            for i in range(1,len(imps[j])-1):
                print(Decimal(imps[j][i]).quantize(Decimal('0.001'),
                    rounding=ROUND_HALF_UP),end=" & ",file=f)
            print(imps[j][-1]+"\\\\\\hline",file=f)
        print("\t\\end{tabular}",file=f)
        print("\\end{table}",file=f)
        
        #探索時間出力
        print("\\begin{table}[htbp]",file=f)
        print("    \\caption{"+tmp+" 探索時間(ms)}",file=f)
        print("""\
    \\begin{tabular}{|l|l|l|l|l|l|l|l|l|}\hline
    \multicolumn{1}{|c|}{\\textbf{type}}
    &\multicolumn{1}{|c|}{\\textbf{1st}}
    &\multicolumn{1}{c|}{\\textbf{median}}
    &\multicolumn{1}{c|}{\\textbf{3rd}}
    &\multicolumn{1}{c|}{\\textbf{mean}}
    &\multicolumn{1}{c|}{\\textbf{std}}
    &\multicolumn{1}{c|}{\\textbf{min}}
    &\multicolumn{1}{c|}{\\textbf{max}}\\\\\\hline""",file=f)
        for j in range(8):
            print("\t",end="",file=f)
            for i in range(4):
                print(msecs[j][i],end=" & ",file=f)
            for i in range(4,6):
                print(Decimal(msecs[j][i]).quantize(Decimal('0.001'),
                    rounding=ROUND_HALF_UP),end=" & ",file=f)
            for i in range(6,len(msecs[j])-1):
                print(msecs[j][i],end=" & ",file=f)
            print(msecs[j][-1]+"\\\\\\hline",file=f)
        print("\t\\end{tabular}",file=f)
        print("\\end{table}",file=f)

texs=os.listdir(out_dir)
#all.texを除外する
texs=[i for i in texs if "all" not in i]
tmp=texs[:2]
texs=texs[2:]
texs=texs+tmp[::-1]
#print(texs)
all_file_name=out_dir+"all.tex"
with open(all_file_name,"w") as out:
    for tex in texs:   
        with open(out_dir+tex) as f:
            out.writelines(f.readlines())