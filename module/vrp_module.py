import matplotlib.pyplot as plt

def dis(a,b):
    return ((a[0]-b[0])**2+(a[1]-b[1])**2)**.5

def draw_graphs(data,orders):
    """各トラックの経路表示

    Args:
        data ([list[list]]): 各点のx,y座標のリスト(0の点がデポとなる)
        orders ([list[list]]): 各トラックの点の訪問順を示すリスト
    
    ※トラック数の上限は6
    """    

    if len(orders)>6:
        print("too many trucks")
        exit(1)
    
    xs=[i[0] for i in data]
    ys=[i[1] for i in data]
    plt.scatter(xs,ys)

    colors=["k","b","y","c","r","m"]
    for idx,order in enumerate(orders):
        xs=[data[i][0] for i in order]
        ys=[data[i][1] for i in order]
        xs.append(xs[0])
        ys.append(ys[0])

        label="truck"+str(idx+1)
        plt.plot(xs,ys,color=colors[idx],label=label)
    
    plt.scatter(data[0][0],data[0][1],label="depo")
    plt.legend()
    plt.show()

def two_opt_method(dis_mat,order):
    #この時点でデポに帰ってくるルートも含まれている前提で実装されている
    size=len(order)
    while True:
        count = 0
        for i in range(size-2):
            i_next=i+1
            for j in range(i+2,size-1):
                j_next=(j+1)%size

                l1 = dis_mat[order[i]][order[i_next]]
                l2 = dis_mat[order[j]][order[j_next]]
                l3 = dis_mat[order[i]][order[j]]
                l4 = dis_mat[order[i_next]][order[j_next]]

                if l1+l2 > l3+l4:
                    new_root = order[i_next:j+1]
                    order[i_next:j+1]=new_root[::-1]
                    count+=1

        if count==0: break
    
    return order

def insert_construct(dis_mat,orders,size):
    truck_size=len(orders)
    #とりあえず添え字順に各トラックの種顧客を設定
    for i in range(truck_size):
        orders[i].insert(1,i)
    
    for i in range(truck_size,size):
        truck_id=0
        #insertは0を渡すと先頭に追加するので挿入位置は1で初期化
        ins_id=1
        min_dis=dis_mat[orders[truck_id][ins_id-1]][i]+dis_mat[i][orders[truck_id][ins_id]]
        for truck in range(truck_size):
            for order in range(1,len(orders[truck])):
                cur_dis=dis_mat[orders[truck][order-1]][i]+dis_mat[i][orders[truck][order]]
                if cur_dis<min_dis:
                    min_dis=cur_dis
                    ins_id=order
                    truck_id=truck
        
        orders[truck_id].insert(ins_id,i)
    
    return orders