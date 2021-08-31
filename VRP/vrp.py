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
    truck_size=3

    #デポから出発して戻ってくるルートを初期化しておく
    
    orders=[[0,0] for _ in range(truck_size)]
    orders=mod.insert_construct(dis_mat,orders,size)

    for i in range(truck_size):
        mod.two_opt_method(dis_mat,orders[i])
    
    mod.draw_nx_graphs(data,orders)