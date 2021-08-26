from simanneal import Annealer
import random
import numpy as np
from scipy.spatial import distance
import matplotlib.pyplot as plt
import pandas as pd
import networkx as nx

# １００点のx, y座標をランダムで生成
np.random.seed(seed=32)
df = pd.DataFrame({ 'x_position' : np.random.randint(1,100,30),
                        'y_position' : np.random.randint(1,100,30),
                        'demand' : np.random.randint(1,13,30)})
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


# 焼きなまし法クラスを宣言
class TravellingSalesmanProblem(simanneal.Annealer):
    # コンストラクター：距離テーブルと解状態を与えています。
    def __init__(self, state, distance_matrix):
        self.distance_matrix = dic_xy2dist
        super(TravellingSalesmanProblem, self).__init__(state)  # important!

    # 状態遷移（入れ替え）の範囲を指定しています。
    # 今回はある点とある点を交換するということにしていますが、ある点2つとある点2つを同時に交換という形で記載することも可能です。
    def move(self):
        initial_energy = self.energy()
        """Swaps two cities in the route."""
        a = random.randint(0, len(self.state) - 1)
        b = random.randint(0, len(self.state) - 1)
        self.state[a], self.state[b] = self.state[b], self.state[a]
        
        return self.energy() - initial_energy

    # 結果の評価方法を指定しています。今回は総距離のため、距離計算をしています。
    def energy(self):
        """Calculates the length of the route."""
        e = 0
        for i in range(len(self.state)):
            e += self.distance_matrix[self.state[i-1]][self.state[i]]
        return e

#初期解を生成
init_state = list(df.index)
random.shuffle(init_state)

# 焼きなましオブジェクトを生成。初期解を第一引数、距離テーブル（辞書）を第二引数に渡しています。
tsp = TravellingSalesmanProblem(init_state, dic_xy2dist)
tsp.copy_strategy = "slice"

# パラメータは下記のデフォルト値が使われます。今回はステップ数を増やして計算を実行します。

#Tmax = 25000.0  # 最大温度
#Tmin = 2.5      # 最小温度
#steps = 50000   # 入れ替え回数
#updates = 100 結果のリアルタイム出力の回数。大きいほど頻繁に更新される。計算結果には影響はない。

tsp.updates = 10000
tsp.steps = 500000 
state, e = tsp.anneal()
print("総距離：{0}".format(e))
print("巡回路")
print(state)

node_labels = df.index
# 各ノード（点）のx,y座標をdictionaryとして作成します。最後に描画する際に必要になります。
pos = {k : (v.x_position,v.y_position) for k,v in df.iterrows()}
# グラフオブジェクトを生成しています。
G = nx.Graph()
# ノード（点）をグラフに一括登録しています。ノード名はindexIDとしています。
G.add_nodes_from(node_labels)

# 先ほど算出した巡回路が入ったstateをfor文で回し、グラフにリンク（線）を追加していきます。
for i in range(len(state)):
    if i < len(state)-1:
        G.add_edge(state[i], state[i+1] )
    else:
        G.add_edge(state[i], state[0])
    
# 最終的にネットワークを描画します。pos引数に先ほど作ったposを渡すとx,y座標が反映されます。
nx.draw_networkx(G, node_color="c", pos=pos, node_size=10)
plt.show()
#参考サイト
#https://axross-recipe.com/recipes/103
#https://github.com/perrygeo/simanneal