# Module_Library
マイコン周辺のセンサーやモジュールのライブラリ

# 利用の仕方
ダウンロードしたzipファイルを展開し、中身をソースコードと同じフォルダに置いてください。  使用するモジュールのファイルだけ置いてもかまいませんが、I2Cなどライブラリ内で依存関係があるものがあるので、エラーを見て必要なファイルを置いてください。


マイコンごとにI2Cなどを使用するための関数が異なるので、○○Handler.hなどの#defineから使用するマイコンに当てはまるもののコメントアウトを外してください。

改変した場合その旨と内容を最初にコメントし、またgithub側を変更する際には後述の「製作の仕方」に従ってください。


以下に典型的な利用の仕方を示します。     
①使うセンサーのヘッダをインクルード  
②[センサー名]型のクラスを作成  
③クラス名.Initialize();で初期化  
④クラス名.Read○○(&x, &y ,&z);で読み取り(○○は読み取る対象の略　例：気圧→Prs)  
⑤値を送信し④に戻る  


# 製作の仕方
どんな稚拙なライブラリでも「Bad is better than nothing(ないよりはまし)」です。  新しいセンサー、新しいマイコンの動作が確認できた場合は積極的にライブラリに追加してください。  一方で、このライブラリは開発の効率を上げるためのものです。ハンドラなどコアとなる(実行頻度が高く他から参照されやすい)部分はバグやボトルネックが発生しないよう注意深くコーディングしてください。またメモリの使用量にも気を付けてください。

製作する際は以下の規則に従ってください。  長いですが、とにかく可読性が確保されていて、コメントがしっかり入っていればまぁOK。詳細は既存のライブラリを参考にしてください。

・コード冒頭に最低限「製作者」、「製作年月」、「対象のモジュール名称」をコメント  

・I2C、SPI、シリアル通信などのマイコンによって異なる部分は必ずハンドラ(○○Handler.h)を作成して分離し、特定のマイコンだけで使用できるコードを作らない  

・上記のハンドラを作成、追記する場合、プリプロセッサを利用して

    //#define ARDUINO
    //#define MBED
    
    #ifdef ARDUINO
    
    ～コード～
    
    #endif
    #ifdef MBED
    
    ～コード～
    
    #endif  
  のようにコメントアウト解除で対応するマイコンを選択するようにする  
  
・同様に、モジュールのヘッダからハンドラを二重に読み込むのを避けるため

    #ifndef I2CHANDLER_H
    #define I2CHANDLER_H
    
    ～コード～
    
    #endif
    
  のようにする(詳細は既存のハンドラを参考に)  
・モジュールはモジュール名型のクラスを作って利用する形式にする  

・なるべく  

    センサー名型のクラス クラス名;
          ↓
    クラス名.Initialize();
          ↓
    クラス名.Read○○(ポインタ);
    
   の形式で利用できるようにする  
   
・センサーの初期値や測定レンジの設定がある場合は、上記の関数のオーバーロードとして実装し、引数なしの場合は一般的な値(測定レンジなら最大値等)を設定しておく(その旨をコード冒頭に「使い方」としてコメント)  
    例：  
        void クラス名.Initiaize();　←最も一般的な設定で初期化  
        void クラス名.Initialize(float 測定レンジ);  
        
・I2Cアドレス、レジスタ、定数などはマクロ定義しマジックナンバーを作らない

・略称の表記、文字数(基本的に3文字)は統一する  
    代表的な略称  
      気圧：Prs  
      気温：Tmp  
      加速度：Acc  
      角速度：Gyr  
      地磁気：Mag  
      
・読み取りの関数はRead[略称](&x, &y,...)の形式で統一  
・#define文ではマクロの前にセンサー名などを付けて別のヘッダと重複しないようにする
