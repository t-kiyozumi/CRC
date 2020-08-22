#include <stdio.h>
#include <stdint.h>
#define MESSAGE 0b00000000000000000011010011101100 //送りたい情報のマクロ
#define GP 0b00000000000000000000000000001011      //生成多項式のマクロ

int main()
{
    uint32_t Ms;  //送りたい情報
    uint32_t csc; //パリティ
    uint32_t g;   //生成多項式
    int Ng;       //生成多項式の最高次数

    uint32_t CDmask; //繰り下げを行うためのマスク
    CDmask = 0b10000000000000000000000000000000;

    //送りたい情報を決定
    Ms = MESSAGE;
    //生成多項式を決定
    g = GP;
    Ng = 4;

    Ms = Ms << (Ng - 1); //余りを後からくっつける分だけ下位ビットを開ける
    g = g << (32 - Ng);

    csc = Ms;
    while (1)
    {
        if (csc & CDmask) //先頭のビットが1であるならばxorの処理を行う
        {
            csc = csc ^ g;
        }
        else //先頭のビットが0ならば，gを右側にビットシフトする
        {
            g = g >> 1;
            CDmask = CDmask >> 1;
        }

        if (csc < GP) //余りが生成多項式より小さくなったらプログラムを終了
        {
            break;
        }
    }
    printf("%d", csc);
    return 0;
}