
/*
    This file is part of the c51_lib, see <https:// github.com/supine0703/c51_lib>.

    Copyright (C) <2024>  <李宗霖>  <email: supine0703@outlook.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https:// www.gnu.org/licenses/>.
*/

#include "__config__.h"

#include "sd.h"
#include "spi.h"

#ifndef UNUSED_SD

/* ========================================================================== */

u8 SD_Type = 0; // SD卡的类型

// 等待卡准备好
// 返回值: 0, 准备好了; 其他, 错误代码
u8 SD_WaitReady(void)
{
    u32 t = 0;
    do
    {
        if (SPI_SendByte(0xff) == 0xff)
            return 0; // OK
        t++;
    } while (t < 0xffffff); // 等待
    return 1;
}

// 取消选择, 释放SPI总线
void SD_DisSelect(void)
{
    SD_CS = 1;
    SPI_SendByte(0xff); // 提供额外的8个时钟
}

// 选择sd卡,并且等待卡准备OK
// 返回值:0,成功;1,失败;
u8 SD_Select(void)
{
    SD_CS = 0;
    if (SD_WaitReady() == 0)
        return 0; // 等待成功
    SD_DisSelect();
    return 1; // 等待失败
}

// 向SD卡发送一个命令
// 输入: u8  cmd   命令
//      u32 arg   命令参数
//      u8  crc   crc校验值
// 返回值:SD卡返回的响应
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;
    u8 Retry = 0;

    SD_DisSelect(); // 取消上次片选
    if (SD_Select())
        return 0xff; // 片选失效

    // 发送
    SD_CS = 1; // 片选拉高
    SPI_SendByte(0xff);
    SPI_SendByte(0xff);
    SPI_SendByte(0xff);
    SD_CS = 0;

    SPI_SendByte(cmd | 0x40); // 分别写入命令    最高2位固定为1
    SPI_SendByte(arg >> 24);  // 命令参数 2-5字节  4个字节 32位
    SPI_SendByte(arg >> 16);
    SPI_SendByte(arg >> 8);
    SPI_SendByte(arg);
    SPI_SendByte(crc);
    // 停止传数据命令
    if (cmd == CMD12)
        SPI_SendByte(0xff); // Skip a stuff byte when stop reading
                        // 等待响应，或超时退出
    Retry = 0x1f;       // 循环32次
    do
    {
        r1 = SPI_SendByte(0xff);
    } while ((r1 & 0x80) && Retry--);
    // 返回状态值
    return r1;
}

// 初始化SD卡
u8 SD_Init(void)
{
    u8 r1;     // 存放SD卡的返回值
    u16 retry; // 用来进行超时计数
    u8 buf[4];
    u8 i;

    SPI_Speed(3); // 设置到低速模式

    // 发送最少74个脉冲 等待SD卡内部供电电压上升时间 进入SPI模式
    for (i = 10; i; --i)
        SPI_SendByte(0xff);

    retry = 20;
    do
    {
        r1 = SD_SendCmd(CMD0, 0, 0x95); // 进入IDLE状态 即复位sd卡 空闲状态
    } while ((r1 != 0x01) && retry--);

    SD_Type = 0; // 默认无卡

    if (r1 == 0x01) // 获取版本信息
    {
        if (SD_SendCmd(CMD8, 0x1aa, 0x87) == 1) // SD V2.0 发送接口状态命令
        { // 如果返回值为1 则是 SDV2.0版本
            for (i = 0; i < 4; i++)
                buf[i] = SPI_SendByte(0xff); // Get trailing return value of R7 resp
                                         // 提取返回值R7数据
            if (buf[2] == 0x01 && buf[3] == 0xaa) // 卡是否支持2.7~3.6V
            {
                retry = 0xfffe;
                do
                {
                    SD_SendCmd(CMD55, 0, 0x01);               // 发送CMD55
                    r1 = SD_SendCmd(CMD41, 0x40000000, 0x01); // 发送CMD41
                } while (r1 && retry--);
                // 鉴别SD2.0卡版本开始
                if (retry && SD_SendCmd(CMD58, 0, 0x01) == 0)
                {
                    for (i = 0; i < 4; i++)
                        buf[i] = SPI_SendByte(0xff); // 得到OCR值
                    if (buf[0] & 0x40)
                        SD_Type = SD_TYPE_V2HC; // 检查CCS
                    else
                        SD_Type = SD_TYPE_V2;
                }
                else
                    SD_Type = SD_TYPE_ERR; // 错误的卡
            }
        }
        else // SD V1.x/ MMC    V3
        {
            // 暂不做处理
        }
    }

    SD_DisSelect(); // 取消片选
    SPI_Speed(0);   // 高速
    if (SD_Type)
        return 0; // 如果没有采集到SD卡版本 则跳出函数
    else if (r1)
        return r1;
    return 0xaa; // 其他错误
}

