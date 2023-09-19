using System;
using System.IO;
using System.IO.Compression;
using System.Text;

public static class MemoryWriteReadExample
{
    private const string Message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    private static readonly byte[] s_messageBytes = Encoding.ASCII.GetBytes(Message);
    public static MemoryStream _stream;

    public static void Main()
    {
        Console.WriteLine($"The original string length is {s_messageBytes.Length} bytes.");
        _stream = new MemoryStream();
        CompressBytesToStream(_stream);
        Console.WriteLine($"The compressed stream length is {_stream.Length} bytes.");
        int decompressedLength = DecompressStreamToBytes(_stream);
        Console.WriteLine($"The decompressed string length is {decompressedLength} bytes, same as the original length.");
        /*
         Output:
            The original string length is 445 bytes.
            The compressed stream length is 282 bytes.
            The decompressed string length is 445 bytes, same as the original length.
        */
    }

    private static void CompressBytesToStream(Stream stream)
    {
        var compressor = new GZipStream(_stream, CompressionMode.Compress);
        //var compressor = new GZipStream(stream, CompressionLevel.SmallestSize, leaveOpen: true);
        compressor.Write(s_messageBytes, 0, s_messageBytes.Length);
        Console.WriteLine(_stream.Position);
    }

    private static int DecompressStreamToBytes(Stream stream)
    {
        _stream.Position = 0;
        int bufferSize = 512;
        byte[] decompressedBytes = new byte[bufferSize];
        var decompressor = new GZipStream(_stream, CompressionMode.Decompress);
        int length = decompressor.Read(decompressedBytes, 0, bufferSize);
        return length;
    }
}