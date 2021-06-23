import random

size=10
fname="rand"+str(size)+".txt"
with open(fname,"w") as f:
    data=[]
    for i in range(size):
        point=(random.uniform(0,10),random.uniform(0,10))
        data.append(point)
        if i!=size-1: f.write(str(data[i][0])+" "+str(data[i][1])+"\n")
    f.write(str(data[size-1][0])+" "+str(data[size-1][1]))