// 在指定扇区读SD卡一扇区数据 即512字节
// buf: 数据缓存区
// sector: 扇区
// 返回值: 0, ok; 其他, 失败.
u8 SD_read_sector(u32 sector, u8* buf)
{
    u16 i;
    u8 r1;
    SD_DisSelect(); // 取消上次片选
    if (SD_Select())
        return 0xff; // 片选失效

    if (SD_Type != SD_TYPE_V2HC)
        sector <<= 9; // 转换为字节地址

    r1 = SD_SendCmd(CMD17, sector, 0x01); // 读命令
    if (r1 == 0)                          // 指令发送成功
    {
        while (SPI_SendByte(0xff) != 0xfe)
            ;
        for (i = 0; i < 512; i++)
        {
            // 连续读出一扇区数据
            buf[i] = SPI_SendByte(0xff);
        }
        // 下面是2个伪CRC（dummy CRC）
        SPI_SendByte(0xff);
        SPI_SendByte(0xff);
    }

    SD_DisSelect(); // 取消片选
    return r1;      //
}

#if 0
// 写SD卡写入512字节数据
// buf:数据缓存区
// sector:起始扇区
// 返回值:0,ok;其他,失败.
u8 SD_WriteDisk(u8*buf,u32 sector)
{
    u8 r1;
    u16 t;
   sector *= 512;// 转换为字节地址

    r1=SD_SendCmd(CMD24,sector,0x01);// 读命令
    if(r1==0)// 指令发送成功
    {

    if(SD_WaitReady()){SD_DisSelect();return 1;}// 等待准备失效
     SPI_SendByte(0xfe); // 发开启符

     for(t=0;t<512;t++)SPI_SendByte(buf[t]);// 写字节
     SPI_SendByte(0xff);    // 忽略crc
     SPI_SendByte(0xff);
     t=SPI_SendByte(0xff);  // 接收响应
     if((t&0x1f)!=0x05)r1=2;// 响应错误

    }


    SD_DisSelect();// 取消片选
    return r1;
}
#endif

/*************************************************************************************
在这个区域内的函数 主要是用来读取SD卡容量的
如果后期不用可以直接去掉
*************************************************************************************/

// 等待SD卡回应
// Response:要得到的回应值
// 返回值:0,成功得到了该回应值
//    其他,得到回应值失败
u8 SD_GetResponse(u8 Response)
{
    u16 Count = 0xfff; // 等待次数
    while ((SPI_SendByte(0xff) != Response) && Count)
        Count--; // 等待得到准确的回应
    if (Count == 0)
        return MSD_RESPONSE_FAILURE; // 得到回应失败
    else
        return MSD_RESPONSE_NO_ERROR; // 正确回应
}

// 从SD卡中读回指定长度的数据，放置在给定位置
// 输入: u8 *data(存放读回数据的内存>len)
//      u16 len(数据长度）
//      u8 release(传输完成后是否释放总线CS置高 0：不释放 1：释放）
// 返回值:0：NO_ERR
//       other：错误信息
u8 SD_ReceiveData(u8* dat, u16 len, u8 release)
{
    // 启动一次传输
    SD_CS = 0;
    if (SD_GetResponse(0xfe)) // 等待SD卡发回数据起始令牌0xfe
    {
        SD_CS = 1;
        return 1;
    }
    while (len--) // 开始接收数据
    {
        *dat = SPI_SendByte(0xff);
        dat++;
    }
    // 下面是2个伪CRC（dummy CRC）
    SPI_SendByte(0xff);
    SPI_SendByte(0xff);
    if (release == RELEASE) // 按需释放总线，将CS置高
    {
        SD_CS = 1; // 传输结束
        SPI_SendByte(0xff);
    }
    return 0;
}

// 获取SD卡的CSD信息，包括容量和速度信息
// 输入:u8 *cid_data(存放CID的内存，至少16Byte）
// 返回值:0：NO_ERR
//       1：TIME_OUT
//       other：错误信息
u8 SD_GetCSD(u8* csd_data)
{
    u8 r1;
    r1 = SD_SendCmd(CMD9, 0, 0xff); // 发CMD9命令，读CSD
    if (r1)
        return r1; // 没返回正确应答，则退出，报错
    SD_ReceiveData(csd_data, 16, RELEASE); // 接收16个字节的数据
    return 0;
}

