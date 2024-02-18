# Luaについて

luaファイルの書き方についてまとめる。

```lua
-- 1.プロトコルを定義する
myproto = Proto("MyProto", "My Protocol")

-- 2.プロトコル解析用の関数を定義する
function myproto.dissector(buffer, pinfo, tree)
  pinfo.cols.protocol = "MyProto" -- (1)
  local subtree = tree:add(myproto, buffer(), "My Protocol Data") -- (2)
  subtree:add(buffer(0,2),"command: ", buffer(0,2):uint()) -- (3)
  subtree:add(buffer(2,1),"flag: ", buffer(2,1):uint()) -- (4)
  -- (5)🌟 le_でlittle Endianでintにできる
  subtree:add(buffer(2,1),"flag: ", buffer(2,1):le_uint()) 

end

-- 3.TCPのポートとプロトコルを紐づける
tcp_table = DissectorTable.get("tcp.port") -- TCPポートのテーブルを取得する
tcp_table:add(777,myproto) -- TCPの777番ポートとプロトコルの紐付けする
```

## ビット操作について

|操作|関数|使い方|
|-|-|-|
|and|bit.band|bit.band(0x01,0x03)|
|or|bit.bor|bit.bor(0x0x1,0x03)|
| >> |bit.rshift|bit.rshift(0x02,1)|
| << |bit.lshift|bit.lshift(0x01,1)|
