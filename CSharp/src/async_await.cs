using System;
using System.Threading.Tasks;

/** macで実行する場合
* mcs async_await.cs
* mono async_await.exe
*/

public class HelloWorld
{
    static public void Main ()
    {
        Console.WriteLine ("Hello Mono World");
        Task res = RunExce();
        try{
          res.Wait();
        }catch(Exception e){
          Console.WriteLine("RunExce Exception");
        }
          Console.WriteLine("IsCompleted:\t"+res.IsCompleted);
          Console.WriteLine("IsCompletedSuccessfully:\t"+res.IsCompletedSuccessfully);
          Console.WriteLine("IsFaulted:\t"+res.IsFaulted);
          Console.WriteLine("IsCanceled:\t"+ (res.IsCanceled));
      
        Task<int> res2 = RunTaskAAsync();
        res2.Wait();
        int result = res2.Result; 
        Console.WriteLine("Result "+ result);

    }

    static public async Task RunExce(){
      await RunTaskAAsync();
      throw new Exception("Async Exception");
    }
    static public async Task<int> RunTaskAAsync()
    {
      Task<int> taskB = RunTaskBAsync();
      Task<int> taskC = RunTaskCAsync();
      int[] results = await Task.WhenAll(taskB, taskC); // 両方完了後、戻り値が配列として返る
      return results[0] + results[1]; // タスクBとタスクCの結果を加算  
    }

static private async Task<int> RunTaskBAsync()
{
  Console.WriteLine("TaskB Start");
  await Task.Delay(100); // 0.1秒待機
  Console.WriteLine("TaskB Finish");
  return 1 + 2 + 3;
}

static private async Task<int> RunTaskCAsync()
{
  Console.WriteLine("TaskC Start");
  await Task.Delay(200); // 0.2秒待機
  Console.WriteLine("TaskC Finish");
  return 4 + 5 + 6;
}
}

