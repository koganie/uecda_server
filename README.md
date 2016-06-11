##コンピュータ大貧民（UECda）用の実験用サーバ（目標）
* 絶賛バグだらけ、構造ぐちゃぐちゃ、汚い、不潔
* 棋譜の採取や各種ルール対応を簡易にするための実験用サーバを目指している

### 利用方法
* makeしてbinの中にできる実行ファイルを起動する。UECda公式サーバと同じ。

### 現状
* UECdaルールで、一応動いているように見える
* 試合の設定はcppdhms.cfgにあるが、一部対応していない
* 
* 画面描画機能がない（作るつもりもない）
* けど棋譜再生のプログラムは、あったら面白そう

### 将来
* 棋譜採取機能（形式は？）
* UECda以外のルール（特に日本大富豪連盟公式ルール）への対応
	* プレイヤー人数
	* ジョーカー枚数
	* 都落ち
	* 11バック
	* 反則あがり
* それに伴う通信プロトコルについて
* 軽量化