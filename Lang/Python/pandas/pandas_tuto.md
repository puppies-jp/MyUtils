# Pandas how to Kaggle,ML メモ

> このメモは Kaggle or ML にてデータクレンジングを行うためのメモです。

`titanic/train.csv`を前提に作成した。(メジャーかつ取得しやすいので)

[home](https://puppies-jp.github.io/MyUtils/)

---

## pandas の基本的な操作

- csv ファイル読み込み

```python
from pandas import read_csv
# index_colを指定することでdataのindexを指定できる。
train_data = read_csv("/kaggle/input/titanic/train.csv",index_col=0)
```

- column の追加は下記のようにすることでできる(他にもあるかも)

```python
test_data["Survived"] = gender_data["Survived"]

# 下記のようにすることで Sex(column)がmaleのdataのみ取得できる。
train_data[train_data["Sex"] == "male"]
```

- 不要な column の削除

```python
# axis = 1とすることで指定したcolumnの削除となる？
drop_col = ["Name",]
train_data_p= train_data.drop(drop_col,axis=1)
```

```python
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

```python
# key取得
print(train_data_p.keys())
print(data.keys())
# index指定取得
print(ans.iloc[0:3])
```

```python
# index(数値で検索の掛け方)(他のcolumnの代わりにindexで指定するだけ)
test_data_p[test_data_p.index > 891]
```

---

## データクレンジングなどに使うやつ

- one-hot 形式に datatable を変更する。
  - `get_dummies`を使った方法`columns`を指定することで自動で one-hot 形式にしてくれる。(column 名は適当につけてくれる。)
  - loc を使った方法(条件 False が nan となる。)
  - np.where を使った方法(条件の true/false で値を設定できる。)

```python
# get_dummies を使った方法
cabin_one_hot=pd.get_dummies(train_data_p,columns=["Sex","Cabin","Embarked","Pclass"],)
cabin_one_hot.head()

# locを使った方法で分けることもできる(ただし条件がFalseだった側がnanとなる。)
tmp.loc[tmp["Sex"] == "male","male"]=1
tmp.loc[tmp["Sex"] != "male","female"]=1
tmp.head()

# np.whereを使った方法(割と直感的でおすすめ)
tmp["male"] = np.where(tmp["Sex"] == "male",1,0)
tmp["female"] = np.where(tmp["Sex"] != "male",1,0)
tmp.head()
```

- `null`の検索での引っ掛け方
  - こんな感じで ok

```python
# fix Age # Nan Value replace -1 and add columns Validate Age Flug
total_data["ValAge"] = np.where((total_data.Age.isnull() == True), 1,0)
total_data.Age = np.where(total_data.ValAge == 1, -1 ,total_data.Age)
total_data.tail()
```

- datatable の要素に対して正規表現を適用する。
  - 最初の一文字目だけを取得し、`Cabin`に上書きした。

```python
train_data_p["Cabin"] = train_data_p["Cabin"].str.extract('(?P<cabin>^.)',expand=False).fillna("no_data")
train_data_p.head()
```

---

## 割と便利な機能

- 訓練用セットのいくつかの列の組み合わせの同時分布を確認する

```python
import seaborn as sns
sns.pairplot(train_dataset[["MPG", "Cylinders", "Displacement", "Weight"]], diag_kind="kde")

# 全てのキーで組み合わせ分布を作成してくれる
import seaborn as sns
sns.pairplot(train_data_p[train_data_p.keys()],diag_kind="kde")

# sns.pairplot(cabin_one_hot[cabin_one_hot.keys()[0:6]],diag_kind="kde")
#cabin_one_hot.hist()

# indexを指定した組み合わせも作成できる。
idx = pd.Index(["Survived","Age","Fare","Sex_male","Sex_female"])
sns.pairplot(cabin_one_hot[idx],diag_kind="kde", kind='reg',hue="Survived")
```
