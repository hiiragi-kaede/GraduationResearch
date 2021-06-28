import matplotlib.pyplot as plt

with open("a280_use.txt") as f:
    data = [list(map(int,i.split())) for i in f.readlines()]
    tmp=[]
    for v in data: tmp.append(v[1:])
    data=tmp

    x=[i[0] for i in data]
    y=[i[1] for i in data]

    plt.scatter(x,y)
    plt.title("a280 citys")
    plt.show()
    