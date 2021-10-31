import matplotlib.pyplot as plt
import matplotlib.animation as animation

def dis(a,b):
    return ((a["x"]-b["x"])**2+(a["y"]-b["y"])**2)**.5

def check_unvisit(data,orders):
    """訪問していない顧客を確認する

    Args:
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        orders ([list[list]]): 各トラックの点の訪問順を示すリスト\\
    """    
    customers=[False for i in range(len(data))]
    for order in orders:
        for i in order:
            customers[i]=True
    
    tmp=[i for i in range(len(customers)) if not customers[i]]
    if len(tmp)==0: return
    print("unvisit:",tmp,end="   ")
    print("total weight:",sum([data[i]["weight"] for i in range(len(customers)) if not customers[i]]))

def show_route_dif(data,old,new,TRUCK_CAPACITY):
    """改善型解法により解がどの程度改善されたかを表示する。

    Args:
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        old ([list[list]]): トラックたちの訪問順のリスト\\
        new ([list[list]]): トラックたちの訪問順のリスト\\
        TRUCK_CAPACITY ([int]): トラックの容量制限
    """    
    size=len(new)
    for id in range(size):
        old_weight=sum([data[i]["weight"] for i in old[id]])
        old_size=len(old[id])

        new_weight=sum([data[i]["weight"] for i in new[id]])
        new_size=len(new[id])
        print("truck "+str(id+1).zfill(2)+"  size:"+str(old_size).zfill(2)+\
            " → "+str(new_size).zfill(2),end="    ")
        print("weight:"+str(old_weight)+" → "+str(new_weight)+" /"+str(TRUCK_CAPACITY))

def calc_total_weight(data,order):
    return sum([data[i]["weight"] for i in order])

def calc_total_dis(dis_mat,orders):
    total=0
    for ord in orders:
        for i in range(len(ord)-1):
            total+=dis_mat[ord[i]][ord[i+1]]
    return total

def gif_init():
    return [plt.figure(),[]]

def gif_append(data,orders,ims):
    im=[]
    cycle = plt.rcParams['axes.prop_cycle'].by_key()['color']
    for idx,order in enumerate(orders):
        if len(order)<=2: continue
        xs=[data[i]["x"] for i in order]
        ys=[data[i]["y"] for i in order]
        xs.append(xs[0])
        ys.append(ys[0])

        frame,=plt.plot(xs,ys,color=cycle[idx])
        im.append(frame)
    ims.append(im)

def save_gif(fig,ims,title="default.gif"):
    file_dir="out/"
    title=file_dir+title
    ani = animation.ArtistAnimation(fig,ims,interval=300,repeat_delay=5000)
    ani.save(title,writer="pillow")