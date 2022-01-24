import sys
sys.path.append("../../")
from module import vrp_module as mod
from pprint import pprint
import matplotlib.pyplot as plt

def make_grid(data):
    xs=[i[0] for i in data]
    ys=[i[1] for i in data]
    max_x=max(xs)
    min_x=min(xs)
    max_y=max(ys)
    min_y=min(ys)
    xs=[(i-min_x)/(max_x-min_x) for i in xs]
    ys=[(i-min_y)/(max_y-min_y) for i in ys]
    ticks=[i*0.1 for i in range(11)]
    plt.xticks(ticks)
    plt.yticks(ticks)

    plt.scatter(xs,ys)
    plt.grid()
    plt.xlim(min(xs),max(xs))
    plt.ylim(min(ys),max(ys))
    plt.show()
    plt.clf()



fname=input()
with open(fname) as f:
    tmp=f.readlines()
    data=[list(map(int,i.split())) for i in tmp]

make_grid(data)

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
    mod.draw_graphs(vrp_data,orders,title="truck routes")
