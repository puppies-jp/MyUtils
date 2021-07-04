# 正規表現の備忘録

- 正規表現のメモです。よく使う正規表現はメモを残す予定

[home](https://puppies-jp.github.io/MyUtils/)

---

## よく使うパターンマッチ

- IP アドレス
  - ^((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])$

```python
>>>import re
regexp = "^((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])$"
>>>
>>> exp = True if  re.search(regexp,"255.255.255.255") else False
>>> print(exp)
True
>>> exp = True if  re.search(regexp,"255.255.255.256") else False
>>> print(exp)
False
>>>
```

- mac アドレス

  - (?:[0-9a-fA-F]{2}¥:){5}[0-9a-fA-F]{2}

- white space 空白 space
  - \s

```python
import re
>>> regexp = "^\s(.*)\s$"
>>> res = re.search(regexp," hogehoge ")
>>> res.group()
' hogehoge '
>>> res.group(1)
'hogehoge'
>>>

```

- uuid

  - [0-9a-f]{8}([:-][0-9a-f]{4}){3}[:-][0-9a-f]{12}
  - -version
    - [0-9a-f]{8}(-[0-9a-f]{4}){3}-[0-9a-f]{12}
  - :version
    - [0-9a-f]{8}(:[0-9a-f]{4}){3}:[0-9a-f]{12}
  - prefix 取得 ver
    - ([0-9a-f]{2})([0-9a-f]{2})[0-9a-f]{4}([:-][0-9a-f]{4}){3}[:-][0-9a-f]{12}

```python
>>>
>>> reg = "([0-9a-f]{2})([0-9a-f]{2})[0-9a-f]{4}([:-][0-9a-f]{4}){3}[:-][0-9a-f]{12}"
>>> xi = re.search(reg,hogeid2)
>>> print(xi.lastindex,xi.group(1),xi.group(2),xi.group(3))
3 22 11 :bd5a
>>> pref = "{}/{}/{}".format(xi.group(1),xi.group(2),xi.string)
>>> print(pref)
22/11/2211606c:3460:489e:bd5a:07c028afc05f
>>>
```

- shell 正規表現(uuid 1~2 桁目,3~4 番目抽出し/をつけて,結合するパターン)

```shell
user@host WorkSpace % echo $tmp
920e0776-e998-47d2-aac6-bd8962f3f811
user@host WorkSpace % reg='s/(([0-9a-f]{2})([0-9a-f]{2})[0-9a-f]{4}([-][0-9a-f]{4}){3}[-][0-9a-f]{12})/\2\/\3\/\1/g'
user@host WorkSpace % echo  $tmp | sed -r $reg
92/0e/920e0776-e998-47d2-aac6-bd8962f3f811
user@host WorkSpace % pref_tmp=`echo  $tmp | sed -r $reg`
user@host WorkSpace % echo $pref_tmp
92/0e/920e0776-e998-47d2-aac6-bd8962f3f811
```
