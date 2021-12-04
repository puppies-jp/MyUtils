# ランタイム Hack

- ここでは Binnary Hack で読んだランタイム Hack についてまとめる

1. [プログラムが main()に辿り着くまで](#start)
2. [共有ライブラリを差し替える(LD_PRELOAD)](#PRELOAD)
3. [trace 機能](#trace)
4. [プログラムの起動高速化](#prelink)
5. [実行中のプロセスにパッチを当てる](#livepatch)

---

## <a name="start">プログラムが main()に辿り着くまで</a>

- まず、Linux において通常のプログラムが main 関数に辿り着くまで

## <a name="PRELOAD">共有ライブラリを差し替える(LD_PRELOAD)</a>

- 共有ライブラリを差し替えて実行する方法をまとめる。

## <a name="unwind">unwind についてまとめる</a>

- コールチェインを制御する。

## <a name="trace">trace 機能</a>

- システムの trace についてまとめる
  - strace
  - ltrace
  - Jockey

## <a name="prelink">prelink でプログラムの起動を高速化する</a>

## <a name="livepatch">実行中のプロセスにパッチを当てる</a>

- livepatch
