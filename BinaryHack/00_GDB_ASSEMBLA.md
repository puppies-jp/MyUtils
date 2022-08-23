# GDB の使い方& ASSEMBlA

ここでは GDB の使い方,Assembla についてまとめる

## GDB の使い方

- GDB 起動

- exe ファイルを指定して起動 -q オプションはなくてもいいかも

```sh
gdb -q ~.out

# 起動済のプロセスにアタッチする gdb単体で起動後に`attach <PID>`でもOK
gdb --pid <PID>
```

- breakpointsの確認方法

> info breakpoints  
> disable `[breakpoint no]` #無効化  
> enable `[breakpoint no]`  #有効化  

- breakからの実行関連

> next # 関数の中までは追わない  
> step # 関数内まで追う  
> continue #次のbreakまで処理を継続  
> finish   #関数の終了進む(一個上のスタックまで戻る)  

### 処理を飛ばして実行する

こんな感じで実行することで指定行にjumpして処理を継続できる。fileのopenをお試しで`call`して成功して以降の処理で際呼び出しをするとエラーが発生するケースなどで使う

> `jump <行番号>`

### thread環境をデバッグする

以下のコマンドで各スレッドが何をしているのかがわかる。(わかると言っても動作中の関数ぐらいのものだが、、)

> `info threads`

他のスレッドに移ってスタックを確認するには以下のコマンドで切り替えることができる。

> `thread <スレッドID>`

- 例

```gdb
(gdb) info threads
  Id   Target Id                                  Frame 
* 1    Thread 0x7f1127df2740 (LWP 584) "main.out" 0x00005571e4c5c4d6 in main ()
(gdb) 

🌟 '*'がついている箇所が現在実行しているスレッドになる。
🌟 他のスレッドに移りたい場合はthread <スレッドId>で移動する
```

### 他いろいろ

- GDB でデバッグ情報を追加して表示するためのフラグ
  - こうすることで GDB 中の`list` コマンドでソースを見れる

```c
gcc -g ~.c
```

- こんな感じ

```gdb
(gdb) list
1   #include <stdio.h>
2
3   int main(){
4       for(int i =0 ; i < 5;i++){
5           printf("hello world\n");
6       }
7   }
8
```

### GDBでint以外の関数を使う

- GDBでint以外の関数を使うのはめんどくさい。。。

```gdb
print cos(0.0) #だめ
print (double)cos(0.0) #だめ

# 以下のようにすることで使える
set $p = (double (*)(double)) cos #1 cos関数を$pの変数で定義
ptype $p # 型確認コマンド(不要)
p $p(3.14159265) #ここで実行
```

### GDBではsignalをハンドリングしている処理を扱う必要がある。

- signalを受けてGDBでプログラムを止めたりすることがる。handleコマンドでいろいろできるらしい

こんな感じ
ただし、本当に発生するシグナルも見落とすことになるので注意

```gdb
# no~~ にどれぐらい種類があるのかは不明
handle SIGSEGV nostop noprint
```

### アセンブラ関連

- 関数をアセンブラで表示する(main は関数名)

  ```gdb
  disassembra main
  ```

- 実行例

  ```gdb
  (gdb) disassemble main
  Dump of assembler code for function main:
  0x0000558c10fcf149 <+0>:  endbr64
  0x0000558c10fcf14d <+4>:  push   %rbp
  0x0000558c10fcf14e <+5>:  mov    %rsp,%rbp
  => 0x0000558c10fcf151 <+8>:   lea    0xeac(%rip),%rdi        # 0x558c10fd0004
  0x0000558c10fcf158 <+15>: callq  0x558c10fcf050 <puts@plt>
  0x0000558c10fcf15d <+20>: mov    $0x0,%eax
  0x0000558c10fcf162 <+25>: pop    %rbp
  0x0000558c10fcf163 <+26>: retq
  End of assembler dump.
  (gdb)
  ```

![GDB_コマンドリスト](gdb_cmd_list.png)

## Assembla

| 機能名                         | レジスタ名(x86) | レジスタ名(x64) | 詳細                                                                                                                   |
| ------------------------------ | --------------: | --------------: | :--------------------------------------------------------------------------------------------------------------------- |
| インストラクションポインタ     |             eip |             rip | プロセッサによって実行されようとしている現在の命令が格納されているアドレス                                             |
| スタックポインタ               |             esp |             rsp | おそらくスタックフレームと対応するポインタ(引数の先頭？(いや、末尾か？))、スタックフレームの末端のポインタを格納する。 |
| ベースポインタ                 |             ebp |             rbp | 局所変数を参照するためのポインタ(stack フレームに格納された変数を参照するために使われる。)                             |
| ソースインデックス             |             esi |             rsi |                                                                                                                        |
| ディティネーションインデックス |             edi |                 |                                                                                                                        |
| アキュミュレータ               |             eax |                 |                                                                                                                        |
| カウンタ                       |             ecx |                 |                                                                                                                        |
| データレジスタ                 |             edx |                 |                                                                                                                        |
| ベースレジスタ                 |             ebx |                 |                                                                                                                        |

- 命令則

```assembly
命令語 <操作対象> <参照元>
```

- 例
  - $rbp +4 バイトのアドレスを+1 する。

```assembly
addl   $0x1,-0x4(%rbp)
```

現在の実行位置を知るためのコマンド

- **インストラクションポインタ**の中身を参照すれば OK

```sh
 info registers rip
 i r rip
```

格納されたポインタの中身を表示する方法

| 表示内容               | コマンド       | コマンド例          |
| ---------------------- | -------------- | ------------------- |
| 文字列表示             | x/s <ポインタ> | `x/s x/s $rbp-0x20` |
| プログラム上の変数表示 | x/s <変数名>   | `x/s str`           |
