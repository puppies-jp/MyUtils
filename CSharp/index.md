# CSharpいろいろメモ

- [async/awaitまとめ](#async_await)

## <a name="async_await">async/awaitまとめ</a>

- [awaitまとめ](#await)
- [asyncまとめ](#async)

### <a name="await">awaitまとめ</a>

- 非同期処理しているタスクの完了を待つ場合はawaitを使う

```cs
// タスクBに戻り値がない(void)場合
await RunTaskBAsync(); 
// タスクBに戻り値がある場合
var returnValue = await RunTaskBAsync(); 
```

```cs
var result = await RunTaskBAsync();// ここで完了待ちをして
Console.WriteLine(result); // 続きの処理を実行する。

// 🌟例外処理もこんな感じ
try
{
  await RunTaskBAsync();
}
catch (TaskCanceledException ex)
{
  // キャンセルされた場合の例外処理
}
catch (Exception ex)
{
  // 異常終了した場合の例外処理
}
```

```cs
// awaitをつけていないのでTask<int>型を受け取る
Task<int> taskB = RunTaskBAsync(); 

/* --------------------------------------------- */
/* 🌟何か別の処理 (この間もタスクBは非同期で実行されている) */
/* --------------------------------------------- */

// ここでタスクBの完了を待つ
int result = await taskB; 
```

### <a name="async">asyncまとめ</a>

- ルール
  1. メソッドにasyncキーワードを付与
  2. 戻り型としてTask型を利用
      - returnする場合: `Task<戻り値の型>`
      - returnしない場合: `Task`

```cs
public async Task<int> RunTaskAAsync()
{
    int result = await RunTaskBAsync();
    // 戻り値はTask型で返してなくても勝手にラップされる
    return result + 1; // taskBの戻り値を使った計算例
}
```

```cs
// 🌟実行例
// タスクA -> タスクB -> タスクC の非同期呼び出し
public async Task<int> RunTaskAAsync()
{
  var result = await RunTaskBAsync(); // taskBの完了を待つ
  return result + 1;
}

private async Task<int> RunTaskBAsync()
{
  await RunTaskCAsync(); // taskCの完了を待つ
  return 1 + 2 + 3;
}

private async Task RunTaskCAsync()
{
  await Task.Delay(500); // 0.5秒待機
}
```

```cs
// 🌟実行例
// taskB,taskCを実行し、同時に終了待ちする。
async Task<int> RunTaskAAsync()
{
  Task<int> taskB = RunTaskBAsync();
  Task<int> taskC = RunTaskCAsync();
  
  int[] results = await Task.WhenAll(taskB, taskC); // 両方完了後、戻り値が配列として返る
  
  return results[0] + results[1]; // タスクBとタスクCの結果を加算
}

async Task<int> RunTaskBAsync()
{
  await Task.Delay(100); // 0.1秒待機
  return 1 + 2 + 3;
}

async Task<int> RunTaskCAsync()
{
  await Task.Delay(200); // 0.2秒待機
  return 4 + 5 + 6;
}
```