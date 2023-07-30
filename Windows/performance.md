# パフォーマンスについて

パフォーマンスの改善についてのメモです。

## 使い方周り

1. SQL Serverの負荷が高い？
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
    2. 配信の最適化 やってないが、問題ないか確認する -> いらないと思う、
        配信の最適化とは、別のPCにローカルネットワーク、またはインターネット経由で更新プログラムやアプリを配信します。 更新プログラムやアプリが必要なPCは、ネットワーク上から既にデータを取得したPCを検出して、データをダウンロードすることができます。

    3. `off`にするには
        - [スタート] 、[設定] 、[更新とセキュリティ] 、[詳細オプション]、[配信の最適化] の順に選択します。 [他の PC からダウンロードを許可する] の下のトグルをオフ

4. 今一番重そうだったプロセスは?
    以下が重そうだった(他の処理稼働中の状況は未確認)
    - system
        [Troubleshoot High CPU usage by the "System" process](https://superuser.com/questions/527401/troubleshoot-high-cpu-usage-by-the-system-process)
        - `Process Explorer`で`System`プロセスの重い処理を確認できる
    - sql server
    - lsass.exe [Lsass.exe が重い場合のトラブルシューティング](https://learn.microsoft.com/ja-jp/troubleshoot/windows-server/identity/troubleshoot-high-lsass.exe-cpu-utilization)
        - ActiveDirectryで使用するプロセスらしい
