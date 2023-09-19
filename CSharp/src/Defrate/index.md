# GzipStream を使う

GZipStreamを使うことでDiskを経由せずに
メモリ上のデータを圧縮したbyteデータを扱うことができる。

## <a name=defrate_mem>MemoryStreamを圧縮する</a>

```cs
/// G
var stream = new MemoryStream();
var compressor = new GZipStream(stream, CompressionLevel.SmallestSize, leaveOpen: true);
compressor.Write(s_messageBytes, 0, s_messageBytes.Length);

/// memory stream上に取り出す
MemoryStream memory = new MemoryStream();
stream.Position = 0;
stream.CopyTo(memory);

/// byte配列で取り出す。 
Stream stream1 = compressor.BaseStream;
var temp = new byte[stream1.Length];
stream1.Position = 0;
stream1.Read(temp, 0, temp.Length);
```

## <a name=infrate_mem>MemoryStreamを圧縮する</a>