// 获取SD卡的容量（字节）
// 返回值:0： 取容量出错
//       其他:SD卡的容量(字节)
u32 SD_GetCapacity(void) // 修改为 获得扇区数！！
{
    u8 csd[16];
    u32 Capacity;
    u8 r1;
    u16 i;
    u16 temp;
    // 取CSD信息，如果期间出错，返回0
    if (SD_GetCSD(csd) != 0)
        return 0;
    // 如果为SDHC卡，按照下面方式计算
    if ((csd[0] & 0xc0) == 0x40) // V2.00的卡
    {
        Capacity = ((u32)csd[8]) << 8;
        Capacity += (u32)csd[9] + 1;
        Capacity = (Capacity) * 1024; // 得到扇区数
                                      // Capacity*=512;// 得到字节数
    }
    else // V1.XX的卡
    {
        i = csd[6] & 0x03;
        i <<= 8;
        i += csd[7];
        i <<= 2;
        i += ((csd[8] & 0xc0) >> 6);
        // C_SIZE_MULT
        r1 = csd[9] & 0x03;
        r1 <<= 1;
        r1 += ((csd[10] & 0x80) >> 7);
        r1 += 2; // BLOCKNR
        temp = 1;
        while (r1)
        {
            temp *= 2;
            r1--;
        }
        Capacity = ((u32)(i + 1)) * ((u32)temp);
        // READ_BL_LEN
        i = csd[5] & 0x0f;
        // BLOCK_LEN
        temp = 1;
        while (i)
        {
            temp *= 2;
            i--;
        }
        // The final result
        Capacity *= (u32)temp;          // 字节为单位
        Capacity = (u32)Capacity / 512; // 改为扇区数
    }

    return (u32)Capacity;
}

/*************************************************************************************
**************************************************************************************
*************************************************************************************/

#if 0
// 在指定扇区写SD卡一扇区数据 即512字节
// buf:数据缓存区
// sector:起始扇区
// cnt:扇区数
// 返回值:0,ok;其他,失败.
u8 SD_write_sector(u32 sector,u8*buf)
{
    u8 r1;
    u16 t;
    u32 m=0;
    if(SD_Type!=SD_TYPE_V2HC)sector *= 512;// 转换为字节地址

     r1=SD_SendCmd(CMD24,sector,0x01);// 读命令
        if(r1==0)// 指令发送成功
        {
            // r1=SD_SendBlock(buf,0xfe);// 写512个字节
             // 等待卡准备就绪
              do
              {
                if(SPI_SendByte(0xff)==0xff)break;// OK
                  m++;
               }while(m<0xffffff);// 等待

              SPI_SendByte(0xfe);

                for(t=0;t<512;t++)SPI_SendByte(buf[t]);// 提高速度,减少函数传参时间
                SPI_SendByte(0xff);// 忽略crc
                SPI_SendByte(0xff);
                t=SPI_SendByte(0xff);// 接收响应
                if((t&0x1f)!=0x05)return 2;// 响应错误
        }
    SD_DisSelect();// 取消片选
    return 0;  // 写入成功


}

// 在指定地址 读取 指定字节数
// addr:地址
// buf:数据缓存区
// Bytes：要读取的字节数
// 返回值:0,ok;其他,失败.
u8 SD_read_Byte(u32 addr,u8 *buf,u32 Bytes)
{
   u16 i;
   u8 temp=0,retry;

   // 命令16设置块长度
   SD_SendCmd(16,Bytes,0xff);

   retry = 0;

   do
   {
   // 写入CMD17     读取命令
   temp=SD_SendCmd(17,addr,0xff);
   retry++;
   }

    while((temp!=0x00) && (retry < TRY_TIME));


   if (retry >= TRY_TIME)
   {
   return READ_BLOCK_ERROR;
   }

   // 读到0xfe报头
   while(SPI_SendByte(0xff)!=0xfe);

    for(i=0;i<Bytes;i++)
   {

       // 连续读出指定 长度字节数据到buf
       buf[i] = SPI_SendByte(0xff);
   }

  // 下面是2个伪CRC（dummy CRC）
   SPI_SendByte(0xff);
   SPI_SendByte(0xff);


   // * 命令16恢复块长度 为512个字节
   SD_SendCmd(16,512,0xff);
    SD_DisSelect();// 取消片选

   return 0;

}
#endif

/* ========================================================================== */

#endif // UNUSED_SD
