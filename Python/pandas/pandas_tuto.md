## Pandas how to Kaggle,ML メモ

> このメモは Kaggle or ML にてデータクレンジングを行うためのメモです。

`titanic/train.csv`を前提に作成した。(メジャーかつ取得しやすいので)

[home](https://puppies-jp.github.io/MyUtils/)

---

- csv ファイル読み込み

```
from pandas import read_csv
# index_colを指定することでdataのindexを指定できる。
train_data = read_csv("/kaggle/input/titanic/train.csv",index_col=0)

```

- column の追加は下記のようにすることでできる(他にもあるかも)

```
test_data["Survived"] = gender_data["Survived"]
```

- 不要な column の削除

```
# axis = 1とすることで指定したcolumnの削除となる？
drop_col = ["Name",]
train_data_p= train_data.drop(drop_col,axis=1)
```

```
# info確認
train_data_p.info()
# 先頭の数個のデータを表示
train_data.head()
```

- data からサンプルを取り出す。
  (訓練用データとテストデータを分離できる)

```python
train_dataset = dataset.sample(frac=0.8,random_state=0)
test_dataset = dataset.drop(train_dataset.index)
```

- 訓練用セットのいくつかの列の組み合わせの同時分布を確認する

```python
import seaborn as sns
sns.pairplot(train_dataset[["MPG", "Cylinders", "Displacement", "Weight"]], diag_kind="kde")

# 全てのキーで組み合わせ分布を作成してくれる
import seaborn as sns
sns.pairplot(train_data_p[train_data_p.keys()],diag_kind="kde")
```

```python
# key取得
print(train_data_p.keys())
print(data.keys())
# index指定取得
print(ans.iloc[0:3])
```

- one-hot 形式に datatable を変更する。
  - `columns`を指定することで自動で one-hot 形式にしてくれる。(column 名は適当につけてくれる。)

```python
cabin_one_hot=pd.get_dummies(train_data_p,columns=["Sex","Cabin","Embarked","Pclass"],)
cabin_one_hot.head()
```

- datatable の要素に対して正規表現を適用する。
  - 最初の一文字目だけを取得し、`Cabin`に上書きした。

```python
train_data_p["Cabin"] = train_data_p["Cabin"].str.extract('(?P<cabin>^.)',expand=False).fillna("no_data")
train_data_p.head()
```
