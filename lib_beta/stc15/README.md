/*
    This file is part of the c51_lib, see <https://github.com/supine0703/c51_lib>.
    
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
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

***beta版本, 是为竞赛准备的版本, 相较于 'lib' 可能会有缺省或精简的部分, 模块封装耦合度更高***

# 策略的改变
经过一轮使用 stc89 系列进行的比赛, 发现有以下问题:
- 比赛中创建 '.h', 写文件头 其实有些浪费时间
- 比赛中 'unsigned char' 和 'unsigned int' 的写法 过于拖沓

因此, stc15 的 beta 版本封装较 stc89, 将会有以下不同:
- 不定义模块的头文件, 在 '\_\_function\_\_.h' 中申明
- 不使用 char 和 int 类型 (注意循环忌用'>=0')
- 包含 '\_\_type\_\_.h', 即可使用 u8, u16

文件将分为三个模块
- main: 包括: 
  - STARTUP.A51
  - main.c
- header: 包括: 
  - 'u_type.h': 定义 u8, u16 (之所以单独一个头文件, 因为我主张使用'typedef', 而不是'#define')
  - \_\_config\_\_.h: 包含所有的引脚的**宏**配置, 注意是**宏**
  - function.h: 通过 'extern' 关键字, 包含所有需要使用函数申明
- source: 包含所有模块的 '.c' 文件
