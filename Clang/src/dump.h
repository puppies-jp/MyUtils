#include <stdio.h>

// 読み込み用定義
void dump(const unsigned char *data_buffer, const unsigned int length);

// packetのダンプ
void caught_packet_dump(const unsigned char *packet,
                        const unsigned int length);