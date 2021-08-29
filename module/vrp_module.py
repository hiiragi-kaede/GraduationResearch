import matplotlib.pyplot as plt

def dis(a,b):
    return ((a[0]-b[0])**2+(a[1]-b[1])**2)**.5

def draw_graph(data,order,title="default_title"):
    x=[data[i][0] for i in order]
    y=[data[i][1] for i in order]
    plt.plot(x,y,label="city")

    plt.scatter(data[order[0]][0],data[order[0]][1],label="start")
    plt.title(title)
    plt.legend()
    plt.show()
