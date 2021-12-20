import matplotlib.pyplot as plt

data="(2,857) (102,815) (181,760) (225,753) (116,698) (124,588) (86,528) (192,598) (220,643) (314,637) (419,592) (608,852) (511,898) (443,869) (336,912) (224,959) (109,935) (7,891) (2,857)"
datas=data.split()
datas=[i[1:-1] for i in datas]
data=[list(map(int,i.split(","))) for i in datas]

xs=[i[0] for i in data]
ys=[i[1] for i in data]
cus=[205,696]

plt.plot(xs,ys,label="route")
plt.scatter(cus[0],cus[1],label="customer")
plt.legend()
plt.show()