#include <stdio.h>

typedef struct _BitmapInfoHeader
{
    int header = sizeof(_BitmapInfoHeader); // この構造体のサイズ
    int width;                              // Bitmapのサイズ・横幅
    int height;                             // Bitmapのサイズ・縦幅
    short plane = 1;                        // plane数・常に１
    short bits;                             // Bitmapの色数(bit単位) 単位[bit]でRGBカラーなら24, グレースケール（単色）なら8
    int compression;                        // 圧縮されているか？
    int comp_image_size;                    // 画像全体のサイズ・使わない
    int x_resolution;                       // Bitmapの解像度・使わない
    int y_resolution;                       // 72と設定しておいてもかまわない
    int pallet_num;                         // パレット数
    // 24bitカラー等の場合使わない
    int important_pallet_num;
    // 重要なパレット数・使わない
    // 24bitカラー等の場合使わない
} BitmapInfoHeader;

typedef struct _BitmapHeader
{
    short distinct = 0x4d42; // 'BM'0x424d
    int32_t filesize;        // 総ファイルサイズ
    short reserve1 = 0;      // 予約領域・常に０
    short reserve2 = 0;      // 予約領域・常に０
    int32_t offset =
        sizeof(_BitmapHeader) +
        sizeof(_BitmapInfoHeader); // ファイルの先頭からデータまでのオフセット
} BitmapHeader;

// ここでデータをビットマップファイルへ書き込む
int WriteBitmap(const char *filename, char *data, int width, int height)
{
    FILE *fp;
    BitmapHeader header;
    BitmapInfoHeader info;
    header.distinct = 0x4d42;
    header.reserve1 = 0;
    header.reserve2 = 0;
    header.offset = sizeof(header) + sizeof(info);

    // Bitmapサイズ
    info.header = sizeof(info);
    info.width = width;
    info.height = height;
    info.plane = 1;
    info.bits = 24;
    int writeWidth;
    // データの幅のバイト数が4の倍数であるかをチェック
    writeWidth = width * 3;

    int i = 0;
    int j = 0;
    int x;
    int y;
    // ファイルオープン
    if ((fp = fopen(filename, "wb")) == NULL)
    {
        return -1;
    }
    int stride = (width * 3 + 3) / 4 * 4;
    info.comp_image_size = (width * height * (info.bits / 8));

    //ファイル容量
    header.filesize =
        info.comp_image_size //ビット情報量
        + sizeof(header)     // BitmapHeader構造体サイズ
        + sizeof(info);      // BitmapInfoHeader構造体サイズ

    //ヘッダ書き込み
    fwrite(&header, sizeof(header), 1, fp);
    fwrite(&info, sizeof(info), 1, fp);
    // イメージデータ書き込み
    for (y = 0; y < height * 4; y++)
    {
        // データをBGRの順で書き込み
        for (x = 0; x < width * 4; x++)
        {
            j = fwrite((data + x * 3 + 3 * y * width + 2), sizeof(char), 1, fp);
            j = fwrite((data + x * 3 + 3 * y * width + 1), sizeof(char), 1, fp);
            j = fwrite((data + x * 3 + 3 * y * width), sizeof(char), 1, fp);
            // j = fwrite((data + x * 3 + 3 * y * width + 3), sizeof(GLubyte), 1, fp);
        }
    }
    // ファイルクローズ
    fclose(fp);
    return 0;
}
