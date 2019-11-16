#include "st7735s.h"

#include <algorithm>
#include <array>
#include <lib/systick.h>
#include <libopencm3/stm32/spi.h>
#include <lib/rcc.h>

namespace {
constexpr std::uint8_t chars_w = 8;
constexpr std::uint8_t chars_h = 16;
constexpr std::array<std::array<unsigned char, 16>, 95> chars = {{
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*" ",0*/
                                                                     {0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18,
                                                                      0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
                                                                      0x00},/*"!",1*/
                                                                     {0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x00, 0x00,
                                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*""",2*/
                                                                     {0x00, 0x00, 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C,
                                                                      0x6C, 0xFE, 0x6C, 0x6C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"#",3*/
                                                                     {0x00, 0x18, 0x18, 0x3C, 0x66, 0x06, 0x0C, 0x18,
                                                                      0x30, 0x60, 0x66, 0x3C, 0x18, 0x18, 0x00,
                                                                      0x00},/*"$",4*/
                                                                     {0x00, 0x00, 0x0E, 0x1B, 0x5B, 0x6E, 0x30, 0x18,
                                                                      0x0C, 0x76, 0xDA, 0xD8, 0x70, 0x00, 0x00,
                                                                      0x00},/*"%",5*/
                                                                     {0x00, 0x00, 0x00, 0x1C, 0x36, 0x36, 0x1C, 0x06,
                                                                      0xF6, 0x66, 0x66, 0xDC, 0x00, 0x00, 0x00,
                                                                      0x00},/*"&",6*/
                                                                     {0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00,
                                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*"'",7*/
                                                                     {0x00, 0x00, 0x00, 0x30, 0x18, 0x18, 0x0C, 0x0C,
                                                                      0x0C, 0x0C, 0x0C, 0x18, 0x18, 0x30, 0x00,
                                                                      0x00},/*"(",8*/
                                                                     {0x00, 0x00, 0x00, 0x0C, 0x18, 0x18, 0x30, 0x30,
                                                                      0x30, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x00,
                                                                      0x00},/*")",9*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x38, 0xFE,
                                                                      0x38, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*"*",10*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E,
                                                                      0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*"+",11*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00, 0x00, 0x38, 0x38, 0x30, 0x18, 0x00,
                                                                      0x00},/*",",12*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E,
                                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*"-",13*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00,
                                                                      0x00},/*".",14*/
                                                                     {0x00, 0x00, 0x00, 0x60, 0x60, 0x30, 0x30, 0x18,
                                                                      0x18, 0x0C, 0x0C, 0x06, 0x06, 0x00, 0x00,
                                                                      0x00},/*"/",15*/
                                                                     {0x00, 0x00, 0x00, 0x78, 0xCC, 0xEC, 0xEC, 0xCC,
                                                                      0xDC, 0xDC, 0xCC, 0x78, 0x00, 0x00, 0x00,
                                                                      0x00},/*"0",16*/
                                                                     {0x00, 0x00, 0x00, 0x30, 0x38, 0x3E, 0x30, 0x30,
                                                                      0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00,
                                                                      0x00},/*"1",17*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x60, 0x30,
                                                                      0x18, 0x0C, 0x06, 0x7E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"2",18*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x60, 0x38,
                                                                      0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"3",19*/
                                                                     {0x00, 0x00, 0x00, 0x0C, 0x0C, 0x6C, 0x6C, 0x6C,
                                                                      0x66, 0xFE, 0x60, 0x60, 0x00, 0x00, 0x00,
                                                                      0x00},/*"4",20*/
                                                                     {0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x3E,
                                                                      0x60, 0x60, 0x30, 0x1E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"5",21*/
                                                                     {0x00, 0x00, 0x00, 0x38, 0x18, 0x0C, 0x3E, 0x66,
                                                                      0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"6",22*/
                                                                     {0x00, 0x00, 0x00, 0x7E, 0x60, 0x30, 0x30, 0x18,
                                                                      0x18, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"7",23*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x6E, 0x3C,
                                                                      0x76, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"8",24*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66,
                                                                      0x7C, 0x30, 0x18, 0x1C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"9",25*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00,
                                                                      0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00,
                                                                      0x00},/*":",26*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00,
                                                                      0x00, 0x00, 0x38, 0x38, 0x30, 0x18, 0x00,
                                                                      0x00},/*";",27*/
                                                                     {0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06,
                                                                      0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00,
                                                                      0x00},/*"<",28*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00,
                                                                      0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*"=",29*/
                                                                     {0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60,
                                                                      0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00,
                                                                      0x00},/*">",30*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x30, 0x18,
                                                                      0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
                                                                      0x00},/*"?",31*/
                                                                     {0x00, 0x00, 0x00, 0x7E, 0xC3, 0xC3, 0xF3, 0xDB,
                                                                      0xDB, 0xF3, 0x03, 0xFE, 0x00, 0x00, 0x00,
                                                                      0x00},/*"@",32*/
                                                                     {0x00, 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x66,
                                                                      0x7E, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"A",33*/
                                                                     {0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E,
                                                                      0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"B",34*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x06, 0x06,
                                                                      0x06, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"C",35*/
                                                                     {0x00, 0x00, 0x00, 0x1E, 0x36, 0x66, 0x66, 0x66,
                                                                      0x66, 0x66, 0x36, 0x1E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"D",36*/
                                                                     {0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x3E,
                                                                      0x06, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"E",37*/
                                                                     {0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x3E,
                                                                      0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00,
                                                                      0x00},/*"F",38*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x06, 0x06,
                                                                      0x76, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"G",39*/
                                                                     {0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7E,
                                                                      0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"H",40*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18,
                                                                      0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"I",41*/
                                                                     {0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60,
                                                                      0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"J",42*/
                                                                     {0x00, 0x00, 0x00, 0x66, 0x66, 0x36, 0x36, 0x1E,
                                                                      0x36, 0x36, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"K",43*/
                                                                     {0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06,
                                                                      0x06, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"L",44*/
                                                                     {0x00, 0x00, 0x00, 0xC6, 0xC6, 0xEE, 0xD6, 0xD6,
                                                                      0xD6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00,
                                                                      0x00},/*"M",45*/
                                                                     {0x00, 0x00, 0x00, 0xC6, 0xC6, 0xCE, 0xDE, 0xF6,
                                                                      0xE6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00,
                                                                      0x00},/*"N",46*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"O",47*/
                                                                     {0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E,
                                                                      0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00,
                                                                      0x00},/*"P",48*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x3C, 0x30, 0x60, 0x00,
                                                                      0x00},/*"Q",49*/
                                                                     {0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E,
                                                                      0x36, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"R",50*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x18,
                                                                      0x30, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"S",51*/
                                                                     {0x00, 0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18,
                                                                      0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00,
                                                                      0x00},/*"T",52*/
                                                                     {0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"U",53*/
                                                                     {0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66,
                                                                      0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00,
                                                                      0x00},/*"V",54*/
                                                                     {0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xD6, 0xD6,
                                                                      0xD6, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"W",55*/
                                                                     {0x00, 0x00, 0x00, 0x66, 0x66, 0x2C, 0x18, 0x18,
                                                                      0x34, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"X",56*/
                                                                     {0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C,
                                                                      0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00,
                                                                      0x00},/*"Y",57*/
                                                                     {0x00, 0x00, 0x00, 0x7E, 0x60, 0x60, 0x30, 0x18,
                                                                      0x0C, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"Z",58*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C,
                                                                      0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C,
                                                                      0x00},/*"[",59*/
                                                                     {0x00, 0x00, 0x00, 0x06, 0x06, 0x0C, 0x0C, 0x18,
                                                                      0x18, 0x30, 0x30, 0x60, 0x60, 0x00, 0x00,
                                                                      0x00},/*"\",60*/
                                                                     {0x00, 0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30,
                                                                      0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C,
                                                                      0x00},/*"]",61*/
                                                                     {0x00, 0x18, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*"^",62*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
                                                                      0x00},/*"_",63*/
                                                                     {0x00, 0x1C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*"`",64*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x60, 0x60,
                                                                      0x7C, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"a",65*/
                                                                     {0x00, 0x00, 0x00, 0x06, 0x06, 0x3E, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"b",66*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06,
                                                                      0x06, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"c",67*/
                                                                     {0x00, 0x00, 0x00, 0x60, 0x60, 0x7C, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"d",68*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66,
                                                                      0x7E, 0x06, 0x06, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"e",69*/
                                                                     {0x00, 0x00, 0x00, 0x78, 0x0C, 0x0C, 0x0C, 0x7E,
                                                                      0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"f",70*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x3E,
                                                                      0x00},/*"g",71*/
                                                                     {0x00, 0x00, 0x00, 0x06, 0x06, 0x3E, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"h",72*/
                                                                     {0x00, 0x00, 0x18, 0x18, 0x00, 0x1E, 0x18, 0x18,
                                                                      0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"i",73*/
                                                                     {0x00, 0x00, 0x30, 0x30, 0x00, 0x3C, 0x30, 0x30,
                                                                      0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1E,
                                                                      0x00},/*"j",74*/
                                                                     {0x00, 0x00, 0x00, 0x06, 0x06, 0x66, 0x66, 0x36,
                                                                      0x1E, 0x36, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"k",75*/
                                                                     {0x00, 0x00, 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18,
                                                                      0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"l",76*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xD6, 0xD6,
                                                                      0xD6, 0xD6, 0xD6, 0xC6, 0x00, 0x00, 0x00,
                                                                      0x00},/*"m",77*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"n",78*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"o",79*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06,
                                                                      0x00},/*"p",80*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60,
                                                                      0x00},/*"q",81*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x76, 0x0E,
                                                                      0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00,
                                                                      0x00},/*"r",82*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x06, 0x06,
                                                                      0x3C, 0x60, 0x60, 0x3E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"s",83*/
                                                                     {0x00, 0x00, 0x00, 0x0C, 0x0C, 0x7E, 0x0C, 0x0C,
                                                                      0x0C, 0x0C, 0x0C, 0x78, 0x00, 0x00, 0x00,
                                                                      0x00},/*"t",84*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"u",85*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
                                                                      0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00,
                                                                      0x00},/*"v",86*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xD6, 0xD6,
                                                                      0xD6, 0xD6, 0x6C, 0x6C, 0x00, 0x00, 0x00,
                                                                      0x00},/*"w",87*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x3C,
                                                                      0x18, 0x3C, 0x66, 0x66, 0x00, 0x00, 0x00,
                                                                      0x00},/*"x",88*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
                                                                      0x66, 0x66, 0x66, 0x3C, 0x30, 0x18, 0x0F,
                                                                      0x00},/*"y",89*/
                                                                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x60, 0x30,
                                                                      0x18, 0x0C, 0x06, 0x7E, 0x00, 0x00, 0x00,
                                                                      0x00},/*"z",90*/
                                                                     {0x00, 0x00, 0x00, 0x30, 0x18, 0x18, 0x18, 0x0C,
                                                                      0x06, 0x0C, 0x18, 0x18, 0x18, 0x30, 0x00,
                                                                      0x00},/*"{",91*/
                                                                     {0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18,
                                                                      0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
                                                                      0x00},/*"|",92*/
                                                                     {0x00, 0x00, 0x00, 0x0C, 0x18, 0x18, 0x18, 0x30,
                                                                      0x60, 0x30, 0x18, 0x18, 0x18, 0x0C, 0x00,
                                                                      0x00},/*"}",93*/
                                                                     {0x00, 0x00, 0x00, 0x8E, 0xDB, 0x71, 0x00, 0x00,
                                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                      0x00},/*"~",94*/
                                                                 }};
}  // namespace

st7735s::st7735s(std::uint8_t x, std::uint8_t y, std::uint8_t w, std::uint8_t h, color_mode cm) :
    _x_offset_(x), _y_offset_(y), _width_(w), _height_(h) {
  // Init RCC
  rcc::periph_clock_enable(RCC_SPI1);
  rcc::periph_clock_enable(RCC_GPIOA);
  rcc::periph_clock_enable(RCC_GPIOB);

  // Init GPIO
  _sck_.setup(GPIO_MODE_AF, GPIO_PUPD_NONE);
  _sck_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _sck_.set_af(GPIO_AF5);
  _mosi_.setup(GPIO_MODE_AF, GPIO_PUPD_NONE);
  _mosi_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _mosi_.set_af(GPIO_AF5);
  _res_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _res_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _dc_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _dc_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _cs_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _cs_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _bl_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _bl_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);

  // Init SPI
  spi_set_master_mode(_spi_);
  // 18MBit/s with 72MHz clock
  spi_set_baudrate_prescaler(_spi_, SPI_CR1_BR_FPCLK_DIV_4);
  spi_set_clock_polarity_0(_spi_);
  spi_set_clock_phase_0(_spi_);
  spi_set_bidirectional_transmit_only_mode(_spi_);
  spi_set_data_size(_spi_, SPI_CR2_DS_8BIT);
  spi_send_msb_first(_spi_);
  /*
   * Set NSS management to software.
   *
   * Note:
   * Setting nss high is very important, even if we are controlling the GPIO
   * ourselves this bit needs to be at least set to 1, otherwise the spi
   * peripheral will not send any data out.
  */
  spi_enable_software_slave_management(_spi_);
  spi_set_nss_high(_spi_);
  spi_enable(_spi_);

  // Init LCD

  unselect();
  hw_reset();

  // LCD Command Init Block
  {
    sw_reset();
    slp_out();
    set_orientation(2);
    set_gamma(2);
    set_color_mode(cm);
    display_enable();
  }

  // Blank screen
  draw_screen(0xFFFFFF);

  backlight(true);
}

void st7735s::select() {
  _cs_.state(false);
}

void st7735s::unselect() {
  _cs_.state(true);
}

void st7735s::hw_reset() {
  _res_.state(false);
  systick::sleep_ms(5);
  _res_.state(true);
}

void st7735s::sw_reset() {
  send_cmd(CMD_SWRESET);
  systick::sleep_ms(150);
}

void st7735s::slp_out() {
  send_cmd(CMD_SLPOUT);
  systick::sleep_ms(150);
}

void st7735s::display_enable() {
  send_cmd(CMD_DISPON);
  systick::sleep_ms(150);
}

void st7735s::backlight(bool state) {
  _bl_.state(state);
}

void st7735s::set_draw_order(refresh_order_v rov, refresh_order_h roh, color_order co) {
  send_cmd(CMD_MADCTL);
  send_data(rov << 4 | roh << 2 | co << 3);
}

void st7735s::set_inverted(bool invert) {
  send_cmd(invert ? CMD_INVON : CMD_INVOFF);
}

void st7735s::set_color_mode(color_mode cm) {
  send_cmd(CMD_COLMOD);
  send_data(cm);
}

void st7735s::set_orientation(std::uint8_t orientation) {
  uint8_t current_w = _width_;
  uint8_t current_h = _height_;
  send_cmd(CMD_MADCTL);

  switch (orientation) {
    case 1:
      send_data(0x60); /* MX + MV */
      _width_ = current_h;
      _height_ = current_w;
      set_window(0, 0, current_h - 1, current_w - 1);
      break;

    case 2:
      send_data(0xC0); /* MY + MX */
      _width_ = current_w;
      _height_ = current_h;
      set_window(0, 0, current_w - 1, current_h - 1);
      break;

    case 3:
      send_data(0xA0); /* MY + MV */
      _width_ = current_h;
      _height_ = current_w;
      set_window(0, 0, current_h - 1, current_w - 1);
      break;

    default:
      send_data(0x00); /* None */
      _width_ = current_w;
      _height_ = current_h;
      set_window(0, 0, current_w - 1, current_h - 1);
      break;
  }
}

void st7735s::set_gamma(std::uint8_t gamma) {
  switch (gamma) {
    case 1:
      gamma = 2;
      break; /* GS_pin=1: 1.8; GS_pin=0: 2.5 */
    case 2:
      gamma = 4;
      break; /* GS_pin=1: 2.5; GS_pin=0: 2.2 */
    case 3:
      gamma = 8;
      break; /* GS_pin=1: 1.0; GS_pin=0: 1.8 */
    default:
      gamma = 1;
      break; /* GS_pin=1: 2.2; GS_pin=0: 1.0 */
  }

  send_cmd(CMD_GAMSET);
  send_data(gamma);
}

bool st7735s::set_window(std::uint8_t x1, std::uint8_t y1, std::uint8_t x2, std::uint8_t y2) {
  /* Accept: 0 <= x1 <= x2 < width */
  if (x2 < x1) { return false; }
  if (x2 >= _width_) { return false; }

  /* Accept: 0 <= y1 <= y2 < height */
  if (y2 < y1) { return false; }
  if (y2 >= _height_) { return false; }

  /* Set column address */
  send_cmd(CMD_CASET);
  send_data(0);
  send_data(x1 + _x_offset_);
  send_data(0);
  send_data(x2 + _x_offset_);

  /* Set row address */
  send_cmd(CMD_RASET);
  send_data(0);
  send_data(y1 + _y_offset_);
  send_data(0);
  send_data(y2 + _y_offset_);

  /* Activate RAM write */
  send_cmd(CMD_RAMWR);
  return true;
}

void st7735s::draw_char(std::uint8_t x, std::uint8_t y, char c, std::uint32_t fg_rgb, std::uint32_t bg_rgb) {
  const std::uint8_t x_max = x + chars_w - 1;
  const std::uint8_t y_max = y + chars_h - 1;

  if (std::clamp<std::uint8_t>(x_max, 1, _width_) != x_max) {
    return;
  }
  if (std::clamp<std::uint8_t>(y_max, 1, _height_) != y_max) {
    return;
  }

  const char pos = c - ' ';

  if (!set_window(x, y, x_max, y_max)) { return; }
  for (std::uint8_t h = 0; h < chars_h; ++h) {
    std::uint8_t col = chars[pos][h];

    for (std::uint8_t c = 0; c < chars_w; ++c) {
      if (col & 0x01) {
        send_data(fg_rgb >> 16 & 0xFF);
        send_data(fg_rgb >> 8 & 0xFF);
        send_data(fg_rgb & 0xFF);
      } else {
        send_data(bg_rgb >> 16 & 0xFF);
        send_data(bg_rgb >> 8 & 0xFF);
        send_data(bg_rgb & 0xFF);
      }

      col >>= 1;
    }
  }
}

void st7735s::draw_string(std::uint8_t x, std::uint8_t y, std::string_view sv, std::uint32_t fg_rgb, std::uint32_t bg_rgb) {
  for (const auto c : sv) {
    draw_char(x, y, c, fg_rgb, bg_rgb);
    x += chars_w;
  }
}

void st7735s::draw_rect(std::uint8_t x, std::uint8_t y, std::uint8_t w, std::uint8_t h, std::uint32_t rgb) {
  /* Draw only in the display space */
  if ((w == 0) || (h == 0)) { return; }
  if ((x + w - 1) >= _width_) { w = _width_ - x; }
  if ((y + h - 1) >= _height_) { h = _height_ - y; }

  /* Draw the filed rectangle */
  if (!set_window(x, y, x + w - 1, y + h - 1)) { return; }
  while (w--) {
    while (h--) {
      send_data(rgb >> 16 & 0xFF);
      send_data(rgb >> 8 & 0xFF);
      send_data(rgb & 0xFF);
    }
  }
}

void st7735s::draw_screen(std::uint32_t rgb) {
  draw_rect(0, 0, _width_, _height_, rgb);
}

void st7735s::send_cmd(command cmd) {
  _dc_.state(false);
  select();
  spi_send8(_spi_, cmd);
  unselect();
}

void st7735s::send_data(std::uint8_t data) {
  _dc_.state(true);
  select();
  spi_send8(_spi_, data);
  unselect();
}

uint8_t st7735s::x_offset() const {
  return _x_offset_;
}

uint8_t st7735s::y_offset() const {
  return _y_offset_;
}

uint8_t st7735s::width() const {
  return _width_;
}

uint8_t st7735s::height() const {
  return _height_;
}
