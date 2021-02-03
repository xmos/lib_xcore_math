// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"


/////////////////////////////////////////////
/////  Rotation Tables (for Magnitude)  /////
/////////////////////////////////////////////

const unsigned rot_table32_rows = 30;

const complex_s32_t rot_table32[30][4] = {
  { {759250125, -759250125}, {759250125, -759250125}, {759250125, -759250125}, {759250125, -759250125} },
  { {992008094, -410903207}, {992008094, -410903207}, {992008094, -410903207}, {992008094, -410903207} },
  { {1053110176, -209476638}, {1053110176, -209476638}, {1053110176, -209476638}, {1053110176, -209476638} },
  { {1068571464, -105245103}, {1068571464, -105245103}, {1068571464, -105245103}, {1068571464, -105245103} },
  { {1072448455, -52686014}, {1072448455, -52686014}, {1072448455, -52686014}, {1072448455, -52686014} },
  { {1073418433, -26350943}, {1073418433, -26350943}, {1073418433, -26350943}, {1073418433, -26350943} },
  { {1073660973, -13176464}, {1073660973, -13176464}, {1073660973, -13176464}, {1073660973, -13176464} },
  { {1073721611, -6588356}, {1073721611, -6588356}, {1073721611, -6588356}, {1073721611, -6588356} },
  { {1073736771, -3294193}, {1073736771, -3294193}, {1073736771, -3294193}, {1073736771, -3294193} },
  { {1073740561, -1647099}, {1073740561, -1647099}, {1073740561, -1647099}, {1073740561, -1647099} },
  { {1073741508, -823550}, {1073741508, -823550}, {1073741508, -823550}, {1073741508, -823550} },
  { {1073741745, -411775}, {1073741745, -411775}, {1073741745, -411775}, {1073741745, -411775} },
  { {1073741804, -205887}, {1073741804, -205887}, {1073741804, -205887}, {1073741804, -205887} },
  { {1073741819, -102944}, {1073741819, -102944}, {1073741819, -102944}, {1073741819, -102944} },
  { {1073741823, -51472}, {1073741823, -51472}, {1073741823, -51472}, {1073741823, -51472} },
  { {1073741824, -25736}, {1073741824, -25736}, {1073741824, -25736}, {1073741824, -25736} },
  { {1073741824, -12868}, {1073741824, -12868}, {1073741824, -12868}, {1073741824, -12868} },
  { {1073741824, -6434}, {1073741824, -6434}, {1073741824, -6434}, {1073741824, -6434} },
  { {1073741824, -3217}, {1073741824, -3217}, {1073741824, -3217}, {1073741824, -3217} },
  { {1073741824, -1608}, {1073741824, -1608}, {1073741824, -1608}, {1073741824, -1608} },
  { {1073741824, -804}, {1073741824, -804}, {1073741824, -804}, {1073741824, -804} },
  { {1073741824, -402}, {1073741824, -402}, {1073741824, -402}, {1073741824, -402} },
  { {1073741824, -201}, {1073741824, -201}, {1073741824, -201}, {1073741824, -201} },
  { {1073741824, -101}, {1073741824, -101}, {1073741824, -101}, {1073741824, -101} },
  { {1073741824, -50}, {1073741824, -50}, {1073741824, -50}, {1073741824, -50} },
  { {1073741824, -25}, {1073741824, -25}, {1073741824, -25}, {1073741824, -25} },
  { {1073741824, -13}, {1073741824, -13}, {1073741824, -13}, {1073741824, -13} },
  { {1073741824, -6}, {1073741824, -6}, {1073741824, -6}, {1073741824, -6} },
  { {1073741824, -3}, {1073741824, -3}, {1073741824, -3}, {1073741824, -3} },
  { {1073741824, -2}, {1073741824, -2}, {1073741824, -2}, {1073741824, -2} },
};

const unsigned rot_table16_rows = 14;
const int16_t rot_table16[14][2][16] = {
  { {  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82,  0x5A82 },
    { -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82, -0x5A82 } },
  { {  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642,  0x7642 },
    { -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC, -0x30FC } },
  { {  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A,  0x7D8A },
    { -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9, -0x18F9 } },
  { {  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62,  0x7F62 },
    { -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C, -0x0C8C } },
  { {  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9,  0x7FD9 },
    { -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648, -0x0648 } },
  { {  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6,  0x7FF6 },
    { -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324, -0x0324 } },
  { {  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE,  0x7FFE },
    { -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192, -0x0192 } },
  { {  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF },
    { -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9, -0x00C9 } },
  { {  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF },
    { -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065, -0x0065 } },
  { {  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF },
    { -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032, -0x0032 } },
  { {  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF },
    { -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019, -0x0019 } },
  { {  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF },
    { -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D, -0x000D } },
  { {  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF },
    { -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006, -0x0006 } },
  { {  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF },
    { -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003, -0x0003 } },
};

