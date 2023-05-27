# VBAについて

大変不本意だが、VBA,マクロについてまとめる。

* VBAとマクロの違いとは
`VBA（Visual Basic for Applications）`はExcelやAccessなどのOffice製品で利用できる`プログラミング言語`です。
一方の`マクロ`はコンピューターの操作を自動化する`機能そのもの`を指します。

つまり、`「マクロを作成するための手段の1つがVBA」`

[変数](#valiable)
[関数](#func)
[Excel VBAでよく使うオブジェクトについて](#excel)

---
---

## <a name=valiable>変数(Valiable)</a>

VBAには一般的なプログラミングの型に加えていくつか特殊な型がある。
それらを踏まえて変数の宣言方法をいかに示す。

```vba
'一般的な型
Dim old As Integer
Dim name As String
Dim dnum As Double

'リスト
Dim names(5) As String
names(1) = "1" '🌟1~5の数字でインデックスを指定するらしい、、
names(2) = name(1)&"2" '🌟文字列の連結は"&"
names(5) = "5"

'通貨型
Dim curr As Currency
'日付型
Dim _date As Date
'オブジェクト型(オブジェクト参照するデータ型)
Dim obj As Object

'バリアント型(あらゆる種類の値を保存できる型)
'🚨 As ~~を省略した場合も "Variant"型となる。
Dim var As Variant

'🌟他、ユーザー定義型がある。
```

### 変数への代入について

オブジェクトで定義した変数への代入は`Set`を使う。
整数や文字列を代入する場合は正式には `Let(省略可)` を使用して代入する

```vba
'オブジェクトをデータ型として宣言した変数に対してオブジェクトを代入することができますが、
'オブジェクトを変数に代入する場合は
'次のように Set 文を使用して代入する必要があります。 

Sub テスト()
    Dim r As Range 
    '🚨Rangeオブジェクトに対して代入するので、Setが必要
    Set r = Range("B2")

    '🌟 整数や文字列なので、Letで代入する。(省略で問題ない)
    Dim num As Integer
    Let num = 20
    num = 30
End Sub
```

---
---

## <a name=func>関数について</a>

```vba
'サブプロージャ関数のようなもの 
Sub Test() '戻り値を返さない
 Debug.Print "テスト"
End Sub
'🚨 👆End で括ること
```

---
---

## スコープについて

他の言語と特に違いはなさそう。
globalに宣言した場合はグローバルに、関数内は関数内でのみ

```vba
```

---

## <a name=excel>Excel VBAでよく使うオブジェクトについて</a>

|オブジェクト|説明|
|--|--|
|Application|Excel アプリケーション全体|
|Workbooks|開いているすべてのブック|
|Workbook|Excel ブック|
|Worksheets|ブックに含まれるすべてのワークシート|
|Worksheet|ワークシート|
|Range|セル、行、列など|
|Chart|グラフ|
|Dialog|組み込みダイアログボックス|

他にもいろいろあるが、キリが無いので割愛適宜調べて追加する。

```vba
' 🌟オブジェクトの階層構造
'Application
' +- Workbook(s)
'     +- Worksheet(s)
'         +- Range
'             +- Font など

Sub テスト()
    Dim ws As Worksheet
    '# Worksheetsからindex:1を取り出す
    Set ws = Worksheets(1) 
    ' 名前指定取り出し
    Set ws = Worksheets("Sheet2")

    ' Workブックを取り出す。
    Dim wb As Workbook
    Set wb = Workbooks(1)
    Set wb = Workbooks("Book1.xlsm")

    ' オブジェクト階層を跨いだアクセス
    Dim r As Range
    Set r = Application.Workbooks(1).Worksheets(2).Range("A2")
End Sub
```
