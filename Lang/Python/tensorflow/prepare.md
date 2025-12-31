# データセット作成(前処理)

## 基本的なデータの型について

- numerical
- categorical

## 画像を扱うとき tips

```python
img_size = (299, 299)
# 🌟 `img` is a PIL image of size 299x299
img = keras.preprocessing.image.load_img(img_path, target_size=size)
# `array` is a float32 Numpy array of shape (299, 299, 3)
# 🌟 ここでnumpy arrayに 変換できる
array = keras.preprocessing.image.img_to_array(img)
```

## Array(画像,配列)のデータを扱う時 tips

```python
# 🌟(60000, 28, 28) だったものが、
input_shape = x_train.shape
print("Shape befor:",input_shape)

# 🌟(60000, 28, 28, 1) に拡張することができる
# tfはchannel first or lastに変えてやる必要があるため
x_train= x_train[:,:,:,np.newaxis]
input_shape = x_train.shape
print("Shape after:",input_shape)

```

## 構造体のデータを扱うとき
