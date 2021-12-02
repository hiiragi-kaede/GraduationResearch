import sys
sys.path.append("../../")
from module import vrp_module as mod
from module import utility as util

data=[]
order_fname=input("binpacking answer file:")    
with open(order_fname) as f:
    data_file=f.readline().split(":")[1]
    #改行文字を取り除く
    data_file=data_file[1:-1]
    with open(data_file) as dataf:
        size=int(dataf.readline())
        TRUCK_CAPACITY=int(dataf.readline())
        for i in range(size):
            tmp=list(map(float,dataf.readline().split()[1:]))
            data.append({"x":tmp[0],"y":tmp[1]})
        
        #DEMAND_SECTIONの行を取り除く
        _=dataf.readline()
        for i in range(size):
            buf=dataf.readline().split()
            data[i]["weight"]=int(buf[1])
    
    #total_weight行は読み飛ばす
    _=f.readline()
    _,truck_size=f.readline().split(":")
    truck_size=int(truck_size)
    
    orders=[]
    for i in range(truck_size):
        tmp=f.readline().split(":")[1]
        tmp=tmp[2:-2]
        datum=list(map(int,tmp.split(",")))
        datum=[0]+datum+[0]
        orders.append(datum)
    
    dis_mat=[[util.dis(data[i],data[j]) for i in range(len(data))] for j in range(len(data))]
    for i in orders: mod.two_opt_method(dis_mat,i)
    
    for i in range(len(orders)):
        print("truck ",i+1,":",orders[i],"    weight:",sum([data[i]["weight"] for i in orders[i]]),"/",TRUCK_CAPACITY)
        
    vrp_data=[{"x":i['x'],"y":i['y'],"weight":i['weight']} for i in data]
    mod.draw_graphs(vrp_data,orders,title="binpacking")
