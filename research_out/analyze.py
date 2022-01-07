import os
from decimal import Decimal,ROUND_HALF_UP
from pprint import pprint
from matplotlib.colors import same_color
import matplotlib.pyplot as plt

msec_info,bef_info,aft_info,imp_info=[],[],[],[]

def tex_out(out_dir,compare_fname,files):
    msec_info.clear()
    bef_info.clear()
    aft_info.clear()
    imp_info.clear()
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
            for i in range(8):
                if i==0:
                    tmp=f.readline().split()[1:]
                    tmp[0]=tmp[0][:1]
                    msecs.append(tmp)
                else:
                    msecs.append(f.readline().split()[1:])
            
            for _ in range(3): f.readline()
            bef=f.readline().split()[1:]
            
            for _ in range(3): f.readline()
            afts=[]
            for i in range(8):
                if i==0:
                    tmp=f.readline().split()[1:]
                    tmp[0]=tmp[0][:1]
                    afts.append(tmp)
                else:
                    afts.append(f.readline().split()[1:])
            
            for _ in range(3): f.readline()
            imps=[]
            for i in range(8):
                if i==0:
                    tmp=f.readline().split()[1:]
                    tmp[0]=tmp[0][:1]
                    imps.append(tmp)
                else:
                    imps.append(f.readline().split()[1:])

        msec_info.append([file[:-4],msecs])
        bef_info.append([file[:-4],bef])
        aft_info.append([file[:-4],afts])
        imp_info.append([file[:-4],imps])
        
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
                ms=Decimal((float(msecs[i][4]))/normal_ms_ave*100).quantize(Decimal('0.001')
                            ,rounding=ROUND_HALF_UP)
                aft=Decimal((float(afts[i][4]))/normal_aft_cos_ave*100).quantize(Decimal('0.001')
                            ,rounding=ROUND_HALF_UP)
                imp=Decimal((float(imps[i][4]))/normal_imp_rate_ave*100).quantize(Decimal('0.001')
                            ,rounding=ROUND_HALF_UP)
                ms,aft,imp=float(ms),float(aft),float(imp)
                ms_scores.append(ms)
                aft_scores.append(aft)
                imp_scores.append(imp)
                if aft<100: print(file,":",afts[i][0],":",aft,"   win")
            
            min_ms,min_aft,max_imp = min(ms_scores),min(aft_scores),max(imp_scores)
            print("\t"+fname,end="",file=f)
            for i in range(1,7):
                if i!=1: print("\t",end="",file=f)
                print("& "+msecs[i][0],end=" & ",file=f)
                
                ms_txt=str(ms_scores[i-1]) if abs(ms_scores[i-1]-min_ms)>1e-4 else "\\bm{"+str(ms_scores[i-1])+"}"
                aft_txt=str(aft_scores[i-1]) if abs(aft_scores[i-1]-min_aft)>1e-4 else "\\bm{"+str(aft_scores[i-1])+"}"
                imp_txt=str(imp_scores[i-1]) if abs(imp_scores[i-1]-max_imp)>1e-4 else "\\bm{"+str(imp_scores[i-1])+"}"
                
                print(ms_txt,end=" & ",file=f)
                print(aft_txt,end=" & ",file=f)
                print(imp_txt,end="\\\\\\cline{2-5}\n",file=f)
            
            print("\t& "+msecs[7][0],end=" & ",file=f)
            ms_txt=str(ms_scores[6]) if abs(ms_scores[6]-min_ms)>1e-4 else "\\bm{"+str(ms_scores[6])+"}"
            aft_txt=str(aft_scores[6]) if abs(aft_scores[6]-min_aft)>1e-4 else "\\bm{"+str(aft_scores[6])+"}"
            imp_txt=str(imp_scores[6]) if abs(imp_scores[6]-max_imp)>1e-4 else "\\bm{"+str(imp_scores[6])+"}"
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
    texs=sorted(texs)
    #all.tex,compare.tex,rank.texを除外する
    texs=[i for i in texs if "all" not in i and
                            "compare" not in i and
                            "rank" not in i]
    tmp=texs[:2]
    texs=texs[2:]
    texs=texs+tmp[::-1]
    all_file_name=out_dir+"all.tex"
    with open(all_file_name,"w") as out:
        for tex in texs:   
            with open(out_dir+tex) as f:
                out.writelines(f.readlines())

def img_out():
    types=[msec_info[0][1][i][0] for i in range(8)]
    fnames=[]
    msecs,afts,imps=[],[],[]
    for i in range(len(msec_info)):
        fnames.append(msec_info[i][0])
        ms,aft,imp=msec_info[i][1],aft_info[i][1],imp_info[i][1]
        ms_ave=[float(ms[i][4]) for i in range(8)]
        aft_ave=[float(aft[i][4]) for i in range(8)]
        imp_ave=[float(imp[i][4]) for i in range(8)]
        
        msecs.append(ms_ave)
        afts.append(aft_ave)
        imps.append(imp_ave)
    
    #Leuvenを除いて、後から付け足す
    fnames=fnames[:-1]
    data_names=[i.split('-')[1][1:] for i in fnames]
    #data_names.append("3000")
    titles=["ms comparison","after comparison","improve rate comparison"]
    names=["ms.png","after.png","improve.png"]
    save_img(data_names,msecs,types,titles[0],names[0])
    save_img(data_names,afts,types,titles[1],names[1])
    save_img(data_names,imps,types,titles[2],names[2])
    
    
