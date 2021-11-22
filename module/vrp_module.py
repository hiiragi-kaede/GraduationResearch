import matplotlib.pyplot as plt
import matplotlib
import itertools
from sklearn.cluster import KMeans
import numpy as np
from module import utility as util
import time

TIME_LIMIT=300

def draw_graphs(data,orders,title="default_title"):
    """各トラックの経路表示

    Args:
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        orders ([list[list]]): 各トラックの点の訪問順を示すリスト\\
        title ([string]): グラフのタイトル。指定しなければ"default_title"になる。
    
    ※トラック数が15を超えるとポイントの形状がすべて統一に
    """    

    markers=[".",",","o","v","^","<",">","8","s","p","P","*","h","x","D"]
    for idx,order in enumerate(orders):
        if len(order)<=2: continue
        xs=[data[i]["x"] for i in order]
        ys=[data[i]["y"] for i in order]
        if len(orders)>len(markers):
            plt.scatter(xs,ys)
        else:
            plt.scatter(xs,ys,marker=markers[idx])
        xs.append(xs[0])
        ys.append(ys[0])

        label="truck"+str(idx+1)
        plt.plot(xs,ys,label=label)
    
    util.check_unvisit(data,orders)
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

def or_opt_method(data,dis_mat,orders,TRUCK_CAPACITY,f_title):
    """Or-opt法による改善型解法

    Args:
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        dis_mat ([list[list]]): 各都市間の距離行列\\
        orders ([list[list]]): トラックたちの訪問順のリスト\\
        TRUCK_CAPACITY ([int]): トラックの容量制限

    Returns:
        fig,ims: gifを保存するための情報を返す
    """
    idxs,weights=local_search_init(data,orders)
    is_save_gif=f_title!=""
    fig,ims=util.gif_init()
    plt.title("or-opt近傍探索",fontname="MS Gothic")
    st=time.time()
    
    while True:
        if time.time()-st>TIME_LIMIT: break
        isBreak=False
        #トラック番号をi,jで管理。iの一部をjに挿入する形
        for v in itertools.combinations(idxs,2):
            if isBreak: break
            i,j=v
            for b in range(1,len(orders[i])-2):
                if isBreak: break
                
                for e in range(len(orders[j])-1):
                    if isBreak: break

                    isBreak = or_opt_change(orders,weights,dis_mat,TRUCK_CAPACITY,i,j,b,e,data,ims,is_save_gif)
        
        #一度or-optが終わるたびに2opt法を実行する
        for i in range(len(orders)):
            two_opt_method(dis_mat,orders[i])
        
        #変更がなくなったらループを終了
        if not isBreak: break

    print("finish or-opt")
    if is_save_gif:
        util.save_gif(fig,ims,title=f_title)

def or_opt_change(orders,weights,dis_mat,TRUCK_CAPACITY,i,j,b,e,data,ims,is_save_gif):
    """Or-opt法のサブルーチン。変更後の距離を比較して更新されたかどうかのbool値を返す。

    Args:
        orders ([list[list]]): トラックたちの訪問順のリスト\\
        weights ([list[int]]): 各トラックの合計重量\\
        dis_mat ([list[list]]): 各都市間の距離行列\\
        TRUCK_CAPACITY ([int]): トラックの容量制限\\
        i ([int]): ルートを抜かれるトラックの添字   
        j ([int]): ルートを入れられるトラックの添字\\
        b ([int]): 抜かれるルートの一人目の顧客の添字   
        e ([int]): 入れられるルートの挿入する直前の顧客の添字\\
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        ims ([list]): gif出力用の画像を保存するリスト

    Returns:
        [bool]: 変更があればTrue、なければFalseを返す
    """    
    isBreak=False
    ord=orders[i]
    bef_score=-dis_mat[ord[b-1]][ord[b]]-dis_mat[ord[b+1]][ord[b+2]]+dis_mat[ord[b-1]][ord[b+2]]
    change_weight=data[ord[b]]["weight"]+data[ord[b+1]]["weight"]
    
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
            if is_save_gif:
                util.gif_append(data,orders,ims)
            
    return isBreak

