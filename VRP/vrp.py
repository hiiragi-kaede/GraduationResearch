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

    orders=[[0] for _ in range(5)]
    for i in range(size):
        orders[i%5].append(i)

    for i in range(5):
        orders[i].append(orders[i][0])
        mod.two_opt_method(dis_mat,orders[i])
    
    mod.draw_nx_graphs(data,orders)