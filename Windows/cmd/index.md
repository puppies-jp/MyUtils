# コマンドプロンプト

何かと使うことがあるので使い方便利コマンドをここにまとめる

[ヒアドキュメント](#here)

1. エスケープ文字

    > ^(キャレット) らしい、、(%はエスケープできなかった。。)
    > %については%%とか、、

2. pipeできないらしい、、
3. 実行後コマンドを繋げる

    ```bat
    :: &&　コマンド1の終了ステータスが0　でコマンド2が　実行される
    コマンド1 && コマンド2

    :: || コマンド1の終了ステータスが0以外でコマンド2が　実行される
    コマンド1 || コマンド2 

    コメントは以下でできる。(※remはbat実行時に見えるが、:: は出てこない)
    rem hogehoge
    :: hogefuga
    ```

4. 関数を定義する

    ```bat
    rem call :label1 で関数を呼べる。
    call :label1 arg1,arg2
    exit /b

    rem label1の関数定義はここ 
    rem exit /bを忘れるとさらに次の処理が実行される。(asmのgotoみたいな感じ？)
    :label1
    setlocal
        :: %1,2で引数を使用している
        set arg1=%1
        set arg2=%2 
    endlocal
    exit /b
    ```

<a name=here>ヒアドキュメント書き方</a>

試したけどできんかった
後日調べる

<a name=logrotate>ログローテート/保存周りをおこなう</a>

* forfiles
* 7zipを使う

```bat
forfiles /P D:\〇〇〇 /d -365 /C "cmd /c IF @isdir==TRUE rmdir /S /Q @file"

rem && を使うことでコマンド実行後に繋げることができる
rem pipeはないらしい、、
forfiles /P C:\Users\Administrator\Desktop /d -1 /C "cmd /c IF @isdir==TRUE  dir && echo ----- && tree"

set dirPath=C:\Users\Administrator\Desktop
set ptn=*.log
set cmd1=cmd /c IF @isdir==FALSE 

:: 🌟 コマンドの接続に&&つなぐ、ただし文字列なので^(キャレット)を使ってエスケープしている
set cmd2=echo @path ^&^& move @path ./driverInfo/
forfiles /P "%dirPath%" /M %ptn% /D -0 /C "%cmd1% %cmd2%"

:: 🌟 7zipで圧縮する場合はこんな感じで書ける
set cmd3=7zip a log.zip @path -mx=0 -sdel 
forfiles /P "%dirPath%" /M %ptn% /D -0 /C "%cmd1% %cmd3%"

```

## <a name=string>cmd文字列操作</a>

```bat
rem 文字列操作
set V=ABCDEFGHI

echo %V:~2,3%   #(0から数えて2番目の文字から3文字取得)
>> "CDE" が出力される

echo %V:~-2,3%  #(末尾から数えて2番目の文字から3文字取得)
>> "HI" が出力される

echo %V:~-2,-1%  #(末尾から数えて2番目の文字から末尾1文字削除)
>> "H" が出力される

echo %V:AB=ab%   #(変数内の文字t1を、文字t2に置換)
>> "abCDEFGHI" が出力される
```
