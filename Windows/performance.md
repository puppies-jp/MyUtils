# パフォーマンスについて

パフォーマンスの改善についてのメモです。

[systemプロセスが重い場合](#system)
[パフォーマンスログ](#performance)

## 使い方周り

1. SQL Serverの負荷が高い？ -> 悪いと言えば悪いが、そこまでっぽい
    1. クエリの実行で表領域が大きくなることでパフォーマンスの低下が見られている。
    2. クエリのパフォーマンスを見直すべきだが、主な要因でないかもしれない。
    3. やるべきことは以下
        - クエリの見直し
        - indexが適切に運用されているか確認(断片化されてないかとか、、)
2. McAfee
    [マカフィー製品をインストールしたパソコンでCPUの使用率やディスクの使用率が急激に上昇した場合](https://www.mcafee.com/support/?locale=ja-JP&articleId=TS102870&page=shell&shell=article-view)
3. Acronis,ManicTimeの負荷が高い？
    対策未検討

---
---

## PCの設定等

1. 電源モードが低パフォーマンスになっている？
    「start」右クリック->「電源option」-> “高パフォーマンス“に変更
2. スタートアップ アプリが多い？
    減らせ

3. 負荷のかかる設定は無効になっているか？
    1. Windows Updateの自動更新
        [サービスを無効にする手順](https://itojisan.xyz/trouble/13235/)
        サービス/スタートアップの種類
        - 自動 :Windows の起動時に自動的に起動
            「自動(遅延開始)」を選択した場合は、 Windows の起動後に少しだけ待ってから起動
        - 手動 :「手動」にした場合は自動では起動せずに手動で起動したり他のサービスから起動されるまで開始しません。
        - 無効 :「自動」や「手動」に切り替えるまでを起動することができません。

    2. 配信の最適化 やってないが、問題ないか確認する -> いらないと思う、
        配信の最適化とは、別のPCにローカルネットワーク、またはインターネット経由で更新プログラムやアプリを配信します。 更新プログラムやアプリが必要なPCは、ネットワーク上から既にデータを取得したPCを検出して、データをダウンロードすることができます。

    3. `off`にするには
        - [スタート] 、[設定] 、[更新とセキュリティ] 、[詳細オプション]、[配信の最適化] の順に選択します。 [他の PC からダウンロードを許可する] の下のトグルをオフ

4. 今一番重そうだったプロセスは?
    以下が重そうだった(他の処理稼働中の状況は未確認)
    - [system](#system)
        [Troubleshoot High CPU usage by the "System" process](https://superuser.com/questions/527401/troubleshoot-high-cpu-usage-by-the-system-process)
        - `Process Explorer`で`System`プロセスの重い処理を確認できる
    - sql server
    - lsass.exe
    [Lsass.exe が重い場合のトラブルシューティング](https://learn.microsoft.com/ja-jp/troubleshoot/windows-server/identity/troubleshoot-high-lsass.exe-cpu-utilization)
        - ActiveDirectryで使用するプロセスらしい
    - svchost
        [svchost.exe の CPU 高負荷およびメモリー使用量増加の調査](https://jpwinsup.github.io/blog/2021/02/08/Performance/SystemResource/SeparatingSvcHostServices/)

---
---

## <a name=performance>パフォーマンス ログ収集</a>

[パフォーマンス ログ収集(Microsoft Support)](https://jpwinsup.github.io/blog/2021/06/07/Performance/SystemResource/PerformanceLogging/)

---
---

## <a name=logrotate>ログのローテート方法(cmd)</a>

[ローテートバッチ参照](https://beyondjapan.com/blog/2023/03/winlogrotate/)

```bat
@echo off
setlocal
 
set LOGDIR="C:\Users\testuser\Documents\test"
 
forfiles /P "%LOGDIR%" /M *.log /C "cmd /C Del /S @path" /D -14
 
exit /B 0
```

```bat
rem /s フォルダにファイル,ディレクトリがあっても削除する
rem /q 警告を表示しない
rd /s /q <フォルダ>

rem フォルダの削除もできるらしい
rem D:\〇〇〇 パス
forfiles /P D:\〇〇〇 /d -365 /C "cmd /c IF @isdir==TRUE rmdir /S /Q @file"
```

---
---

## <a name=system>systemプロセスの動作が重い場合の対処</a>

まず、systemプロセスが重い場合、以下の原因が考えられる。

- サービスの負荷が高い
- I/O負荷が高い
- ハードウェアドライバが調子悪い
  - ハードウェアドライバ: ハードウェア↔︎OS間の通信ソフトウェア
  - デバイスドライバ: 特定のハードウェアデバイスを制御するソフトウェア
  - デバイスドライバの一覧とsysファイルがどのドライバかは以下で確認できる。

---

### **ドライバの負荷が高い場合の対象**

1~3はデバイスマネージャがやってくれる。

1. ドライバの更新
2. ドライバの再インストール
    - アンインストール後、デバイスを検出すればOSがインストールするらしい
3. ドライバの無効化

4. ウイルスの場合には、スキャン、、

※ infファイル、sysファイルの違い

- infファイル ドライバのインストールに使うファイル
- sysファイル ドライバの本体

```bat
rem /fo [table,csv,list]
rem 🌟 /v でsysファイルなどの詳細が確認できる
driverquery /v /fo [任意] > driverlist.txt

rem 🌟 /SI でinfファイルなどの詳細が確認できる
driverquery /SI /fo [任意] > driverlist.txt


rem ドライバのアンインストール
pnputil /delete-driver ドライバーのINFファイル名
pnputil /add-driver ドライバーのINFファイル名

rem infファイル名は以下で確認できる、、
pnputil /enum-drivers
```

---

### ドライバなどの負荷が高い場合の確認方法と対策

1. **`ProcessExplore`**
    1. System を右クリック -> プロパティをクリック
    2. Threads
    3. CPU負荷の高いスレッドを特定する。
        - `suspend`でスレッドを一時停止し、`resume`で再開できる。
    (正しい確認方法は下らしい、、、)

2. **`Windows Performance Toolkit(Windows SDK)`**
    1. `WPRUI.exe`を実行
        - First Level選択
        - CPU usage 選択
    2. click on start.
        - CPU使用時間のキャプチャが記録される
    3. 十分記録したら **`save`** をクリック
        - **`analyze the generated ETL file`** とか言うのが生成されるらしい。
        (ファイルが作られるから、確認は別PCでもできる？)
        - **`Windows Performance Analyzer`** で確認できるとか、、
    4. 負荷の高いやつの判定はめんどそうなので、後日書く
