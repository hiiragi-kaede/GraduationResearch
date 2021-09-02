import matplotlib.pyplot as plt
import itertools

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

def insert_construct(dis_mat,truck_size,size):
    #デポから出発して戻ってくるルートを初期化しておく
    orders=[[0,0] for _ in range(truck_size)]
    
    #とりあえず添え字順に各トラックの種顧客を設定
    for i in range(truck_size):
        #デポの分だけ添字をずらしておく
        orders[i].insert(1,i+1)
    
    #デポの分だけ添字がずれているので、トラックが5台なら添字の始まりは6になる
    for i in range(truck_size+1,size):
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

def saving_construct(dis_mat,truck_size,size):
    #デポから顧客1人だけを訪問するルートで初期化
    orders=[[0,i,0] for i in range(1,size)]

    #他のルートに繋げられていないトラックの添字集合
    firsts=[i for i in range(1,size)]
    ends=[i for i in range(1,size)]
    #繋げられてルートの最初もしくは最後でなくなった添字の集合
    not_first=[]
    not_end=[]
    #トラックの台数が限度の台数に減るまで繰り返し
    while len(firsts)>truck_size:
        firsts=[i for i in range(1,size) if i not in not_first]
        ends=[i for i in range(1,size) if i not in not_end]
        #print(firsts,ends)
        i,j=ends[0],firsts[0]
        max_dis=dis_mat[i][0]+dis_mat[0][j]-dis_mat[i][j]
        max_ids=[i,j]

        for i in ends:
            for j in firsts:
                if i==j: continue
                distance=dis_mat[i][0]+dis_mat[0][j]-dis_mat[i][j]
                if distance>max_dis:
                    max_dis=distance
                    max_ids=[i,j]
        
        i,j=max_ids
        #iを訪問してからデポに帰るルートとjを最初に訪問するルートを併合
        #print(orders[i],orders[j])
        orders[i][:-1].extend(orders[j][1:])
        not_first.append(j+1)
        not_end.append(i+1)
        #print(not_first,not_end)
    
    return [orders[i] for i in firsts]