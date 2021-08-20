import random
import matplotlib.pyplot as plt
import math
import time

def dis(a,b):
    return ((a[0]-b[0])**2+(a[1]-b[1])**2)**.5

def nn_method(data):
    size=len(data)
    idx=random.randint(0,size-1)
    visited=[idx]
    ans=[[data[idx][0],data[idx][1]]]

    for i in range(size-1):#次に行く都市を(size-1)回選ぶ
        #とりあえず次に訪れるのは訪れていない街の内、一番添字が小さい都市とする
        idxs=[j for j in range(size) if j not in visited]
        
        if len(idxs)==0:
            print("point unexist error")
            exit(1)
        
        minid=idxs[0]
        min=dis(ans[-1],data[minid])
        for id in idxs:
            if min>dis(ans[-1],data[id]):
                minid=id
                min=dis(ans[-1],data[id])
        
        visited.append(minid)
        ans.append([data[minid][0],data[minid][1]])
    
    ans.append([data[idx][0],data[idx][1]])
    return ans

def insertion_method(data,ins_state=0):
    if ins_state==0:
        pass
    elif ins_state==1:
        data.sort(key=lambda x: x[0])
    elif ins_state==2:
        data.sort(key=lambda x: x[1])

    size=len(data) 
    ans=[[data[0][0],data[0][1]],[data[0][0],data[0][1]]]
    
    for i in range(size-1):
        minid=0
        min=1e9
        for j in range(len(ans)-1):
            if min > dis(data[j],data[i+1])+dis(data[j+1],data[i+1]):
                minid=j
                min=dis(data[j],data[i+1])+dis(data[j+1],data[i+1])
        ans.insert(minid+1,[data[i+1][0],data[i+1][1]])
    
    return ans

def sa_method(data):
    #ans=random_route(data,size)
    ans=nn_method(data)
    ini_temp=50000 #初期温度
    cool=0.9995 #冷却スケジュール
    unchanged=0 #解の更新がなかった回数
    unchange_threshold=10 #反復を終了する解の更新がなかった回数
    time_threshold=100 #反復を終了するまでの経過時間

    draw_graph(ans)
    temp=ini_temp
    unchanged=0
    st=time.time()
    while unchanged<unchange_threshold:
        if time.time()-st>time_threshold: break
        #変更後の方が経路長が短くなっていればdif_score<0になる
        #dif_score,new_ans=sa_exchange_neighbor(ans)
        dif_score,new_ans=sa_two_opt_exchange(ans)

        if dif_score <=0:
            ans=new_ans
            unchanged=0
        else:
            #Metropolis法を採用
            A=math.exp(-dif_score/temp)
            if random.random() < A:
                ans=new_ans
            else:
                unchanged+=1
        
        temp*=cool
        
    elapsed=time.time()-st
    print(" - - > elapsed time:{0}".format(elapsed)+"[sec]")
    
    return ans

def sa_exchange_neighbor(ans):
    """焼きなまし用の交換近傍操作を行う

    Args:
        ans (list[list]): 都市の訪問順のリスト

    Returns:
        [list]: dif_score,new_ansの順で構成されたリスト
    """  
    #戻ってくる順番も足されているため、-1しないといけない  
    size=len(ans)-1
    from_idx = random.randint(1,size-1)
    to_idx = random.randint(1,size-1)
    while from_idx==to_idx:
        to_idx = random.randint(1,size-1)
    
    new_ans=ans[:]
    score_before = dis(ans[from_idx-1],ans[from_idx])+dis(ans[from_idx],ans[from_idx+1])
    score_before += dis(ans[to_idx-1],ans[to_idx])+dis(ans[to_idx],ans[to_idx+1])
    new_ans[from_idx],new_ans[to_idx]=new_ans[to_idx],new_ans[from_idx]
    score_after = dis(new_ans[from_idx-1],new_ans[from_idx])+dis(new_ans[from_idx],new_ans[from_idx+1])
    score_after += dis(new_ans[to_idx-1],new_ans[to_idx])+dis(new_ans[to_idx],new_ans[to_idx+1])

    dif_score=score_after-score_before
    return [dif_score,new_ans]

def sa_two_opt_exchange(ans):
    """焼きなまし用の2-opt近傍操作を行う

    Args:
        ans (list[list]): 都市の訪問順のリスト
    
    Returns:
        [list]: dif_score,new_ansの順で構成されたリスト
    """
    #戻ってくる順番も足されているため、-1しないといけない
    size=len(ans)-1
    for i in range(1,size-2):
        i_next=i+1
        for j in range(i+2,size-1):
            j_next=(j+1)%size

            l1 = dis(ans[i],ans[i_next])
            l2 = dis(ans[j],ans[j_next])
            l3 = dis(ans[i],ans[j])
            l4 = dis(ans[i_next],ans[j_next])

            cost_before = l1+l2
            cost_after = l3+l4
            dif_score = cost_after - cost_before

            new_root = ans[i_next:j+1]
            ans[i_next:j+1]=new_root[::-1]
    
    return [dif_score,ans]

def show_data(data):
    """
    ルートの内容を表示させる
    """
    for i,d in enumerate(data):
        print(i,":",d)

def two_opt_method(data):
    size=len(data)
    while True:
        count = 0
        for i in range(1,size-2):
            i_next=i+1
            for j in range(i+2,size-1):
                j_next=(j+1)%size

                l1 = dis(data[i],data[i_next])
                l2 = dis(data[j],data[j_next])
                l3 = dis(data[i],data[j])
                l4 = dis(data[i_next],data[j_next])

                if l1+l2 > l3+l4:
                    new_root = data[i_next:j+1]
                    data[i_next:j+1]=new_root[::-1]
                    count+=1

        if count==0: break
    
    return data

def random_route(data):
    size=len(data)
    tmp=[[data[i][0],data[i][1]] for i in range(size)]
    tmp.append([data[0][0],data[0][1]])
    return tmp

def draw_graph(ans,title="default_title"):
    x=[i[0] for i in ans]
    y=[i[1] for i in ans]
    plt.plot(x,y,label="city")

    plt.scatter(ans[0][0],ans[0][1],label="start")
    plt.title(title)
    plt.legend()
    plt.show()

def total_move_cost(ans):
    total=0
    for i in range(len(ans)-1):
        total+=dis(ans[i],ans[i+1])
    return total
