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

```lua
-- 1.プロトコルを定義する
myproto = Proto("MyProto", "My Protocol")
  
-- 2.プロトコルフィールドを定義する
--- 数値(base.DEC, base.HEX or base.OCT, base.DEC_HEX, base.HEX_DEC, base.UNIT_STRING or base.RANGE_STRING)
local pf_command_dec = ProtoField.uint16("myproto.command_dec", "command", base.DEC)
local pf_command_hex = ProtoField.uint16("myproto.command_hex", "command", base.HEX)
local pf_command_oct = ProtoField.uint16("myproto.command_oct", "command", base.OCT)
local pf_command_dechex = ProtoField.uint16("myproto.command_dechex", "command", base.DEC_HEX)
local pf_command_hexdec = ProtoField.uint16("myproto.command_hexdec", "command", base.HEX_DEC)

-- 単位表示
--- [valuestring] = unit_table = {"単数単位", "複数単位"}
local unit_table = {"esc", "secs"}
local pf_command_unitstring = ProtoField.uint16("myproto.command_unitstring", "command", base.UNIT_STRING, unit_table)

-- 範囲表示
--- [valuestring] = range_table = {{min, max, "string"}, ...}
local range_table = {{0, 100, "1 handred"}}
local pf_command_rangestring = ProtoField.uint16("myproto.command_rangestring", "command", base.RANGE_STRING, range_table)

-- ビット表示
--- ProtoField.new(name, abbr, type, [valuestring], [base], [mask], [descr])
local pf_flags = ProtoField.new ("Flags", "myproto.flags", ftypes.UINT8, nil, base.HEX)

local pf_flag_response = ProtoField.new ("Response", "myproto.flags.response", ftypes.BOOLEAN, {"this is a response","this is a query"}, 8, 0x80, "is the message a response?")

local pf_flag_opcode = ProtoField.new ("Opcode", "myproto.flags.opcode", ftypes.UINT8, nil, base.DEC, 0x0F, "operation code")
  
-- 文字列表示
--- ProtoField.string(abbr, [name], [display], [desc])
local pf_locale = ProtoField.string("locale", "locale", base.ANSI)
  
myproto.fields = {pf_command_dec,pf_command_hex,pf_command_oct,pf_command_dechex,pf_command_hexdec,pf_command_unitstring,pf_command_rangestring,pf_locale,pf_flags,pf_flag_response, pf_flag_opcode}

-- 3.プロトコル解析用の関数を定義する
function  myproto.dissector(buffer,pinfo,root)
  pinfo.cols.protocol = "MyProto"
  
  local subtree = root:add(dns, buffer(), "My Protocol Data")
  
  -- 数値
  subtree:add(pf_command_dec, buffer(0,2), buffer:range(0,2):uint())
  subtree:add(pf_command_hex, buffer(0,2), buffer:range(0,2):uint())
  subtree:add(pf_command_oct, buffer(0,2), buffer:range(0,2):uint())
  subtree:add(pf_command_dechex, buffer(0,2), buffer:range(0,2):uint())
  subtree:add(pf_command_hexdec, buffer(0,2), buffer:range(0,2):uint())
  
  -- 単位表示
  subtree:add(pf_command_unitstring, buffer(0,2), buffer:range(0,2):uint())
  
  -- 範囲表示
  subtree:add(pf_command_rangestring, buffer(0,2), buffer:range(0,2):uint())
  
  -- ビット表示
  local flagrange = buffer:range(2,1)
  local flag_tree = subtree:add(pf_flags, flagrange)
  local query_flag_tree = flag_tree:add(pf_flag_response, flagrange)
  flag_tree:add(pf_flag_opcode, flagrange)
  
  -- 文字列表示
  subtree:add(pf_locale, buffer(13, 2), buffer(13, 2):string())
end

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
