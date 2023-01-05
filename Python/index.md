# python index

- データサイエンス関連
  - [pandas](pandas/pandas_tuto)
  - [matplot](pandas/matplot)

- [RestAPI](RestAPI)
- [非同期処理について](#async)

---

## <a name=async>非同期処理について</a>

pythonでもasync/awaitの書き方が追加されたらしいので、書き方をまとめておく

```python
import asyncio

async def hello_world(n):
    # 🌟awaitはasync内でしか使えない
    # 1sec sleep(🚨安易にtime.sleepすると他スレッドもsleepする。
    # 非同期の重い処理代わりにsleepする場合はこれを使うこと)
    await asyncio.sleep(1) 
    print("{}: Hello World!".format(n))

async def call_hello_world1():
    print("call_hello_world1()")
    await hello_world(1)

async def call_hello_world2():
    print("call_hello_world2()")
    await hello_world(2)

# 🌟メインスレッドでのスレッド立ち上げ/完了待ちは以下で行うらしい
loop = asyncio.get_event_loop()
loop.create_task(call_hello_world1())
loop.run_until_complete(call_hello_world2())
```
