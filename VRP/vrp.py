import sys
sys.path.append("../")
from module import vrp_module as mod
import pprint

#fname="rand100.txt"
fname="rand100_weight.txt"

###main function###
with open(fname) as f:
    if fname!="a280_use.txt":
        data=[list(map(float,i.split())) for i in f.readlines()]
    else:
        data=[list(map(float,i.split()[1:])) for i in f.readlines()]

    dis_mat=[[mod.dis(data[i],data[j]) for i in range(len(data))] for j in range(len(data))]
    size=len(data)
    #トラックの台数
    TRUCK_SIZE=5
    #トラック1台の容量
    TRUCK_CAPACITY=300

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

    for i in range(TRUCK_SIZE):
        mod.two_opt_method(dis_mat,orders[i])
    
    if state==0:
        mod.draw_graphs(data,orders,title="挿入法構築")
    elif state==1:
        mod.draw_graphs(data,orders,title="セービング法構築")
    elif state==2:
        mod.draw_graphs(data,orders,title="k-means法構築")
    
    mod.or_opt_method(data,dis_mat,orders,TRUCK_CAPACITY)

    mod.draw_graphs(data,orders,title="or-opt法実行後")