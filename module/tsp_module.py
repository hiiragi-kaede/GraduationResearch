import random
import matplotlib.pyplot as plt
import math
import time
import sys

from scipy.spatial import distance
import matplotlib.pyplot as plt
import pandas as pd
import networkx as nx

def dis(a,b):
    return ((a[0]-b[0])**2+(a[1]-b[1])**2)**.5

def nn_method(size,dis_mat):
    idx=random.randint(0,size-1)
    visited=[idx]

    for i in range(size-1):#次に行く都市を(size-1)回選ぶ
        #とりあえず次に訪れるのは訪れていない街の内、一番添字が小さい都市とする
        idxs=[j for j in range(size) if j not in visited]
        
        if len(idxs)==0:
            print("point unexist error")
            exit(1)
        
        minid=idxs[0]
        min=dis_mat[visited[-1]][minid]
        for id in idxs:
            if min>dis_mat[visited[-1]][id]:
                minid=id
                min=dis_mat[visited[-1]][id]
        
        visited.append(minid)
    
    #スタートに戻ってくる
    visited.append(idx)
    return visited

def insertion_method(data,dis_mat,ins_state=0):
    order=random_route(len(data))
    ziped=zip(data,order)
    if ins_state==0:
        pass
    elif ins_state==1:
        zip_sort = sorted(ziped,key=lambda x: x[0][0])
    elif ins_state==2:
        zip_sort = sorted(ziped,key=lambda x: x[0][1])

    data, order = zip(*zip_sort)
    size=len(data) 
    ans=[order[0],order[0]]
    
    for i in range(size-1):
        minid=0
        min=1e9
        for j in range(len(ans)-1):
            if min > dis_mat[order[j]][order[i+1]]+dis_mat[order[j+1]][order[i+1]]:
                minid=j
                min=dis_mat[order[j]][order[i+1]]+dis_mat[order[j+1]][order[i+1]]
        ans.insert(minid+1,order[i+1])
    
    return ans

def sa_method(data,size,dis_mat):
    order=random_route(size)
    #order=nn_method(size,dis_mat)
    
    ini_temp=50000 #初期温度
    cool=0.9 #冷却スケジュール
    unchanged=0 #解の更新幅が小さかった回数
    time_threshold=50 #反復を終了するまでの経過時間(s)
    count=0
    temp=ini_temp
    unchanged=0
    accept=0
    trial=0

    print("近傍操作を選択してください")
    print("0:交換近傍(2都市を交換)")
    print("1:3都市近傍(3都市をランダムな順に交換)")
    type=int(input("neighbor type:"))
    
    sa_counts=[count]
    sa_costs=[total_move_cost(order,dis_mat)]
    
    i = random.randint(0,size-2)
    st=time.time()
    while unchanged<300*size:
        count+=1
        if time.time()-st>time_threshold: break
        #その温度で周辺をある程度探索してから冷却を行う
        if accept>=10*size or trial>=100*size:
            temp*=cool
            accept=0
            trial=0
            sa_counts.append(count)
            sa_costs.append(total_move_cost(order,dis_mat))
        
        print("\rcurrent temp:"+str(temp),end="")
        trial+=1
        #変更後の方が経路長が短くなっていればdif_score<0になる
        if type==0:
            dif_score,new_order=sa_exchange_neighbor(order,dis_mat)
        else:
            dif_score,new_order=sa_triple_exchange(order,dis_mat)

        if dif_score <=0:
            order=new_order
            accept+=1
        else:
            #Metropolis法を採用
            A=math.exp(-dif_score/temp)
            if random.random() < A:
                order=new_order
                accept+=1
        
        if abs(dif_score)<10:
            unchanged+=1
        else:
            unchanged=0
        
    elapsed=time.time()-st
    print(" - - > elapsed time:{0}".format(elapsed)+"[sec]")

    sa_draw_score_graph(x=sa_counts,y=sa_costs)

    return order

def sa_draw_score_graph(x,y):
    plt.plot(x,y)
    plt.title("SA scores")
    plt.xlabel("SA trial")
    plt.ylabel("Score")
    plt.show()

def sa_exchange_neighbor(order,dis_mat):
    """焼きなまし用の交換近傍操作を行う

    Args:
        order ([list]): 都市の訪問順のリスト
        dis_mat ([list[list]]): 各都市間の距離行列

    Returns:
        [list]: dif_score,new_ansの順で構成されたリスト
    """  
    #戻ってくる順番も足されているため、-1しないといけない  
    size=len(order)-1
    from_idx = random.randint(1,size-1)
    to_idx = random.randint(1,size-1)
    while from_idx==to_idx:
        to_idx = random.randint(1,size-1)
    
    new_order=order[:]
    from_prev=order[from_idx-1]
    from_curr=order[from_idx]
    from_next=order[from_idx+1]
    to_prev=order[to_idx-1]
    to_curr=order[to_idx]
    to_next=order[to_idx+1]

    score_before = dis_mat[from_prev][from_curr]+dis_mat[from_curr][from_next]
    score_before += dis_mat[to_prev][to_curr]+dis_mat[to_curr][to_next]
    new_order[from_idx],new_order[to_idx]=new_order[to_idx],new_order[from_idx]

    from_prev=new_order[from_idx-1]
    from_curr=new_order[from_idx]
    from_next=new_order[from_idx+1]
    to_prev=new_order[to_idx-1]
    to_curr=new_order[to_idx]
    to_next=new_order[to_idx+1]

    score_after = dis_mat[from_prev][from_curr]+dis_mat[from_curr][from_next]
    score_after += dis_mat[to_prev][to_curr]+dis_mat[to_curr][to_next]

    dif_score=score_after-score_before
    return [dif_score,new_order]