def save_img(data_names,data,types,title,img_name):
    colors=plt.get_cmap("tab10")
    xs=range(len(data_names)//2)
    plt.figure(figsize=(8,6))
    plt.xticks(xs,data_names[:len(data_names)//2])
    plt.xlabel("customer size")
    for i in range(8):
        ys=[data[id][i] for id in range(len(xs))]
        plt.plot(xs,ys,color=colors(i),label=types[i])
    plt.title(title)
    plt.legend()
    plt.savefig("img/comparison/"+img_name)
    plt.clf()
    
    xs=range(len(data_names)-len(data_names)//2)
    plt.figure(figsize=(8,6))
    plt.xticks(xs,data_names[len(data_names)//2:])
    plt.xlabel("customer size")
    for i in range(8):
        ys=[data[id+len(xs)][i] for id in range(len(xs))]
        plt.plot(xs,ys,color=colors(i),label=types[i])
    plt.title(title)
    plt.legend()
    img_name=img_name.replace(".","2.")
    plt.savefig("img/comparison/"+img_name)
    plt.clf()

def tex_ranking(ranking_fname):
    types=[msec_info[0][1][i][0] for i in range(8)]
    fnames=[]
    msecs,afts,imps=[],[],[]
    for i in range(len(msec_info)):
        fnames.append(msec_info[i][0])
        ms,aft,imp=msec_info[i][1],aft_info[i][1],imp_info[i][1]
        ms_ave=[float(ms[i][4]) for i in range(8)]
        aft_ave=[float(aft[i][4]) for i in range(8)]
        imp_ave=[float(imp[i][4]) for i in range(8)]
        
        msecs.append(ms_ave)
        afts.append(aft_ave)
        imps.append(imp_ave)
    
    ms_ranking=[0 for _ in range(8)]
    for i in range(len(msec_info)):
        ids=list(range(8))
        ms_ids=sorted(ids,key=lambda x: msecs[i][x])
        for cnt,id in enumerate(ms_ids): ms_ranking[id]+=(cnt+1)
    ms_ranking=[i/len(msec_info) for i in ms_ranking]
    ms_ranking=[float(Decimal(i).quantize(Decimal('0.01'),rounding=ROUND_HALF_UP))
                for i in ms_ranking]
    
    aft_ranking=[0 for _ in range(8)]
    for i in range(len(msec_info)):
        ids=list(range(8))
        aft_ids=sorted(ids,key=lambda x: afts[i][x])
        for cnt,id in enumerate(aft_ids): aft_ranking[id]+=(cnt+1)
    aft_ranking=[i/len(msec_info) for i in aft_ranking]
    aft_ranking=[float(Decimal(i).quantize(Decimal('0.01'),rounding=ROUND_HALF_UP))
                for i in aft_ranking]
    
    imp_ranking=[0 for _ in range(8)]
    for i in range(len(msec_info)):
        ids=list(range(8))
        imp_ids=sorted(ids,key=lambda x: imps[i][x],reverse=True)
        for cnt,id in enumerate(imp_ids): imp_ranking[id]+=(cnt+1)
    imp_ranking=[i/len(msec_info) for i in imp_ranking]
    imp_ranking=[float(Decimal(i).quantize(Decimal('0.01'),rounding=ROUND_HALF_UP))
                for i in imp_ranking]
    
    with open(ranking_fname,"w") as f:
        print("""\
\\begin{table}[htbp]
    \\caption{各手法の平均ランキング比較}
    \\begin{tabular}{|l|l|l|l|l|l|l|l|l|}\\hline
    \multicolumn{1}{|c|}{}
    &\multicolumn{1}{|c|}{\\textbf{n}}
    &\multicolumn{1}{|c|}{\\textbf{h}}
    &\multicolumn{1}{|c|}{\\textbf{l}}
    &\multicolumn{1}{|c|}{\\textbf{lh}}
    &\multicolumn{1}{|c|}{\\textbf{t}}
    &\multicolumn{1}{|c|}{\\textbf{th}}
    &\multicolumn{1}{|c|}{\\textbf{tl}}
    &\multicolumn{1}{|c|}{\\textbf{tlh}}\\\\\\hline""",file=f)
        print("\tms &",end="",file=f)
        for i in ms_ranking[:-1]:
            text=str(i)
            if i==min(ms_ranking): text="\\textbf{"+text+"}"
            print(text,end=" &",file=f)
        text=str(ms_ranking[-1])
        if ms_ranking[-1]==min(ms_ranking): text="\\textbf{"+text+"}"
        print(text,end="\\\\\\hline\n",file=f)

        print("\tafter cost &",end="",file=f)
        for i in aft_ranking[:-1]:
            text=str(i)
            if i==min(aft_ranking): text="\\textbf{"+text+"}"
            print(text,end=" &",file=f)
        text=str(aft_ranking[-1])
        if aft_ranking[-1]==min(aft_ranking): text="\\textbf{"+text+"}"
        print(text,end="\\\\\\hline\n",file=f)
        
        print("\timprove rate &",end="",file=f)
        for i in imp_ranking[:-1]:
            text=str(i)
            if i==min(imp_ranking): text="\\textbf{"+text+"}"
            print(text,end=" &",file=f)
        text=str(imp_ranking[-1])
        if imp_ranking[-1]==min(imp_ranking): text="\\textbf{"+text+"}"
        print(text,end="\\\\\\hline\n",file=f)
        
        print("\t\\end{tabular}",file=f)
        print("\\end{table}",file=f)


#main
file_path="./info"

files=os.listdir(file_path)
files=sorted(files)
files=[i for i in files if ".txt" in i]
tmp=files[:2]
files=files[2:]+tmp[::-1]

out_dir=file_path+"/tex_out/"
compare_fname="compare.tex"
tex_out(out_dir,compare_fname,files)

#pprint(msec_info,width=120)
img_out()

ranking_fname=out_dir+"rank.tex"
tex_ranking(ranking_fname)