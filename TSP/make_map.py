import random

size=100
fname="rand"+str(size)+"_weight.txt"
with open(fname,"w") as f:
    data=[]
    for i in range(size):
        point=(random.uniform(0,10),random.uniform(0,10),random.uniform(1,20))
        data.append(point)
        if i!=size-1: f.write(str(data[i][0])+" "+str(data[i][1])+" "+str(data[i][2])+"\n")
    f.write(str(data[size-1][0])+" "+str(data[size-1][1])+" "+str(data[i][2]))