def twp_opt_asterisk_method(data,dis_mat,orders,TRUCK_CAPACITY,f_title):  
    """2-opt*法による改善型解法

    Args:
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        dis_mat ([list[list]]): 各都市間の距離行列\\
        orders ([list[list]]): トラックたちの訪問順のリスト\\
        TRUCK_CAPACITY ([int]): トラックの容量制限

    Returns:
        fig,ims: gifを保存するための情報を返す
    """
    idxs,weights=local_search_init(data,orders)
    is_save_gif=f_title!=""
    fig,ims=util.gif_init()
    plt.title("2-opt*近傍探索",fontname="MS Gothic")
    st=time.time()
    
    while True:
        if time.time()-st>TIME_LIMIT: break
        isBreak=False
        #トラック番号をi,jで管理。iとjのある添字以降を交換する形。
        for v in itertools.combinations(idxs,2):
            if isBreak: break
            i,j=v
            for f_id in reversed(range(1,len(orders[i])-1)):
                if isBreak: break
                fst_ord=orders[i]
                fst_weight=util.calc_total_weight(data,fst_ord[f_id:])
                
                for s_id in reversed(range(1,len(orders[j])-1)):
                    if isBreak: break
                    sec_ord=orders[j]
                    sec_weight=util.calc_total_weight(data,sec_ord[s_id:])
                    
                    #ルートを交換した場合容量オーバーなら次のループへ
                    if (util.calc_total_weight(data,fst_ord[:f_id])+sec_weight > TRUCK_CAPACITY\
                    or util.calc_total_weight(data,sec_ord[:s_id])+fst_weight > TRUCK_CAPACITY):
                        continue
                    
                    isBreak=two_opt_asterisk_change(data,dis_mat,orders,fst_ord,f_id,sec_ord,s_id,ims,is_save_gif)
        
        #一度2-opt*が終わるたびに各ルートに2opt法を実行する
        for i in range(len(orders)):
            two_opt_method(dis_mat,orders[i])
        
        #変更がなくなったらループを終了
        if not isBreak: break        
        
        print("\r"+str(util.calc_total_dis(dis_mat,orders)),end="")
    print("\nfinish 2-opt*")
    if(f_title==""):
        pass
    else:
        util.save_gif(fig,ims,title=f_title)

def two_opt_asterisk_change(data,dis_mat,orders,fst_ord,f_id,sec_ord,s_id,ims,is_save_gif):
    isBreak=False
    dif=-dis_mat[fst_ord[f_id-1]][fst_ord[f_id]]\
        -dis_mat[sec_ord[s_id-1]][sec_ord[s_id]]\
        +dis_mat[fst_ord[f_id-1]][sec_ord[s_id]]\
        +dis_mat[sec_ord[s_id-1]][fst_ord[f_id]]
                    
    #ルートを交換する
    if dif<0 and abs(dif)>1e-4:
        tmp=fst_ord[f_id:]
        fst_ord[f_id:]=sec_ord[s_id:]
        sec_ord[s_id:]=tmp
        isBreak=True
        if is_save_gif:
            util.gif_append(data,orders,ims)
    
    return isBreak

