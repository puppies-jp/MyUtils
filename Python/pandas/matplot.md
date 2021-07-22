# how to matplotlib

- この markdown では matplotlib の使い方をまとめる
  (特に kaggle でよく使う、便利なものなどを中心にまとめる)

- [ヒストグラム](#hist)
- [棒グラフ](#bar)
- [subplot](#subplot)

---

## <a name=hist>histgram</a>

```python
_ = plt.hist(
    [train_data_p[train_data_p["Survived"]==1]["Pclass"],
     train_data_p[train_data_p["Survived"]!=1]["Pclass"]],
     label=["Survied","Dead"], color= ["red","blue"],
     stacked=True, # trueにすることで積み上げることができる。
)
plt.legend()
plt.xlabel("Pclass")
plt.ylabel("Count")
plt.xticks([1,2,3],["Pclass{}".format(i) for i in range(1,4) ])
plt.show()
```

## <a name=bar>棒グラフ</a>

```python
plt.bar([i for i in range(len(colums))],mean_value,width = 1.)
plt.ylabel(r"$\frac{\partial y}{\partial x_i}$",fontsize=18)
plt.xlabel(r"$columns: x_i$")
plt.grid()
_ = plt.xticks([i for i in range(len(colums))],colums,rotation=-90)

```

## <a name=subplot>subplot</a>

```python
fig, axes = plt.subplots(1, 5, figsize=(20,20))
axes = axes.flatten()
for img, ax in zip( images_arr, axes):
    ax.imshow(img, cmap="gray") # imshow で画像の表示ができる
    ax.axis('off')
plt.tight_layout()
plt.show()
```
