# filesystem

Linuxのfilesystemについてのあれこれをメモってく

- [`df`について](#df)
- [`fdisk`について(hdd,ssdのパーティション作成)](#fdisk)
- [`mkfs`について(ファイルシステム作成)]()

---

## <a name='df'>filesystemのをみるやつ</a>

> df -h

|Filesystem|Size|Used|Avail Use% |Mounted on|
|--|--|--|--|--|
|ディスク名|全ディスク容量|使用容量|空き容量|使用率|ディスクがファイルシステム上のどこにマウントされているのか(マウントポイント)|

```sh
(base)root@ubuntu:~\# df -h
Filesystem      Size  Used Avail Use% Mounted on
overlay          59G   28G   29G  49% /
tmpfs            64M     0   64M   0% /dev
shm              64M     0   64M   0% /dev/shm
grpcfuse        466G  426G   22G  96% /root/Desktop
/dev/vda1        59G   28G   29G  49% /etc/hosts
tmpfs           2.0G     0  2.0G   0% /proc/acpi
tmpfs           2.0G     0  2.0G   0% /sys/firmware
```

```sh
(base)root@ubuntu:~\# df --help
使用法: df [オプション]... [ファイル]...
Show information about the file system on which each FILE resides,
or all file systems by default.

Mandatory arguments to long options are mandatory for short options too.
  -a, --all             include pseudo, duplicate, inaccessible file systems
  -B, --block-size=SIZE  scale sizes by SIZE before printing them; e.g.,
                           '-BM' prints sizes in units of 1,048,576 bytes;
                           see SIZE format below
  -h, --human-readable  print sizes in powers of 1024 (e.g., 1023M)
  -H, --si              print sizes in powers of 1000 (e.g., 1.1G)
  -i, --inodes          ブロック使用量の代わりに iノード情報を表示する
  -k                    --block-size=1K と同様
  -l, --local           ローカルファイルシステムのみ表示するように制限する
      --no-sync         使用量の情報を得る前に sync を行わない (デフォルト)
      --output[=FIELD_LIST]  use the output format defined by FIELD_LIST,
                               or print all fields if FIELD_LIST is omitted.
  -P, --portability     use the POSIX output format
      --sync            invoke sync before getting usage info
      --total           elide all entries insignificant to available space,
                          and produce a grand total
  -t, --type=TYPE       limit listing to file systems of type TYPE
  -T, --print-type      print file system type
  -x, --exclude-type=TYPE   limit listing to file systems not of type TYPE
  -v                    (ignored)
      --help     この使い方を表示して終了する
      --version  バージョン情報を表示して終了する

--block-size で指定した SIZE, DF_BLOCK_SIZE, BLOCK_SIZE およびBLOCKSIZE 環境変数
のうち、最初に指定されているサイズ単位で値が表示されます。それ以外の場合、デフォ
ルトの単位は 1024 バイトになります (POSIXLY_CORRECT が設定されている場合 512 バ
イト)。

The SIZE argument is an integer and optional unit (example: 10K is 10*1024).
Units are K,M,G,T,P,E,Z,Y (powers of 1024) or KB,MB,... (powers of 1000).

FIELD_LIST is a comma-separated list of columns to be included.  Valid
field names are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info page).

GNU coreutils online help: <https://www.gnu.org/software/coreutils/>
Report df translation bugs to <https://translationproject.org/team/>
Full documentation at: <https://www.gnu.org/software/coreutils/df>
or available locally via: info '(coreutils) df invocation'

```

---

## <a name='fdisk'>hdd,ssd,パーティションの操作関連</a>

`fdisk`コマンドを実行することでパーティションを操作したりできる。

```sh
# パーティション一覧を表示
fdisk -l
```

- fdisk --help

```sh
(base)root@ubuntu:~# fdisk --help

使い方:
 fdisk [オプション] <ディスク>      パーティションテーブルを変更
 fdisk [オプション] -l [<ディスク>] パーティションテーブルを一覧表示

ディスクのパーティション情報を表示または操作します。

オプション:
 -b, --sector-size <サイズ>    物理および論理セクタサイズ
 -B, --protect-boot            boot　ビットを消さずに新しいラベルを作成します
 -c, --compatibility[=<モード>] 互換モード: 'dos' または 'nondos' (既定値)
 -L, --color[=<いつ>]          メッセージを色づけします(auto、always、never のどれか)
                                 カラー表示はデフォルトで有効です
 -l, --list                    パーティション一覧を表示して終了します
 -o, --output <list>           出力する列を指定します
 -t, --type <種類>             指定した種類のパーティションテーブルのみを認識させます
 -u, --units[=<単位>]          表示単位: 'cylinders' または 'sectors' (既定値)
 -s, --getsz                   デバイスのサイズを 512バイトセクタ数で表示します（非推奨）
     --bytes                   可読性の高い形式ではなく、バイト単位でサイズを表示します
 -w, --wipe <モード>           署名を消去する (auto, always, never のどれか)
 -W, --wipe-partitions <モード>  新しいパーティションから署名を消去する (auto, always, never のどれか)

 -C, --cylinders <数値>        シリンダ数を指定します
 -H, --heads <数値>            ヘッド数を指定します
 -S, --sectors <数値>          1 トラックあたりのセクタ数を指定します

 -h, --help                    display this help
 -V, --version                 display version

Available output columns:
 gpt: Device Start End Sectors Size Type Type-UUID Attrs Name UUID
 dos: Device Start End Sectors Cylinders Size Type Id Attrs Boot End-C/H/S Start-C/H/S
 bsd: Slice Start End Sectors Cylinders Size Type Bsize Cpg Fsize
 sgi: Device Start End Sectors Cylinders Size Type Id Attrs
 sun: Device Start End Sectors Cylinders Size Type Id Flags

詳しくは fdisk(8) をお読みください。
```

---

## <a name='mkfs'>ファイルシステム作成</a>

```sh
mkfs -t <type> <ディスク>
mkfs -t ext4 /dev/sdb1
```

```sh
(base)root@ubuntu:~# mkfs --help

使い方:
 mkfs [オプション] [-t <タイプ>] [fs-options] <デバイス> [<サイズ>]

Linux ファイルシステムを作成します。

オプション:
 -t, --type=<タイプ>  ファイルシステムの種類を指定します。何も指定しない場合、  ext2 が指定されたものとみなします
     fs-options     実際のファイルシステムビルダに渡すオプションを指定します
     <デバイス>       使用すべきデバイスのパスを指定します
     <サイズ>         このデバイスで使用するブロック数を指定します
 -V, --verbose      何が行われるのかを詳しく表示します;
                      2 つ以上 -V を指定すると、実際には何も行わないようになります。
 -h, --help         display this help
 -V, --version      display version

詳しくは mkfs(8) をお読みください。
(base)root@ubuntu:~# 

```
