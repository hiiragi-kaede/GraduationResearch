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

    order=[i for i in range(size)]
    order.append(0)

    mod.draw_graph(data,order)