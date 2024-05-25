#ifndef HC_SR04_H
#define HC_SR04_H

/**
 * 注意事项: 探测并返回, 整个过程不会用到中断, 请勿开启所使用的定时器中断
 * @return: 返回超声波传感器测量计算后的距离, 单位: mm (毫米)
*/
unsigned int HC_SR04_Millimeter(void);

#endif // HC_SR04_H