import os
from decimal import Decimal,ROUND_HALF_UP

file_path="./info"

files=os.listdir(file_path)
files=[i for i in files if ".txt" in i]
tmp=files[:2]
files=files[2:]+tmp[::-1]

out_dir=file_path+"/tex_out/"
compare_fname="compare.tex"
with open(out_dir+compare_fname,"w") as f:
    print("\\begin{table}[htbp]",file=f)
    print("\t\\centering",file=f)
    print("    \\caption{通常手法との比較}",file=f)
    print("""\
    \\begin{tabular}{|l|l|l|l|l|}\hline
    \multicolumn{1}{|c|}{\\textbf{data}}
    &\multicolumn{1}{|c|}{\\textbf{type}}
    &\multicolumn{1}{c|}{\\textbf{ms}}
    &\multicolumn{1}{c|}{\\textbf{after}}
    &\multicolumn{1}{c|}{\\textbf{improve}}\\\\\\hline""",file=f)

for cnt,file in enumerate(files):
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
        fname=file[:-4]
        #texではアンダーバーは特殊文字なので、ファイル名に含まれる場合はエスケープ処理
        if "_" in fname: fname=fname.replace("_","\_")
        print("    \\caption{"+fname+" 探索前評価値}",file=f)
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
        print("    \\caption{"+fname+" 探索後評価値}",file=f)
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
        print("    \\caption{"+fname+" 改善率(\\%)}",file=f)
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
        print("    \\caption{"+fname+" 探索時間(ms)}",file=f)
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
    if(cnt!=0 and cnt%4==0):
        with open(out_dir+compare_fname,"a") as f:
            print("\\begin{table}[htbp]",file=f)
            print("\t\\centering",file=f)
            print("""\
            \\begin{tabular}{|l|l|l|l|l|}\hline
            \multicolumn{1}{|c|}{\\textbf{data}}
            &\multicolumn{1}{|c|}{\\textbf{type}}
            &\multicolumn{1}{c|}{\\textbf{ms}}
            &\multicolumn{1}{c|}{\\textbf{after}}
            &\multicolumn{1}{c|}{\\textbf{improve}}\\\\\\hline""",file=f)
    
    with open(out_dir+compare_fname,"a") as f:
        normal_ms_ave=float(msecs[0][4])
        normal_aft_cos_ave=float(afts[0][4])
        normal_imp_rate_ave=float(imps[0][4])
        
        ms_scores,aft_scores,imp_scores=[],[],[]
        for i in range(1,8):
            ms=Decimal((float(msecs[i][4])-normal_ms_ave)/normal_ms_ave*100).quantize(Decimal('0.001')
                        ,rounding=ROUND_HALF_UP)
            aft=Decimal((float(afts[i][4])-normal_aft_cos_ave)/normal_aft_cos_ave*100).quantize(Decimal('0.001')
                        ,rounding=ROUND_HALF_UP)
            imp=Decimal((float(imps[i][4])-normal_imp_rate_ave)/normal_imp_rate_ave*100).quantize(Decimal('0.001')
                        ,rounding=ROUND_HALF_UP)
            ms,aft,imp=float(ms),float(aft),float(imp)
            ms_scores.append(ms)
            aft_scores.append(aft)
            imp_scores.append(imp)
        
        min_ms,min_aft,max_imp = min(ms_scores),min(aft_scores),max(imp_scores)
        print("\t"+fname,end="",file=f)
        for i in range(1,7):
            if i!=1: print("\t",end="",file=f)
            print("& "+msecs[i][0],end=" & ",file=f)
            
            ms_txt=str(ms_scores[i-1]) if abs(ms_scores[i-1]-min_ms)>1e-2 else "\\bm{"+str(ms_scores[i-1])+"}"
            aft_txt=str(aft_scores[i-1]) if abs(aft_scores[i-1]-min_aft)>1e-2 else "\\bm{"+str(aft_scores[i-1])+"}"
            imp_txt=str(imp_scores[i-1]) if abs(imp_scores[i-1]-max_imp)>1e-2 else "\\bm{"+str(imp_scores[i-1])+"}"
            
            print(ms_txt,end=" & ",file=f)
            print(aft_txt,end=" & ",file=f)
            print(imp_txt,end="\\\\\\cline{2-5}\n",file=f)
        
        print("\t& "+msecs[7][0],end=" & ",file=f)
        ms_txt=str(ms_scores[6]) if abs(ms_scores[6]-min_ms)>1e-2 else "\\bm{"+str(ms_scores[6])+"}"
        aft_txt=str(aft_scores[6]) if abs(aft_scores[6]-min_aft)>1e-2 else "\\bm{"+str(aft_scores[6])+"}"
        imp_txt=str(imp_scores[6]) if abs(imp_scores[6]-max_imp)>1e-2 else "\\bm{"+str(imp_scores[6])+"}"
        print(ms_txt,end=" & ",file=f)
        print(aft_txt,end=" & ",file=f)
        print(imp_txt,end="\\\\\\hline\n",file=f)
    
    if((cnt+1)%4==0):
        with open(out_dir+compare_fname,"a") as f:
            print("\t\\end{tabular}",file=f)
            print("\\end{table}",file=f)

if(len(files)%4!=0):
    with open(out_dir+compare_fname,"a") as f:
        print("\t\\end{tabular}",file=f)
        print("\\end{table}",file=f)

texs=os.listdir(out_dir)
#all.tex,compare.texを除外する
texs=[i for i in texs if "all" not in i and "compare" not in i]
tmp=texs[:2]
texs=texs[2:]
texs=texs+tmp[::-1]
all_file_name=out_dir+"all.tex"
with open(all_file_name,"w") as out:
    for tex in texs:   
        with open(out_dir+tex) as f:
            out.writelines(f.readlines())