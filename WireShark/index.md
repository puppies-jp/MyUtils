# WireShark

- Wiresharkとは

ネットワークパケット解析ツール「Wireshark」
GUIが利用できない場合は使いにくいが、tshark,tcpdumpといったでCUIツールもある。
また、tsharkは取得したキャプチャをWiresharkと連携させることもできる。
tshark,tcpdumpはまた別の機会で、`今回は自作プロトコルを表示する方法`について書いていく

## 表示するパケットについて

wiresharkはluaファイルを設定することで自身で作成したプロトコルに合わせて表示することができる。
デフォルトで以下のパスにinstallされinit.luaがここに格納される。

> C:\Program Files\Wireshark\

init.luaに自作のluaのパスを追加することで新規のプロトコル表示させることができる

```lua
dofile(DATA_DIR.."myprotocol.lua")
```

サンプルとしてはこんな感じ  
[公式サンプル](https://www.wireshark.org/docs/wsdg_html_chunked/wslua_dissector_example.html)

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

## <a name=func>関数</a>

関数の作り方としてはこんな感じ

```lua
function p_multi.dissector(buf, pkt, tree)

        local subtree = tree:add(p_multi, buf(0,2))
        subtree:add(f_proto, buf(0,1))
        subtree:add(f_dir, buf(1,1))
        local proto_id = buf(0,1):uint()
        local dissector = protos[proto_id]

        if dissector ~= nil then
                -- Dissector was found, invoke subdissector with a new Tvb,
                -- created from the current buffer (skipping first two bytes).
                dissector:call(buf(2):tvb(), pkt, tree)
        elseif proto_id < 2 then
                subtree:add(f_text, buf(2))
                -- pkt.cols.info:set(buf(2, buf:len() - 3):string())
        else
                -- fallback dissector that just shows the raw data.
                data_dis:call(buf(2):tvb(), pkt, tree)
        end

end

```
