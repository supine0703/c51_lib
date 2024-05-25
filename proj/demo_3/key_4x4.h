#ifndef KEY_4X4_H
#define KEY_4X4_H

#define KEY_NULL 0xff

/**
 * @return: KEY_NULL(0xff) 即 没有被按下, 被按下则返回 0-15
*/
unsigned char KEY_4X4_Value(void);

#endif // KEY_4X4_H