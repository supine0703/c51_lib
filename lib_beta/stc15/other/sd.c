#include "sd.h"
#include "spi.h"
#include "stc15f2k60s2.h"

u8 SD_Type = 0; // SD卡的类型

///////////////////////////////////////////////////////////////////////////////////

// 等待卡准备好
// 返回值:0,准备好了;其他,错误代码
u8 SD_WaitReady(void)
{
    u32 t = 0;
    do
    {
        if (SPI_SendByte(0XFF) == 0XFF)
            return 0; // OK
        t++;
    } while (t < 0XFFFFFF); // 等待
    return 1;
}

// 取消选择,释放SPI总线
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
// 输入: u8 cmd   命令
//      u32 arg  命令参数
//      u8 crc   crc校验值
// 返回值:SD卡返回的响应
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;
    u8 Retry = 0;

    SD_DisSelect(); // 取消上次片选
    if (SD_Select())
        return 0XFF; // 片选失效

    // 发送
    SD_CS = 1; // 片选拉高
    SPI_SendByte(0xff);
    SPI_SendByte(0xff);
    SPI_SendByte(0xff);
    SD_CS = 0;

    SPI_SendByte(cmd | 0x40); // 分别写入命令	最高2位固定为1
    SPI_SendByte(arg >> 24);  // 命令参数 2-5字节  4个字节 32位
    SPI_SendByte(arg >> 16);
    SPI_SendByte(arg >> 8);
    SPI_SendByte(arg);
    SPI_SendByte(crc);
    // 停止传数据命令
    if (cmd == CMD12)
        SPI_SendByte(0xff); // Skip a stuff byte when stop reading
                            // 等待响应, 或超时退出
    Retry = 0X1F;           // 循环32次
    do
    {
        r1 = SPI_SendByte(0xFF);
    } while ((r1 & 0X80) && Retry--);
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

    // 发送最少74个脉冲 等待SD卡内部供电电压上升时间	  进入SPI模式
    for (i = 0; i < 10; i++)
        SPI_SendByte(0XFF);

    retry = 20;
    do
    {
        r1 = SD_SendCmd(CMD0, 0, 0x95); // 进入IDLE状态 	 即复位sd卡 空闲状态
    } while ((r1 != 0X01) && retry--);

    SD_Type = 0; // 默认无卡

    if (r1 == 0X01) // 获取版本信息
    {
        if (SD_SendCmd(CMD8, 0x1AA, 0x87) == 1) // SD V2.0	  发送接口状态命令
        { // 如果返回值为1 则是 SDV2.0版本
            for (i = 0; i < 4; i++)
                buf[i] =
                    SPI_SendByte(0XFF); // Get trailing return value of R7 resp
                                        // 提取返回值R7数据
            if (buf[2] == 0X01 && buf[3] == 0XAA) // 卡是否支持2.7~3.6V
            {
                retry = 0XFFFE;
                do
                {
                    SD_SendCmd(CMD55, 0, 0X01);               // 发送CMD55
                    r1 = SD_SendCmd(CMD41, 0x40000000, 0X01); // 发送CMD41
                } while (r1 && retry--);
                if (retry &&
                    SD_SendCmd(CMD58, 0, 0X01) == 0) // 鉴别SD2.0卡版本开始
                {
                    for (i = 0; i < 4; i++)
                        buf[i] = SPI_SendByte(0XFF); // 得到OCR值
                    if (buf[0] & 0x40)
                        SD_Type = SD_TYPE_V2HC; // 检查CCS
                    else
                        SD_Type = SD_TYPE_V2;
                }
                else
                    SD_Type = SD_TYPE_ERR; // 错误的卡
            }
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
