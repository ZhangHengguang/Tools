#include "global.h"
#include <cstdint>
#include <iostream>
#include <sstream>

size_t outOfRange(size_t width)
{
    size_t ret = 0;
    for (size_t i = 0; i < width; i++)
    {
        size_t oneBit = 0x01 << (width - 1 - i);
        ret += oneBit;
    }
    return ret;
}

template<class T>
T reverseRet(T resByte, size_t bitSize)
{
    T ret = 0;
    for (size_t i = 0; i < bitSize; i++)
    {
        if (resByte & 0x01)//数据或1，得到最小位的值
        {
            T oneBit = 0x01 << (bitSize - 1 - i);//如果是1则通过移位的方式达到单个位的反转
            ret += oneBit;//把所有位反转后的值加和，得到所有位按位反转后的值
        }
        resByte >>= 1;//每取走1位就右移1位，取下一位
    }
    return ret;
}

template<class T>
T funCrc(T crc, size_t bitSize, size_t moveBit, crcType<T> optionType, std::vector<unsigned char> arr, size_t len)
{
    int cnt = 0;
    while (len--)
    {
        if (optionType.refIn)
        {
            crc ^= ((T)reverseRet(arr[cnt], 8)) << (bitSize - 8);
        }
        else
        {
            crc ^= (T)(arr[cnt]) << (bitSize - 8);
        }
        cnt++;
        for (int i = 0; i < 8; i++)
        {
            if (crc & moveBit)
            {
                crc <<= 1;
                crc ^= (optionType.poly << (bitSize - optionType.width));
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    if (optionType.refOut)
    {
        crc = reverseRet(crc, bitSize);

        if (optionType.xOrOut)
        {
            crc ^= optionType.xOrOut;
        }
        return crc;
    }
    else
    {
        if (optionType.xOrOut)
        {
            crc ^= optionType.xOrOut;
        }
        return crc >> (bitSize - optionType.width);
    }
}

template <class T>
unsigned int crcCheck(crcType<T> optionType, std::vector<unsigned char> arr, size_t len)
{
    if (optionType.init > outOfRange(optionType.width))
    {
        return -1;
    }
    if (optionType.xOrOut > outOfRange(optionType.width))
    {
        return -1;
    }
    size_t bitSize = 8;
    size_t moveBit = 0x80;
    if (optionType.width <= 8)
    {
        bitSize = 8;
        moveBit = 0x80;
    }
    else if (optionType.width > 8 && optionType.width <= 16)
    {
        bitSize = 16;
        moveBit = 0x8000;
    }
    else if (optionType.width > 16)
    {
        bitSize = 32;
        moveBit = 0x80000000;
    }
    else
    {
        return -1;
    }
    T crc = optionType.init << (bitSize - optionType.width);
    return funCrc(crc, bitSize, moveBit, optionType, arr, len);
}

// 显式实例化声明
template unsigned int crcCheck<uint8_t>(crcType<uint8_t>, std::vector<unsigned char>, size_t);
template unsigned int crcCheck<uint16_t>(crcType<uint16_t>, std::vector<unsigned char>, size_t);
template unsigned int crcCheck<uint32_t>(crcType<uint32_t>, std::vector<unsigned char>, size_t);

unsigned int calcChkSum(std::vector<unsigned char> arr)
{
    int sum = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        sum += static_cast<unsigned int>(arr[i]);
    }
    return sum & 0xFF;
}

unsigned int calcXorSum(std::vector<unsigned char> arr)
{
    int sum = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        sum ^= static_cast<unsigned int>(arr[i]);
    }
    return sum & 0xFF;
}

std::string floatToHex(float value) {
    union {
        float f;
        unsigned int i;
    } u;
    u.f = value;

    std::string hex = "";
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%08X", u.i);
    hex = buffer;
    return hex;
}

float hexToFloat(const std::string& hex) {
    if(hex.length() != 8) {
        std::cerr << "Invalid hex length" << std::endl;
        return 0.0f;
    }

    union {
        float f;
        unsigned int i;
    } u;
    u.i = std::stoul(hex, nullptr, 16);
    return u.f;
}

std::string doubleToHex(double value) {
    union {
        double d;
        unsigned long long ull;
    } u;
    u.d = value;

    std::stringstream ss;
    ss << std::hex << std::uppercase << u.ull;
    return ss.str();
}

double hexToDouble(const std::string& hex) {
    union {
        double d;
        unsigned long long ull;
    } u;
    u.ull = std::stoull(hex, nullptr, 16);
    return u.d;
}
