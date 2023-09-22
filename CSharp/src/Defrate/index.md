# GzipStream を使う

GZipStreamを使うことでDiskを経由せずに
メモリ上のデータを圧縮したbyteデータを扱うことができる。

## <a name=defrate_mem>MemoryStreamをGZipで圧縮する</a>

```cs
/// GZipStreamを経由してmemorystreamに書き込むことで
/// memorystreamに圧縮されて格納される
var stream = new MemoryStream();
using {var compressor = new GZipStream(stream, CompressionLevel.SmallestSize, leaveOpen: false);
    compressor.Write(s_messageBytes, 0, s_messageBytes.Length);
}

/// memory stream上に取り出す
/// 圧縮したデータを取り出す場合は、書き込んだストリームのpositionを0にして取り出す。
MemoryStream memory = new MemoryStream();
stream.Position = 0;
stream.CopyTo(memory);

/// byte配列で取り出す。 
Stream stream1 = compressor.BaseStream;
var temp = new byte[stream1.Length];
stream1.Position = 0;
stream1.Read(temp, 0, temp.Length);
```

## <a name=infrate_mem>GZipしたメモリを展開する</a>

```cs
/// 圧縮したbyte[]なりmemory streamを用意する
Stream stream
using (var decompressor = new GZipStream(stream, CompressionMode.Decompress))
{
    /// 出力先配列
    stream.Position=0;
    byte decompressedBytes[bufferSize]; 
    int length = decompressor.Read(decompressedBytes, 0, bufferSize);
    
    /// 出力先メモリストリーム
    stream.Position=0;
    MemoryStream memory = new MemoryStream();
    decompressor.CopyTo(memory);
}

```
