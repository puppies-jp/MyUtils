# (1)コンパイラ
CC  = g++
# (2)コンパイルオプション
CFLAGS    =
# (3)実行ファイル名
TARGET  = Sample
# (4)コンパイル対象のソースコード
# 複数ある場合はこうやって作成する
SRCS    = Sample.cpp
SRCS    += hoge1.cpp
SRCS    += hoge2.cpp
SRCS    += hoge3.cpp

# (5)オブジェクトファイル名
OBJS    = $(SRCS:.cpp=.o)
 
# (6)インクルードファイルのあるディレクトリパス
INCDIR  = -I../inc
 
# (7)ライブラリファイルのあるディレクトリパス
LIBDIR  = 
 
# (8)追加するライブラリファイル
LIBS    = 

# (9)ターゲットファイル生成
# 1行目は、$(TARGET): $(OBJS)と記載して、「TARGET」が「OBJS」に依存することを示している
# 依存ファイルが指定してあると、 "ターゲットのファイルがない"もしくは
# "ターゲットの更新日時が依存ファイルの更新日時より古かった場合"に
# コマンドを実行する。
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS)
	
# (10)オブジェクトファイル生成
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(SRCS)

# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(OBJS) $(TARGET)
# (12).oファイル、実行ファイル、.dファイルを削除
clean:
	-rm -f $(OBJS) $(TARGET) *.d