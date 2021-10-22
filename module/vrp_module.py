import matplotlib.pyplot as plt
import matplotlib
import itertools
from sklearn.cluster import KMeans
import numpy as np

def dis(a,b):
    return ((a["x"]-b["x"])**2+(a["y"]-b["y"])**2)**.5

def draw_graphs(data,orders,title="default_title"):
    """各トラックの経路表示

    Args:
        data ([list[list]]): 各点のx,y座標のリスト(0の点がデポとなる)
        orders ([list[list]]): 各トラックの点の訪問順を示すリスト
    
    ※トラック数の上限は15
    """    

    markers=[".",",","o","v","^","<",">","8","s","p","P","*","h","x","D"]
    if len(orders)>len(markers):
        print("too many trucks")
        exit(1)

    
    for idx,order in enumerate(orders):
        if len(order)<=2: continue
        xs=[data[i]["x"] for i in order]
        ys=[data[i]["y"] for i in order]
        plt.scatter(xs,ys,marker=markers[idx])
        xs.append(xs[0])
        ys.append(ys[0])

        label="truck"+str(idx+1)
        plt.plot(xs,ys,label=label)
    
    plt.scatter(data[0]["x"],data[0]["y"],label="depo")
    plt.title(title,fontname="MS Gothic")
    plt.legend()
    plt.show()

def two_opt_method(dis_mat,order):
    """orderに渡されたルートを2opt法を用いて最適化する

    Args:
        dis_mat ([list[list]]): 各都市間の距離行列
        order ([list]): 都市の訪問順のリスト

    Returns:
        [list]: 2optによって最適化されたorder
    """    
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

def or_opt_method(data,dis_mat,orders,TRUCK_CAPACITY):
    idxs=[i for i in range(len(orders))]
    weights=[0 for _ in range(len(orders))]
    #重さの初期化
    for i in range(len(orders)):
        for j in range(1,len(orders[i])-1):
            weights[i]+=data[orders[i][j]]["weight"]

    while True:
        isBreak=False
        #トラック番号をi,jで管理。iの一部をjに挿入する形
        for v in itertools.combinations(idxs,2):
            if isBreak: break
            i,j=v
            for b in range(1,len(orders[i])-2):
                if isBreak: break
                ord=orders[i]
                bef_score=-dis_mat[ord[b-1]][ord[b]]-dis_mat[ord[b+1]][ord[b+2]]+dis_mat[ord[b-1]][ord[b+2]]
                change_weight=data[ord[b]]["weight"]+data[ord[b+1]]["weight"]
                for e in range(len(orders[j])-1):
                    if isBreak: break
                    to_ord=orders[j]
                    aft_score=dis_mat[to_ord[e]][ord[b]]+dis_mat[to_ord[e+1]][ord[b+1]]-dis_mat[to_ord[e]][to_ord[e+1]]

                    #他のルートに挿入すると総移動距離が短くなるとき
                    if bef_score+aft_score<0:
                        #挿入先の容量成約を満たしているならば
                        if weights[j]+change_weight<TRUCK_CAPACITY:
                            weights[i]-=change_weight
                            weights[j]+=change_weight

                            orders[j]=orders[j][0:e]+orders[i][b:b+2]+orders[j][e:]
                            orders[i]=orders[i][0:b]+orders[i][b+2:]
                            isBreak=True
                            break
        
        #一度or-optが終わるたびに2opt法を実行する
        for i in range(len(orders)):
            two_opt_method(dis_mat,orders[i])
        
        #変更がなくなったらループを終了
        if not isBreak: break

    print("finish or-opt")              
    #show_truck_cap(weights,TRUCK_CAPACITY)
    return orders

def insert_construct(dis_mat,truck_size,TRUCK_CAPACITY,data,size):
    """挿入法による初期解構築

    Args:
        dis_mat ([list[list]]): 各都市間の距離行列
        truck_size ([int]): トラックの台数
        TRUCK_CAPACITY ([int]): トラックの容量制限
        data ([list[list]]): 各点のx,y座標のリスト(0の点がデポとなる)
        size ([int]): デポを含んだ都市の大きさ

    Returns:
        [list[list]]: 各トラックの訪問する都市の順番のリスト
    """

    #デポから出発して戻ってくるルートを初期化しておく
    orders=[[0,0] for _ in range(truck_size)]
    weights=[0 for _ in range(truck_size)]
    
    #とりあえず添え字順に各トラックの種顧客を設定
    for i in range(truck_size):
        #デポの分だけ添字をずらしておく
        orders[i].insert(1,i+1)
        weights[i]+=data[i+1]["weight"]
    
    #デポの分だけ添字がずれているので、トラックが5台なら添字の始まりは6になる
    for i in range(truck_size+1,size):
        truck_id=0
        #insertは0を渡すと先頭に追加するので挿入位置は1で初期化
        ins_id=1
        min_dis=dis_mat[orders[truck_id][ins_id-1]][i]+dis_mat[i][orders[truck_id][ins_id]]
        for truck in range(truck_size):
            #トラックの容量を超えているならば挿入位置を調べることはしない
            if weights[truck]+data[i]["weight"]>TRUCK_CAPACITY:
                continue

            for order in range(1,len(orders[truck])):
                cur_dis=dis_mat[orders[truck][order-1]][i]+dis_mat[i][orders[truck][order]]
                if cur_dis<min_dis:
                    min_dis=cur_dis
                    ins_id=order
                    truck_id=truck
        
        orders[truck_id].insert(ins_id,i)
        weights[truck_id]+=data[i]["weight"]
    
    #show_truck_cap(weights,TRUCK_CAPACITY)
    #check_unvisit(data,orders)
    return orders

