import random
import matplotlib.pyplot as plt
import math
import time
import sys

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
    time_threshold=100 #反復を終了するまでの経過時間(s)

    draw_graph(data,order)
    temp=ini_temp
    unchanged=0
    print("近傍操作を選択してください")
    print("0:交換近傍(2都市を交換)")
    print("1:2-opt近傍(2つの辺を交換)")
    type=int(input("neighbor type:"))
    st=time.time()
    accept=0
    trial=0
    while unchanged<300*size:
        if time.time()-st>time_threshold: break
        #その温度で周辺をある程度探索してから冷却を行う
        if accept>=10*size or trial>=100*size:
            temp*=cool
            accept=0
            trial=0
        
        # sys.stdout.write("\033[2K\033[G current temp:%s" % str(temp))
        # sys.stdout.flush()
        print("\rcurrent temp:"+str(temp),end="")
        trial+=1
        #変更後の方が経路長が短くなっていればdif_score<0になる
        if type==0:
            dif_score,new_order=sa_exchange_neighbor(order,dis_mat)
        else:
            dif_score,new_order=sa_two_opt_exchange(order,dis_mat)

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

    
    return order

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

def sa_two_opt_exchange(order,dis_mat):
    """焼きなまし用の2-opt近傍操作を行う

    Args:
        order ([list]): 都市の訪問順のリスト
        dis_mat ([list[list]]): 各都市間の距離行列
    
    Returns:
        [list]: dif_score,new_ansの順で構成されたリスト
    """
    size = len(order)
    i = random.randint(0,size-4)
    i_next = i+1
    j = random.randint(i+2,size-2)
    j_next = (j+1)%size

    l1 = dis_mat[order[i]][order[i_next]]
    l2 = dis_mat[order[j]][order[j_next]]
    l3 = dis_mat[order[i]][order[j]]
    l4 = dis_mat[order[i_next]][order[j_next]]

    cost_before = l1+l2
    cost_after = l3+l4
    dif_score = cost_after - cost_before

    new_root = order[i_next:j+1]
    order[i_next:j+1]=new_root[::-1]
    
    return [dif_score,order]

def show_data(data):
    """
    ルートの内容を表示させる
    """
    for i,d in enumerate(data):
        print(i,":",d)

def two_opt_method(dis_mat,order):
    #スタートに戻ってくる手続きのだけsizeを調整
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
