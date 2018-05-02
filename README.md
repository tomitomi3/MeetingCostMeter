# MeetingCostMeter
MeetingCostMeter / 会議の見せる化タイマー

## Introduction

自動運転等に用いられる光で物体までの距離を測距する技術であるLIDARを使って身近に使えそうな会議コストを“見せる”化するモノを考えました。人が参加者の人数を入力し、時給表示させるアプリや計算機は存在します。人の手を介在させず自動的に人数を把握しコストを通知できる「MeetingCostMeter」を作りました。

## 外観

NT加賀2017で参考出展したモノ

![meetingcostmeter_proto](https://github.com/tomitomi3/MeetingCostMeter/blob/master/pic/meetingcostmeter_ntkaga2017.jpg)

## システム図

![systemdiagram](https://github.com/tomitomi3/MeetingCostMeter/blob/master/pic/meetingcostmeter_ntkaga2017_block.jpg.png)

## Todo

* 光の広がり考慮
* LIDARモジュールの回転機構改善
* 外観の改善
* 検出ロジックの再選定（非線形分離が出来るようなやつ。。。）