def saving_construct(dis_mat,truck_size,TRUCK_CAPACITY,data,size):
    """セービング法による初期解構築

    Args:
        dis_mat ([list[list]]): 各都市間の距離行列
        truck_size ([int]): トラックの台数
        size ([int]): デポを含んだ都市の大きさ

    Returns:
        [list[list]]: 各トラックの訪問する都市の順番のリスト
    """    
    #デポから顧客1人だけを訪問するルートで初期化
    orders=[[0,i,0] for i in range(1,size)]
    weights=[i["weight"] for i in data]

    #他のルートに繋げられていないトラックの添字集合
    valid=[i for i in range(1,size)]
    #繋げられてルートの最初でなくなった添字の集合
    invalid=[]

    #トラックの台数が限度の台数に減るまで繰り返し
    while True:
        valid=[i for i in range(1,size) if i not in invalid]
        if len(valid)==truck_size: break

        i,j=valid[0],valid[1]
        max_dis=dis_mat[i][0]+dis_mat[0][j]-dis_mat[i][j]
        max_ids=[i,j]

        for v in list(itertools.combinations(valid,2)):
            i,j=v
            if weights[i]+weights[j]>TRUCK_CAPACITY:
                continue
            distance=dis_mat[i][0]+dis_mat[0][j]-dis_mat[i][j]
            if distance>max_dis:
                max_dis=distance
                max_ids=[i,j]
        
        i,j=max_ids
        #iを訪問してからデポに帰るルートとjを最初に訪問するルートを併合
        #デポ分添字がずれてしまっているので、ordersに対しては添字を-1する必要がある
        orders[i-1]=orders[i-1][:-1]+orders[j-1][1:]
        invalid.append(j)
        weights[i]+=weights[j]
    
    aft_weights=[weights[i] for i in valid]
    #show_truck_cap(aft_weights,TRUCK_CAPACITY)
    #check_unvisit(data,orders)
    return [orders[i-1] for i in valid]

def kmeans(data,truck_size,TRUCK_CAPACITY):
    """dataをtruck_sizeのクラスターに分割し、各クラスターの添字をordersに格納して返す

    Args:
        data ([list[list]]): 各点のx,y座標のリスト(0の点がデポとなる)
        truck_size ([int]): トラックの台数

    Returns:
        orders ([list[list]]): 各クラスターに含まれる顧客の添字のリスト
    """

    #重さ情報はクラスタリング作業に影響を与えてしまうため取り去ってしまう
    tmp_data=[[i["x"],i["y"]] for i in data]
    x = np.array(tmp_data[1:])
    model = KMeans(n_clusters=truck_size).fit(x)
    labels = model.labels_

    orders=[[0] for _ in range(truck_size)]
    weights=[0 for _ in range(truck_size)]
    for i in range(len(labels)):
        if weights[labels[i]]+data[i+1]["weight"]>TRUCK_CAPACITY:
            continue
        orders[labels[i]].append(i+1)
        weights[labels[i]]+=data[i+1]["weight"]
    for i in orders:
        i.append(0)
    
    #show_truck_cap(weights,TRUCK_CAPACITY)
    #check_unvisit(data,orders)
    #print(orders)
    return orders

def show_truck_cap(weights,TRUCK_CAPACITY):
    print("Truck Capacity:",TRUCK_CAPACITY)
    for i in range(len(weights)):
        print("weight",i+1,":",weights[i])
    print("total weight:",sum(weights))

def check_unvisit(data,orders):
    customers=[False for i in range(len(data))]
    for order in orders:
        for i in order:
            customers[i]=True
    print("unvisit:",[i for i in range(len(customers)) if not customers[i]])

def route_dif(data,old,new,TRUCK_CAPACITY):
    size=len(new)
    for id in range(size):
        old_weight=sum([data[i]["weight"] for i in old[id]])
        old_size=len(old[id])

        new_weight=sum([data[i]["weight"] for i in new[id]])
        new_size=len(new[id])
        print("truck "+str(id+1)+"  size:"+str(old_size)+" → "+str(new_size),end="    ")
        print("weight:"+str(old_weight)+" → "+str(new_weight)+" /"+str(TRUCK_CAPACITY))