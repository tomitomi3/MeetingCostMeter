# MeetingCostMeter
MeetingCostMeter / 会議の見せる化タイマー

## Introduction

自動運転等に用いられる光で物体までの距離を測距する技術であるLIDARを使って身近に使えそうな会議コストを“見せる”化するモノを考えました。人が参加者の人数を入力し、時給表示させるアプリや計算機は存在します。人の手を介在させず自動的に人数を把握しコストを通知できる「MeetingCostMeter」を作りました。

## 外観

* NT加賀2017で参考出展したモノ

ToFなLiDARモジュールとサーボを用いて周辺情報を取得する。このハードを置いた周囲１８０度の凸凹≒人のいる数として会議参加している人から時給換算して会議コストの”見せる化”ができないかを考えている。

![meetingcostmeter_proto](https://github.com/tomitomi3/MeetingCostMeter/blob/master/pic/meetingcostmeter_ntkaga2017.jpg)

社会人というかサラリーマンには好評だった。

* Maker Faire Tokyo 2018に出展
 * 出展ページ:[うーぼーず Wooboos](https://makezine.jp/event/makers2018/m0431/)

LiDARを使用して見栄えをよくしたが、、、LiDARの制御が難しい。

さすがMaker Faire。人が多い。好評（主観）であった。ハッカソンで会議コストを減らすというテーマの方がいたようであった。概念実証みたいになったかも。

![meetingcostmeter_proto](https://github.com/tomitomi3/MeetingCostMeter/blob/master/pic/meetingcostmeter_mftokyo2018.jpg)

## システム図

![systemdiagram](https://github.com/tomitomi3/MeetingCostMeter/blob/master/pic/meetingcostmeter_ntkaga2017_block.jpg.png)

## Todo

* 光の広がり考慮 → 360度回転するLiDAR使用により済
* LIDARモジュールの回転機構改善 → 360度回転するLiDAR使用により済
* 外観の改善 → 済
* 検出ロジックの再選定（非線形分離が出来るようなやつ。。。）
* LiDARモジュールの制御（難しい。192kbpsというシリアル通信ができる別マイコンで制御といったことを考える必要あるかも）