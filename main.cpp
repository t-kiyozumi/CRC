#include <stdio.h>
#include <stdint.h>
#define MESSAGE 0b00000000000000000011010011101100 //送りたい情報のマクロ
#define GP 0b00000000000000000000000000001011      //生成多項式のマクロ

//devided(情報)をdivisor(生成多項式)で割ってあまりのremを返す関数
//Ngは生成多項式の最高次数
uint32_t calc_rem(uint32_t divided, uint32_t divisor, uint32_t Ng)
{

    int rem;
    rem = divided;
    int CDmask = 0b10000000000000000000000000000000;

    divisor = divisor << (32 - Ng);
    while (1)
    {
        if (rem & CDmask) //先頭のビットが1であるならばxorの処理を行う
        {
            rem = rem ^ divisor;
        }
        else //先頭のビットが0ならば，gを右側にビットシフトする
        {
            divisor = divisor >> 1;
            CDmask = CDmask >> 1;
        }

        if (rem < GP) //余りが生成多項式より小さくなったらプログラムを終了
        {
            return rem;
        }
    }
}

int main()
{
    uint32_t Ms;     //送りたい情報
    uint32_t crc;    //パリティ，crc符号
    uint32_t g;      //生成多項式
    int Ng;          //生成多項式の最高次数
    uint32_t frame;  //実際に送信するパリティ付の情報，フレーム
    uint32_t CDmask; //繰り下げを行うためのマスク
    CDmask = 0b10000000000000000000000000000000;
    uint32_t checkP;

    //--------------------------------以下送信部----------------------

    //送りたい情報を決定
    Ms = MESSAGE;
    //生成多項式を決定
    g = GP;
    Ng = 4;
    //割り算のために調節する
    Ms = Ms << (Ng - 1); //余りを後からくっつける分だけ下位ビットを開ける
    //割り算の余りをcrcとする。

    crc = calc_rem(Ms, g, Ng); //Msをgで割った余りを計算
    printf("%u\n", crc);
    frame = crc | Ms; //送信用フレーム
    //----------------------------------以下伝送路--------------------

    //----------------------------------以下受信部--------------------
    //受け取ったフレームが破損していないかしらべる
    checkP = calc_rem(frame, g, Ng);
    printf("%u\n", checkP);

    return 0;
}