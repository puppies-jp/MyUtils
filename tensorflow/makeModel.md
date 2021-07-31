# tensorflow 使い方メモ その 1

- このメモでは tensorflow の使い方についてまとめる。

  - 多くなることが予想されるため、その 1 としてモデルの作成方法などをまとめる。

- [モデル作成方法](#makeModel)
  - 学習モデルの input_shape,layer,output_shape を決定する。
- [コンパイル方法](#compileModel)
  - 学習モデルの optimizer,loss 関数、監視する metric を決定する。
  ***

## <a name=makeModel>モデル作成方法</a>

```python
# 基本的にはこんな感じ、HyperparameterTuneでは別の作成方法がある。(基本的に同じだが、、、)

from tensorflow import keras
from tensorflow.keras import layers

model = keras.Sequential(
    [
        keras.Input(shape=input_shape),
        layers.Conv2D(32, kernel_size=(3, 3), activation="relu"),
        layers.MaxPooling2D(pool_size=(2, 2)),
        layers.Conv2D(64, kernel_size=(3, 3), activation="relu"),
        layers.MaxPooling2D(pool_size=(2, 2)),
        layers.Flatten(),
        layers.Dropout(0.5),
        layers.Dense(num_classes, activation="softmax"),
    ]
)

model.summary()

# モデル画像 のoutputは以下でできる。
keras.utils.plot_model(model, "my_first_model_with_shape_info.png", show_shapes=True)
```

## <a name=compileModel>モデルのコンパイル</a>

```python
model.compile(
    loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True),
    optimizer=keras.optimizers.RMSprop(),
    metrics=["accuracy"],
)
```
