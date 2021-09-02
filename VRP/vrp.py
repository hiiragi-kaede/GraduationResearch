import sys
sys.path.append("../")
from module import vrp_module as mod
import pprint

fname="rand100.txt"

###main function###
with open(fname) as f:
    if fname!="a280_use.txt":
        data=[list(map(float,i.split())) for i in f.readlines()]
    else:
        data=[list(map(float,i.split()[1:])) for i in f.readlines()]

    dis_mat=[[mod.dis(data[i],data[j]) for i in range(len(data))] for j in range(len(data))]
    size=len(data)
    #トラックの台数
    truck_size=5

    print("初期解構築法を選択してください")
    print("0:挿入法")
    print("1:セービング法")
    state=int(input())
    if state==0:
        orders=mod.insert_construct(dis_mat,truck_size,size)
    elif state==1:
        orders=mod.saving_construct(dis_mat,truck_size,size)
    else:
        print("正しい数字を入力してください")
        exit(1)

    for i in range(truck_size):
        mod.two_opt_method(dis_mat,orders[i])
    
    mod.draw_graphs(data,orders)