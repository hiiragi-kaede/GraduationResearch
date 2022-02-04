# 概要
卒業研究をするにあたり、6月あたりから勉強していたものをまとめました。
毎日作業していた内容をいきあたりばったりでコミットしているので粒度がバラバラだったり、
ファイル構成やファイル名が適当なものがあるのはごめんなさい。

前半はTSPでの構築法や改善型解法、焼きなましも自作実装とライブラリ使用を試したりしました。

後半はVRPを解くように移行し、最終的にはCVRPを解くようになりました。

# 各フォルダの簡単な説明

* TSP：TSPを解くのに関わるプログラム、及びその出力画像など
* VRP：VRPを解くのに関わるプログラム、及びその出力画像など
* module：Pythonのモジュール分割したファイル置き場
* 考察用：卒論の考察を書く時に使ったプログラムなどの置き場

# 使用したデータについて
[ベンチマークデータ掲載サイト1](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/vrp/)

[ベンチマークデータ掲載サイト2](http://vrp.atd-lab.inf.puc-rio.br/index.php/en/)

CVRP用のベンチマークデータがまとめてあるところからダウンロードし、一部情報を改変して利用。
改変後のデータはVRP/CVRP_Dataに格納。

掲載サイト1からダウンロードしたものがVRP/CVRP_Data/tsplibに格納されていて、
掲載サイト2からダウンロードしたもののうち、

VRP/CVRP_Data/vrplibにはUchoa et al. (2014)のデータが、

VRP/CVRP_Data/largeにはArnold, Gendreau and Sörensen (2017)のデータが格納されている。

改変内容としては、各データの上部に記載されている情報のうち、顧客数(Dimension)と容量(Capacity)のみを残して削除している。
今考えるとPythonで読み込んでいるので普通に読み飛ばすように実装すればよかった。
後でダウンロードしたデータでそのまま動くように治すかも。(2022/2/1執筆現在修正なし)

# 実行方法
当初Pythonで実装していたものの、近傍探索は実行速度が重要なのでC++で実装をし直した。

そのため、CVRPを解くプログラムで最新のものはVRP/cpp/main.cppにまとめてある。

## 最初にやること
まず最初に、VRP/cppのディレクトリに移動して

```
$ mkdir tmp
```

を実行する必要がある。これは.gitignoreでgit上に上げてはいないものの、一時データを置くためのディレクトリとして必須。

## コンパイル
上記と同じようにVRP/cppに移動して、

```
$g++ -Wno-format-security -O3 -o main main.cpp src/construct.cpp src/util.cpp src/improve.cpp src/tools.cpp -pthread
```

を実行する。当然だが利用するにはg++のインストールが必要。

"-Wno-format-security"部分は必須ではないが、今回のプログラムには警告メッセージが出る
部分があるため、コンパイルのたびに警告メッセージが出るのを嫌がる人は入れておいたほうがよい。


## 実行
上記のコンパイルを実行した後に、

```
$./main < "入力データ名"
```

で実行できる。
入力データ名を具体的に例示する。

X-n303-k21.txtを対象に実行したいのであるならば
```
$./main < ../CVRP_Data/vrplib/X-n303-k21.txt
```
として実行する。

main.cppがあるディレクトリからの相対ディレクトリで入力データの位置を指定することに注意。

## コマンドのオプション
探索の効率化手法や近傍探索方法など、コマンドライン引数によって
実行内容を切り替えることができる。

これにより実行内容を切り替える際に再度コンパイルする必要がなくなる。

<table>
  <tr>
    <th>切り替え内容</th>
    <th>オプション</th>
    <th>説明</th>
    <th>注意事項</th>
  </tr>
  <tr>
    <td rowspan=8>近傍探索法</td>
    <td>-n=t</td>
    <td>2-opt*法に変更する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-n=ft</td>
    <td>高速2-opt*法に変更する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-n=it</td>
    <td>改善型2-opt*法に変更する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-n=c</td>
    <td>クロス交換近傍に変更する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-n=fc</td>
    <td>高速クロス交換近傍に変更する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-n=it</td>
    <td>改善型クロス交換近傍に変更する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-n=ilst</td>
    <td>改善型2-opt*法で反復局所探索法を実行させる</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-n=ilsc</td>
    <td>改善型クロス交換近傍で反復局所探索法を実行させる</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td rowspan=2>キック法</td>
    <td>-k=d</td>
    <td>double-bridgeに変更する</td>
    <td rowspan=2>反復局所探索法を使用していなければ<br>効果がないことに注意</td>
  </tr>
  <tr>
    <td>-k=f</td>
    <td>4-opt*法に変更する</td>
  </tr>
  <tr>
    <td rowspan=3>効率化手法</td>
    <td>-t=</td>
    <td>=の後に0でタブーリスト法を使用しない。<br>
    指定しないなど、それ以外の場合は利用する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-l=</td>
    <td>=の後に0で格子リスト法を使用しない。<br>
    指定しないなど、それ以外の場合は利用する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>-h=</td>
    <td>=の後に0で多角形判定法を使用しない。<br>
    指定しないなど、それ以外の場合は利用する</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td>乱数の種</td>
    <td>-s=</td>
    <td>=の後の数字を使って乱数を初期化する。指定しないときは0を使う。</td>
    <td>マルチスレッドの場合は0を代入した場合、0から(スレッド数-1)までの数字を使う。</td>
  </tr>
</table>

実際に実験で使ったコマンドは以下のようになっている。
```
$./main -n=ilsc -k=f -t=0 -l=0 -h=0 < ../CVRP_Data/vrplib/X-n1001-k43.txt
$./main -n=ilsc -k=f -t=0 -l=0 -h=1 < ../CVRP_Data/vrplib/X-n1001-k43.txt
$./main -n=ilsc -k=f -t=0 -l=1 -h=0 < ../CVRP_Data/vrplib/X-n1001-k43.txt
$./main -n=ilsc -k=f -t=0 -l=1 -h=1 < ../CVRP_Data/vrplib/X-n1001-k43.txt
$./main -n=ilsc -k=f -t=1 -l=0 -h=0 < ../CVRP_Data/vrplib/X-n1001-k43.txt
$./main -n=ilsc -k=f -t=1 -l=0 -h=1 < ../CVRP_Data/vrplib/X-n1001-k43.txt
$./main -n=ilsc -k=f -t=1 -l=1 -h=0 < ../CVRP_Data/vrplib/X-n1001-k43.txt
$./main -n=ilsc -k=f -t=1 -l=1 -h=1 < ../CVRP_Data/vrplib/X-n1001-k43.txt
```
これらのコマンドをスレッド数8で-s=0から-s=2まで実行させている。

## 実行にあたっての注意
解を求めた後、その解の可視化にVRP/cpp/plot.pyを用いているが、
popenを使ってPythonを絶対パスで呼び出させている。

そのため、自分の環境でしか動かないコードになっている。

どこの環境でも動くと思われる修正方法をここに提示しておく。

main.cppをコンパイル実行したあとに、ディレクトリ移動をすることなく

```
$ python plot.py
$ tmp/data.txt
$ tmp/order.txt
```

を入力するようにすればPython,matplotlibが使える環境ならこのようなグラフが表示される。

![sample_out](https://user-images.githubusercontent.com/53923593/152079943-6df05016-c67e-4428-a875-75f77fc80949.png)

これはお試しでVRP/CVRP_Data/vrplib/X-n303-k21.txtに対して実行して表示させたもの。

サンプルとして使用するのでトラック番号がきちんと表示されるように調整しているが、デフォルトではtruck~のところは見切れているはず。