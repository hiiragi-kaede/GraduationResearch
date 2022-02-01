# 概要
卒業研究をするにあたり、6月あたりから勉強していたものをまとめました。
毎日作業していた内容をいきあたりばったりでコミットしているので粒度がバラバラだったり、
ファイル構成やファイル名が適当なものがあるのはごめんなさい。

前半はTSPでの構築法や改善型解法、焼きなましも自作実装とライブラリ使用を試したりしました。

後半はVRPを解くように移行し、最終的にはCVRPを解くようになりました。

# 各フォルダの簡単な説明

* TSP：TSPを解くのに関わるプログラム、及びその出力画像など
* VRP：VRPを解くのに関わるプログラム、及びその出力画像など
* module：pythonのモジュール分割したファイル置き場
* 考察用：卒論の考察を書く時に使ったプログラムなどの置き場

# 使用したデータについて
[ベンチマークデータ掲載サイト1](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/vrp/)

[ベンチマークデータ掲載サイト2](http://vrp.atd-lab.inf.puc-rio.br/index.php/en/)

CVRP用のベンチマークデータがまとめてあるところからダウンロードし、一部情報を改変して利用。
改変後のデータはVRP/CVRP_Dataに格納。
掲載サイト1からダウンロードしたものがVRP/CVRP_Data/tsplibに格納されていて、
掲載サイト2からダウンロードしたもののうち、

VRP/CVRP_Data/vrplibにはUchoa et al.(2014)のデータが、

VRP/CVRP_Data/largeにはArnold, Gendreau and Sörensen (2017)のデータが格納されています。

改変内容としては、各データの上部に記載されている情報のうち、顧客数(Dimension)と容量(Capacity)のみを残して削除している。
今考えるとPythonで読み込んでいるので普通に読み飛ばすように実装すればよかった。
後でダウンロードしたデータでそのまま動くように治すかも(2022/2/1執筆現在修正なし)