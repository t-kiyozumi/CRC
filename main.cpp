#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#define MESSAGE 0b1111111111100000000 //送りたい情報のマクロ
#define GP      0b10000100000010011        //生成多項式のマクロ
#define GPD 17                          //生成多項式の最上位bit数

// #define GPD 4 //生成多項式の最上位bit数

//devided(情報)をdivisor(生成多項式)で割ってあまりのremを返す関数
//Ngは生成多項式の最高次数
uint64_t calc_rem(uint64_t divided, uint64_t divisor, uint64_t Ng)
{

    uint64_t rem;
    rem = divided;
    uint64_t CDmask = 0b1000000000000000000000000000000000000000000000000000000000000000;

    divisor = divisor << (64 - Ng);
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

//数値を二進数で表示するためのプログラム
std::string to_binString(uint64_t val)
{
    if (!val)
        return std::string("0");
    std::string str;
    while (val != 0)
    {
        if ((val & 1) == 0)               // val は偶数か？
            str.insert(str.begin(), '0'); //  偶数の場合
        else
            str.insert(str.begin(), '1'); //  奇数の場合
        val >>= 1;
    }
    return str;
}

int main()
{
    srand((unsigned int)time(NULL));
    uint64_t Ms;     //送りたい情報
    uint64_t crc;    //パリティ，crc符号
    uint64_t g;      //生成多項式
    int Ng;          //生成多項式の最高次数
    uint64_t frame;  //実際に送信するパリティ付の情報，フレーム
    uint64_t CDmask; //繰り下げを行うためのマスク
    CDmask = 0b1000000000000000000000000000000000000000000000000000000000000000;
    uint64_t checkP;

    uint64_t noise = 0b0; //情報を誤らせるための雑音

    //--------------------------------以下送信部----------------------
    //送りたい情報を決定
    Ms = MESSAGE;
    //生成多項式を決定
    g = GP;
    Ng = GPD;
    //割り算のために調節する
    Ms = Ms << (Ng - 1); //余りを後からくっつける分だけ下位ビットを開ける
    //割り算の余りをcrcとする。
    crc = calc_rem(Ms, g, Ng); //Msをgで割った余りを計算
    frame = crc | Ms;          //送信用フレームの作成

    //送信する情報に関することを表示
    printf("\n----------infromation of send--------------------\n");
    printf("sendMesage:%s\n", to_binString(MESSAGE).c_str());
    printf("GP:%s\n", to_binString(GP).c_str());
    printf("CRC:%s\n", to_binString(crc).c_str());
    printf("sendFrame:%s\n", to_binString(frame).c_str());

    //----------------------------------以下伝送路--------------------
    printf("\n----------infromation of channel-----------------\n");
    //50％の確率でノイズを混ぜる
    noise = rand() % 2;
    if (noise != 0b0)
    {
        //ノイズを受信したので信号を破損させる
        frame = frame>>5;
        printf("Noise has occurred\n");
    }
    else
    {
        printf("NO noise\n");
    }

    //----------------------------------以下受信部--------------------
    //受け取ったフレームが破損していないかしらべる
    printf("\n----------infromation of receive-----------------\n");
    checkP = calc_rem(frame, g, Ng);
    if (checkP > 0)
    {
        printf("The comm has failed\n");
        printf("receiveFrame:%s\n", to_binString(frame).c_str());
    }
    else
    {
        printf("The comm was a successe!!\n");
        printf("receiveFrame:%s\n", to_binString(frame).c_str());
    }
    return 0;
}