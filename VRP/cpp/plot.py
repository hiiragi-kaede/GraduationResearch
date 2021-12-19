import sys
sys.path.append("../../")
from module import vrp_module as mod
from pprint import pprint
import matplotlib.pyplot as plt

fname=input()
with open(fname) as f:
    tmp=f.readlines()
    data=[list(map(int,i.split())) for i in tmp]

order_fname=input()    
with open(order_fname) as f:
    tmp=f.readlines();
    orders=[list(map(int,i.split())) for i in tmp]
    
    vrp_data=[{"x":i[0],"y":i[1],"weight":i[2]} for i in data]
    # for id in range(len(orders)):
    #     order=orders[id]
    #     xs=[vrp_data[i]["x"] for i in order]
    #     ys=[vrp_data[i]["y"] for i in order]
    #     for i in range(len(xs)):
    #         plt.scatter(xs[i],ys[i])
    #         plt.annotate(str(i),xy=(xs[i],ys[i]))
    #     xs.append(xs[0])
    #     ys.append(ys[0])
    #     plt.plot(xs,ys)
    #     plt.title("truck "+str(id))
    #     plt.show()
    #     plt.clf()
    mod.draw_graphs(vrp_data,orders,title="cpp_insert")