def sa_triple_exchange(order,dis_mat):
    """焼きなまし用の3つの都市の交換操作を行う

    Args:
        order ([list]): 都市の訪問順のリスト
        dis_mat ([list[list]]): 各都市間の距離行列
    
    Returns:
        [list]: dif_score,new_ansの順で構成されたリスト
    """
    size=len(order)
    a_idx=random.randint(1,size-4)
    b_idx=random.randint(a_idx+1,size-3)
    c_idx=random.randint(b_idx+1,size-2)
    a_ids=[order[a_idx-1],order[a_idx],order[a_idx+1]]
    b_ids=[order[b_idx-1],order[b_idx],order[b_idx+1]]
    c_ids=[order[c_idx-1],order[c_idx],order[c_idx+1]]

    new_order=order[:]
    score_before=dis_mat[a_ids[0]][a_ids[1]]+dis_mat[a_ids[1]][a_ids[2]]
    score_before+=dis_mat[b_ids[0]][b_ids[1]]+dis_mat[b_ids[1]][b_ids[2]]
    score_before+=dis_mat[c_ids[0]][c_ids[1]]+dis_mat[c_ids[1]][c_ids[2]]

    tmp=[order[a_idx],order[b_idx],order[c_idx]]
    random.shuffle(tmp)
    new_order[a_idx],new_order[b_idx],new_order[c_idx]=tmp

    a_ids=[new_order[a_idx-1],new_order[a_idx],new_order[a_idx+1]]
    b_ids=[new_order[b_idx-1],new_order[b_idx],new_order[b_idx+1]]
    c_ids=[new_order[c_idx-1],new_order[c_idx],new_order[c_idx+1]]
    score_after=dis_mat[a_ids[0]][a_ids[1]]+dis_mat[a_ids[1]][a_ids[2]]
    score_after+=dis_mat[b_ids[0]][b_ids[1]]+dis_mat[b_ids[1]][b_ids[2]]
    score_after+=dis_mat[c_ids[0]][c_ids[1]]+dis_mat[c_ids[1]][c_ids[2]]

    dif_score=score_after-score_before
    return [dif_score,new_order]

def show_data(data):
    """
    ルートの内容を表示させる
    """
    for i,d in enumerate(data):
        print(i,":",d)

def two_opt_method(dis_mat,order):
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

def random_route(size):
    order=[i for i in range(size)]
    order.append(order[0])
    return order

def draw_graph(data,order,title="default_title"):
    x=[data[i][0] for i in order]
    y=[data[i][1] for i in order]
    plt.plot(x,y,label="city")

    plt.scatter(data[order[0]][0],data[order[0]][1],label="start")
    plt.title(title)
    plt.legend()
    plt.show()

def total_move_cost(order,dis_mat):
    total=0
    for i in range(len(order)-1):
        total+=dis_mat[order[i]][order[i+1]]
    return total

def draw_nx_graphs(data,order):

    df = pd.DataFrame({ 'x_position' : [i[0] for i in data],
                            'y_position' : [i[1] for i in data]})
    plt.scatter(x=df.x_position, y=df.y_position)

    mat = df[['x_position', 'y_position']].values
    dist_mat = distance.cdist(mat, mat, metric='euclidean') # ユークリッド距離

    # 各点間の距離ディクショナリーを作成
    dic_xy2dist = {}
    for i in df.index:
        if i not in dic_xy2dist.keys():
            dic_xy2dist[i] = {}
        for j in df.index:
            dic_xy2dist[i][j] = dist_mat[i][j]

    node_labels = df.index
    # 各ノード（点）のx,y座標をdictionaryとして作成します。最後に描画する際に必要になります。
    pos = {k : (v.x_position,v.y_position) for k,v in df.iterrows()}
    # グラフオブジェクトを生成しています。
    G = nx.Graph()
    # ノード（点）をグラフに一括登録しています。ノード名はindexIDとしています。
    G.add_nodes_from(node_labels)

    for i in range(len(order)-1):
        G.add_edge(order[i],order[i+1])

    # 最終的にネットワークを描画します。pos引数に先ほど作ったposを渡すとx,y座標が反映されます。
    nx.draw_networkx(G, node_color="c", pos=pos, node_size=10)
    plt.show()