def cross_exchange_method(data,dis_mat,orders,TRUCK_CAPACITY,f_title):
    idxs,weights=local_search_init(data,orders)
    is_save_gif=f_title!=""
    fig,ims=util.gif_init()
    plt.title("クロス交換近傍探索",fontname="MS Gothic")
    st=time.process_time()
    
    while True:
        elapsed=time.process_time()-st
        if elapsed>TIME_LIMIT: break
        isBreak=False
        #トラック番号をi,jで管理。iとjの途中のルートを交換する形。
        for v in itertools.combinations(idxs,2):
            if isBreak: break
            i,j=v
            for f_ids in itertools.combinations(range(1,len(orders[i])-1),2):
                if isBreak: break
                fst_ord=orders[i]
                fst_weight=util.calc_total_weight(data,fst_ord[f_ids[0]:f_ids[1]])
                
                for s_ids in itertools.combinations(range(1,len(orders[j])-1),2):
                    if isBreak: break
                    sec_ord=orders[j]
                    sec_weight=util.calc_total_weight(data,sec_ord[s_ids[0]:s_ids[1]])
                    
                    #ルートを交換した場合容量オーバーなら次のループへ
                    if (util.calc_total_weight(data,fst_ord[:f_ids[0]])\
                        +util.calc_total_weight(data,fst_ord[f_ids[1]:])\
                        +sec_weight > TRUCK_CAPACITY\
                        or 
                        util.calc_total_weight(data,sec_ord[:s_ids[0]])\
                        +util.calc_total_weight(data,sec_ord[s_ids[1]:])\
                        +fst_weight > TRUCK_CAPACITY):
                        continue
                    
                    dif=get_cross_dif(dis_mat,fst_ord,f_ids,sec_ord,s_ids)
                    
                    #ルートを交換する
                    if dif<0 and abs(dif)>1e-4:
                        tmp=fst_ord[f_ids[0]:f_ids[1]]
                        fst_ord[f_ids[0]:f_ids[1]]=sec_ord[s_ids[0]:s_ids[1]]
                        sec_ord[s_ids[0]:s_ids[1]]=tmp
                        isBreak=True
                        if is_save_gif:
                            util.gif_append(data,orders,ims)
        
        #一度近傍探索が終わるたびに各ルートに2opt法を実行する
        for i in range(len(orders)):
            two_opt_method(dis_mat,orders[i])
        
        #変更がなくなったらループを終了
        if not isBreak: break       
        print("\r"+str(util.calc_total_dis(dis_mat,orders)),end="") 
    
    print("\nfinish cross-exchange")
    if(f_title==""):
        pass
    else:
        util.save_gif(fig,ims,title=f_title)

def get_cross_dif(dis_mat,fst_ord,f_ids,sec_ord,s_ids):
    ret=-dis_mat[fst_ord[f_ids[0]-1]][fst_ord[f_ids[0]]]\
        -dis_mat[fst_ord[f_ids[1]-1]][fst_ord[f_ids[1]]]\
        -dis_mat[sec_ord[s_ids[0]-1]][sec_ord[s_ids[0]]]\
        -dis_mat[sec_ord[s_ids[1]-1]][sec_ord[s_ids[1]]]\
        +dis_mat[fst_ord[f_ids[0]-1]][sec_ord[s_ids[0]]]\
        +dis_mat[fst_ord[f_ids[1]-1]][sec_ord[s_ids[1]]]\
        +dis_mat[sec_ord[s_ids[0]-1]][fst_ord[f_ids[0]]]\
        +dis_mat[sec_ord[s_ids[1]-1]][fst_ord[f_ids[1]]]
    
    return ret

def insert_construct(dis_mat,truck_size,TRUCK_CAPACITY,data,size):
    """挿入法による初期解構築

    Args:
        dis_mat ([list[list]]): 各都市間の距離行列\\
        truck_size ([int]): トラックの台数\\
        TRUCK_CAPACITY ([int]): トラックの容量制限\\
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        size ([int]): デポを含んだ都市の大きさ

    Returns:
        [list[list]]: 各トラックの訪問する都市の順番のリスト
    """

    #デポから出発して戻ってくるルートを初期化しておく
    orders=[[0,0] for _ in range(truck_size)]
    weights=[0 for _ in range(truck_size)]
    idxs=[i for i in range(1,size)]
    idxs.sort(key=lambda x: data[x]["weight"])
    idxs.reverse()
    
    #とりあえず需要量の降順に各トラックの種顧客を設定
    for i in range(truck_size):
        orders[i].insert(1,idxs[i])
        weights[i]+=data[idxs[i]]["weight"]
    
    for i in range(truck_size,size-1):
        truck_id=0
        #insertは0を渡すと先頭に追加するので挿入位置は1で初期化
        ins_id=1
        min_dis=10**10
        for truck in range(truck_size):
            #トラックの容量を超えているならば挿入位置を調べることはしない
            if weights[truck]+data[idxs[i]]["weight"]>TRUCK_CAPACITY:
                continue

            for order in range(1,len(orders[truck])):
                cur_dis=dis_mat[orders[truck][order-1]][idxs[i]]+dis_mat[idxs[i]][orders[truck][order]]
                if cur_dis<min_dis:
                    min_dis=cur_dis
                    ins_id=order
                    truck_id=truck
        
        orders[truck_id].insert(ins_id,idxs[i])
        weights[truck_id]+=data[idxs[i]]["weight"]
    
    return orders

