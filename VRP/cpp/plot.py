import sys
sys.path.append("../../")
from module import vrp_module as mod
from pprint import pprint

fname=input()
with open(fname) as f:
    tmp=f.readlines()
    data=[list(map(int,i.split())) for i in tmp]

order_fname=input()    
with open(order_fname) as f:
    tmp=f.readlines();
    orders=[list(map(int,i.split())) for i in tmp]
    
    vrp_data=[{"x":i[0],"y":i[1],"weight":i[2]} for i in data]
    mod.draw_graphs(vrp_data,orders,title="cpp_insert")
