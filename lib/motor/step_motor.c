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

#include "__config__.h"

#ifndef UNUSED_STEP_MOTOR

/* ========================================================================== */

unsigned char StepMotor_Index = 0;
unsigned char code StepMotor_Turns[] = {
    0x02, /* 0010 */
    0x06, /* 0110 */
    0x04, /* 0100 */
    0x0c, /* 1100 */
    0x08, /* 1000 */
    0x09, /* 1001 */
    0x01, /* 0001 */
    0x03, /* 0011 */
};

/* ========================================================================== */

#endif // UNUSED_STEP_MOTOR