def saving_construct(dis_mat,truck_size,TRUCK_CAPACITY,data,size):
    """セービング法による初期解構築

    Args:
        dis_mat ([list[list]]): 各都市間の距離行列\\
        truck_size ([int]): トラックの台数\\
        TRUCK_CAPACITY ([int]): トラックの容量制限\\
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        size ([int]): デポを含んだ都市の大きさ

    Returns:
        [list[list]]: 各トラックの訪問する都市の順番のリスト
    """   
    #デポから顧客1人だけを訪問するルートで初期化
    orders=[[0,i,0] for i in range(size)]
    weights=[i["weight"] for i in data]

    #他のルートに繋げられていないトラックの添字集合
    valid=[i for i in range(1,size)]
    #繋げられてルートの最初でなくなった添字の集合
    invalid=[0]

    #トラックの台数が限度の台数に減るまで繰り返し
    while True:
        valid=[i for i in range(1,size) if i not in invalid]
        if len(valid)==truck_size: break

        i,j=valid[0],valid[1]
        max_dis=-10**10
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
        #最終的に実行できる解が出来なければjのルートをバラして生きているルートに併合
        if weights[i]+weights[j]>TRUCK_CAPACITY:
            sub_saving(TRUCK_CAPACITY,data,weights,valid,orders,j)
            invalid.append(j)
            continue
        #iを訪問してからデポに帰るルートとjを最初に訪問するルートを併合
        orders[i]=orders[i][:-1]+orders[j][1:]
        invalid.append(j)
        weights[i]+=weights[j]
        
    return [orders[i] for i in valid]

def sub_saving(TRUCK_CAPACITY,data,weights,valid,orders,j):
    """セービング法で併合エラーが起きたときのサブルーチン。
    併合される側をバラして生きているルートの先頭に挿入する。

    Args:
        TRUCK_CAPACITY ([int]): トラックの容量制限\\
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
        weights ([int]): セービング法での各トラックの合計重量\\
        valid ([list[int]]): セービング法で現在生きているトラックの添字\\
        orders ([list[int]]): セービング法での各トラックのルート\\
        j ([int]): ルートをバラされるトラックの添字
    """    
    for out in orders[j][1:-1]:
        for id in valid:
            if weights[id]+data[out]["weight"]>TRUCK_CAPACITY:
                continue
            orders[id].insert(1,out)
            weights[id]+=data[out]["weight"]
            
def kmeans(data,truck_size,TRUCK_CAPACITY):
    """dataをtruck_sizeのクラスターに分割し、各クラスターの添字をordersに格納して返す

    Args:
        data ([list[dict]]): 各顧客の位置と需要量の辞書\\
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
    
    return orders

def local_search_init(data,orders):
    idxs=[i for i in range(len(orders))]
    weights=[0 for _ in range(len(orders))]
    #重さの初期化
    for i in range(len(orders)):
        weights[i]=util.calc_total_weight(data,orders[i])
    
    return idxs,weights

def is_valid_weight(weights,capacity):
    return all([i<=capacity for i in weights])