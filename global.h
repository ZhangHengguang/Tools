#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>

enum CalcType {
    CUSTOM = 0,
    SUM,
    XOR,
    HEX_TO_DEC,
    HEX_TO_FLOAT,
    HEX_TO_DOUBLE,
    DEC_TO_HEX,
    FLOAT_TO_HEX,
    DOUBLE_TO_HEX,
    CRC8_AUTOSAR,// 汽车电子系统
    CRC8_BLUETOOTH,
    CRC8_CDMA2000,
    CRC8_DARC,
    CRC8_DVB_S2,
    CRC8_GSM_A,
    CRC8_GSM_B,
    CRC8_HITAG,
    CRC8_I432_1,
    CRC8_I_CODE,
    CRC8_LTE,
    CRC8_MAXIM_DOW,
    CRC8_MIFARE_MAD,
    CRC8_NRSC_5,
    CRC8_OPENSAFETY,
    CRC8_ROHC,
    CRC8_SAE_J1850,
    CRC8_SMBUS,
    CRC8_TECH3250,
    CRC8_WCDMA,
    CRC16_ARC,
    CRC16_CDMA2000,
    CRC16_CMS,
    CRC16_DDS110,
    CRC16_DECT_R,
    CRC16_DECT_X,
    CRC16_DNP,
    CRC16_EN13757,
    CRC16_GENIBUS,
    CRC16_GSM,
    CRC16_IBM_3740,
    CRC16_IBM_SDLC,
    CRC16_ISO_IEC_14443_3_A,
    CRC16_KERMIT,
    CRC16_LJ1200,
    CRC16_M17,
    CRC16_MAXIM_DOW,
    CRC16_MCRF4XX,
    CRC16_MODBUS,
    CRC16_NRSC_5,
    CRC16_OPENSAFETY_A,
    CRC16_OPENSAFETY_B,
    CRC16_PROFIBUS,
    CRC16_RIELLO,
    CRC16_SPI_FUJITSU,
    CRC16_T10_DIF,
    CRC16_TELEDISK,
    CRC16_TMS37157,
    CRC16_UMTS,
    CRC16_USB,
    CRC16_XMODEM,
    CRC32_AIXM,
    CRC32_AUTOSAR,
    CRC32_BASE91_D,
    CRC32_BZIP2,
    CRC32_CD_ROM_EDC,
    CRC32_CKSUM,
    CRC32_ISCSI,
    CRC32_ISO_HDLC,
    CRC32_JAMCRC,
    CRC32_MEF,
    CRC32_MPEG_2,
    CRC32_XFER
};

template <class T>
struct crcType {
    std::string name;
    size_t width;
    size_t poly;
    T init;
    bool refIn;
    bool refOut;
    T xOrOut;
};

size_t outOfRange(size_t width);

template<class T>
T reverseRet(T resByte, size_t bitSize);

template<class T>
T funCrc(T crc, size_t bitSize, size_t moveBit, crcType<T> optionType, std::vector<unsigned char> arr, size_t len);

template <class T>
unsigned int crcCheck(crcType<T> optionType, std::vector<unsigned char> arr, size_t len);

#endif // GLOBAL_H
