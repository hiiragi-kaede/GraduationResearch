import sys
sys.path.append("../")
from module import vrp_module as mod
from module import utility as util
from pprint import pprint
import matplotlib.pyplot as plt
import math
import copy
import time

#fname="rand100_weight.txt"
fname="CVRP_Data/tsplib/eilA101.txt"
#fname="CVRP_Data/vrplib/X-n228-k23.txt"

#トラックの台数
TRUCK_SIZE=10

#データにこれらの情報がある場合は使われない
#トラック1台の容量
TRUCK_CAPACITY=300

data=[]
###main function###
with open(fname) as f:
    if fname=="rand100_weight.txt":
        tmp=[list(map(float,i.split())) for i in f.readlines()]
        data=[{"x":i[0],"y":i[1],"weight":i[2]} for i in tmp]
    else:
        size=int(f.readline())
        TRUCK_CAPACITY=int(f.readline())
        for i in range(size):
            tmp=list(map(float,f.readline().split()[1:]))
            data.append({"x":tmp[0],"y":tmp[1]})
        
        #DEMAND_SECTIONの行を取り除く
        _=f.readline()
        for i in range(size):
            buf=f.readline().split()
            data[i]["weight"]=int(buf[1])

    print("data file:",fname)
    total_weight=sum([i["weight"] for i in data])
    print("total weight:",total_weight)
    TRUCK_SIZE=math.ceil(total_weight/TRUCK_CAPACITY)
    print("truck size:",TRUCK_SIZE)
    dis_mat=[[util.dis(data[i],data[j]) for i in range(len(data))] for j in range(len(data))]
    size=len(data)

    print("初期解構築法を選択してください")
    print("0:挿入法")
    print("1:セービング法")
    print("2:k-means法 + random-2opt")
    state=int(input())
    if state==0:
        orders=mod.insert_construct(dis_mat,TRUCK_SIZE,TRUCK_CAPACITY,data,size)
    elif state==1:
        orders=mod.saving_construct(dis_mat,TRUCK_SIZE,TRUCK_CAPACITY,data,size)
    elif state==2:
        orders=mod.kmeans(data,TRUCK_SIZE,TRUCK_CAPACITY)
    else:
        print("正しい数字を入力してください")
        exit(1)
    
    print("局所探索法を選択してください")
    print("0:Or-opt法")
    print("1:2-opt*法")
    print("2:クロス交換法")
    l_state=int(input())
    
    print("局所探索法の過程のgifを保存します。")
    print("出力ファイル名を入力してください(out/につづくディレクトリ名を入力してください)")
    print("無入力でスキップします")
    f_title=input()

    for i in range(TRUCK_SIZE):
        mod.two_opt_method(dis_mat,orders[i])
    if state==0:
        mod.draw_graphs(data,orders,title="挿入法構築")
    elif state==1:
        mod.draw_graphs(data,orders,title="セービング法構築")
    elif state==2:
        mod.draw_graphs(data,orders,title="k-means法構築")
    
    old=copy.deepcopy(orders)
    st=time.time()
    if l_state==0:
        mod.or_opt_method(data,dis_mat,orders,TRUCK_CAPACITY,f_title)
        mod.draw_graphs(data,orders,title="or-opt法実行後")
    elif l_state==1:
        mod.twp_opt_asterisk_method(data,dis_mat,orders,TRUCK_CAPACITY,f_title)
        mod.draw_graphs(data,orders,title="2-opt*法実行後")
    elif l_state==2:
        mod.cross_exchange_method(data,dis_mat,orders,TRUCK_CAPACITY,f_title)
        mod.draw_graphs(data,orders,title="クロス交換法実行後")
    else:
        print("正しい数字を入力してください")
        exit(1)
        
    end=time.time()
    print("elapsed time:",(end-st),"s")
    
    util.show_route_dif(data,old,orders,TRUCK_CAPACITY)
    print("total move cost:"+str(util.calc_total_dis(dis_mat,old))+\
        " → "+str(util.calc_total_dis(dis_mat,orders)))