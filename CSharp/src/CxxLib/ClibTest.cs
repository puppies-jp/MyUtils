using System;
using System.Threading.Tasks;

/** macで実行する場合
* mcs ClibTest.cs
* mono ClibTest.exe
*/

// DllImportに必要
using System.Runtime.InteropServices;

public class HelloWorld
{

    [DllImport("CxxLibrary.so")]
    private static extern void test();

    static public void Main()
    {
        Console.WriteLine("This is C++ Library----");
        test();
        Console.WriteLine("-----------------------");
    }
}

