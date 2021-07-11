import random
import matplotlib.pyplot as plt
import math

def dis(a,b):
    return ((a[0]-b[0])**2+(a[1]-b[1])**2)**.5

def nn_method(data,size):
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

def insertion_method(data,size,ins_state=0):
    if ins_state==0:
        pass
    elif ins_state==1:
        data.sort(key=lambda x: x[0])
    elif ins_state==2:
        data.sort(key=lambda x: x[1])
    
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

def sa_method(data,size):
    ans=random_route(data,size)
    ini_temp=500 #初期温度
    end_temp=0.1 #終了温度
    cool=0.9 #冷却スケジュール
    loop=10 #反復回数

    for _ in range(loop):
        temp=ini_temp
        while temp > end_temp:
            from_idx = random.randint(0,size-1)
            to_idx = random.randint(0,size-1)
            while from_idx==to_idx:
                to_idx = random.randint(0,size-1)
            
            new_ans=ans[:]
            dif_score = total_move_cost(ans)
            new_ans[from_idx],new_ans[to_idx]=new_ans[to_idx],new_ans[from_idx]
            dif_score -= total_move_cost(new_ans)
            #変更後の方が経路長が短くなっていればdif_score<0になる

            if dif_score < 0:
                ans=new_ans
            else:
                #Metropolis法を採用
                A=math.exp(-dif_score/temp)
                if random.random() < A:
                    ans=new_ans
            
            temp-=cool
    
    return ans


def show_data(data):
    for i,d in enumerate(data):
        print(i,":",d)

def two_opt_method(data,size):
    total=0
    while True:
        count = 0
        for i in range(size-2):
            i_next=i+1
            for j in range(i+2,size):
                if j==size-1:
                    j_next=0
                else:
                    j_next=j+1

                if i!=0 or j_next!=0:
                    l1 = dis(data[i],data[i_next])
                    l2 = dis(data[j],data[j_next])
                    l3 = dis(data[i],data[j])
                    l4 = dis(data[i_next],data[j_next])

                    if l1+l2 > l3+l4:
                        new_root = data[i_next:j+1]
                        data[i_next:j+1]=new_root[::-1]
                        count+=1
        total+=count
        if count==0: break
    
    return data

def random_route(data,size):
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
