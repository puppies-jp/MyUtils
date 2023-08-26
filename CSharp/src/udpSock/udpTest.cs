using System;
using System.Threading.Tasks;

public class UdpClass
{

    public static void Main()
    {
        Task.Run(
            Recv
        );
        Send();
    }

    //バインドするローカルIPとポート番号
    static string localIpString = "127.0.0.1";
    static int localPort = 2002;

    public static void Recv()
    {

        System.Net.IPAddress localAddress =
            System.Net.IPAddress.Parse(localIpString);
        //UdpClientを作成し、ローカルエンドポイントにバインドする
        System.Net.IPEndPoint localEP =
            new System.Net.IPEndPoint(localAddress, localPort);
        System.Net.Sockets.UdpClient udp =
            new System.Net.Sockets.UdpClient(localEP);


        for (; ; )
        {
            //データを受信する
            System.Net.IPEndPoint remoteEP = null;
            byte[] rcvBytes = udp.Receive(ref remoteEP);

            //データを文字列に変換する
            string rcvMsg = System.Text.Encoding.UTF8.GetString(rcvBytes);

            //受信したデータと送信者の情報を表示する
            Console.WriteLine("受信したデータ:{0}", rcvMsg);
            Console.WriteLine("送信元アドレス:{0}/ポート番号:{1}",
                remoteEP.Address, remoteEP.Port);

            //"exit"を受信したら終了
            if (rcvMsg.Equals("exit")) break;
        }

    }

    public static void Send()
    {

        //UdpClientオブジェクトを作成する
        System.Net.Sockets.UdpClient udp =
            new System.Net.Sockets.UdpClient();

        for (; ; )
        {
            //送信するデータを作成する
            Console.WriteLine("送信する文字列を入力してください。");
            string sendMsg = Console.ReadLine();
            byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes(sendMsg);

            //リモートホストを指定してデータを送信する
            udp.Send(sendBytes, sendBytes.Length, localIpString, localPort);

            //"exit"と入力されたら終了
            if (sendMsg.Equals("exit")) break;
        }

        //UdpClientを閉じる
        udp.Close();

        Console.WriteLine("終了しました。");
        Console.ReadLine();

    }

}