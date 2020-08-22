#include <stdio.h>
#include <stdint.h>
class message
{
public:
};


int main()
{
    uint32_t Ms; //送りたい情報
    uint32_t c;  //パリティ
    uint32_t g;  //生成多項式
    uint32_t mask;

    Ms = 0b00000000000000000011010011101100;
    g = 0b00000000000000000000000000001011;
    mask = 0b10000000000000000000000000000000;

    Ms = Ms << 3;
    g = g << (32 - 4);

    c = Ms;
    while (1)
    {
        if (c & mask) //先頭のビットが1であるならばxorの処理を行う
        {
            c = c ^ g;
        }
        else //先頭のビットが0ならば，gを右側にビットシフトする
        {
            g = g >> 1;
            mask = mask >> 1;
        }

        if (c < 0b00000000000000000000000000001011)
        {
            break;
        }
    }
    printf("%d", c);
    return 0;
}