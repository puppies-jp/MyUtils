# コマンドプロンプト

何かと使うことがあるので使い方便利コマンドをここにまとめる

[ヒアドキュメント](#here)

1. エスケープ文字

    > ^(キャレット) らしい、、(%はエスケープできなかった。。)
    > %については%%とか、、

2. pipeできないらしい、、
3. 実行後コマンドを繋げる
    > &&

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

rem 🌟 コマンドの接続に&&でなく^を使ってエスケープしている
set cmd2=echo @path ^&^& move @path ./driverInfo/
forfiles /P "%dirPath%" /M %ptn% /D -0 /C "%cmd1% %cmd2%"

rem 🌟 7zipで圧縮する場合はこんな感じで書ける
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
