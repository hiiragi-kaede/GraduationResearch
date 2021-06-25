import matplotlib.pyplot as plt
import random

fname="rand10.txt"

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

def insertion_method(data,size):
    data.sort(key=lambda x: x[0])
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

def show_data(data):
    for i,d in enumerate(data):
        print(i,":",d)

with open(fname) as f:
    data=[list(map(float,i.split())) for i in f.readlines()]
    size=len(data)

    #ans=nn_method(data,size)
    ans=insertion_method(data,size)

    x=[i[0] for i in ans]
    y=[i[1] for i in ans]
    plt.plot(x,y,label="city")

    plt.scatter(ans[0][0],ans[0][1],label="start")
    plt.legend()
    plt.show()