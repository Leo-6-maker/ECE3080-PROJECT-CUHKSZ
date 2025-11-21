#ifndef __EIE3810_TFTLCD_H
#define __EIE3810_TFTLCD_H
#include "stm32f10x.h"


/* define each color */
#define BLACK 0x0000
#define WHITE 0xFFFF
#define BLUE 0x001F
#define GREEN 0x07E0
#define RED 0xF800
#define YELLOW 0xFFE0

/* the address of the FSMC memory for command */
#define LCD_COMMAND ((u32)0x6C000000)
/* the address of the FSMC memory for data */
#define LCD_DATA ((u32)0x6C000800)
/* turn on the LCD light by set the PB0 to 1 */
#define LCD_LIGHT_ON GPIOB->BSRR |= 0x00000001 // set the BS0 to 1

/* initialize the TFTLCD */
void EIE3810_TFTLCD_Init(void);
/* set the parameter of the TFTLCD */
void EIE3810_TFTLCD_SetParameter(void);
/* write a 16-bit command and then a 16-bit data to the TFTLCD */
void EIE3810_TFTLCD_WrCmdData(u16 cmd, u16 data);
/* write a 16-bit command to the TFTLCD */
void EIE3810_TFTLCD_WrCmd(u16 cmd);
/* write a 16-bit data to the TFTLCD */
void EIE3810_TFTLCD_WrData(u16 data);
/* clear the screen with color */
void EIE3810_TFTLCD_Clear(u16 color);

/* draw a dot with the index of x and y */
void EIE3810_TFTLCD_DrawDot(
    u16 x, u16 y, u16 colors);
/* fill a rectangle with indices and sizes */
void EIE3810_TFTLCD_DrawRectangle(
    u16 start_x, u16 length_x,
    u16 start_y, u16 length_y, u16 color);
/* x0, y0 : center of the circle; r: radius;
 * full: whether the circle is hollow or solid;
 * color: color of the circle. */
void EIE3810_TFTLCD_DrawCircle(
    u16 x0, u16 y0, u8 r, u8 full, u16 color);

/* show the seven segment based on digit */
void EIE3810_TFTLCD_SevenSegment(
    u16 start_x, u16 start_y, u8 digit, u16 color, u16 bgcolor);

/* show 12 * 06 fonts */
void EIE3810_TFTLCD_ShowChar1206(
    u16 x, u16 y, u8 ascii, u16 color, u16 bgcolor);
/* show 16 * 08 fonts */
void EIE3810_TFTLCD_ShowChar1608(
    u16 x, u16 y, u8 ASCII, u16 color, u16 bgcolor);
/* show 24 * 12 fonts */
void EIE3810_TFTLCD_ShowChar2412(
    u16 x, u16 y, u8 ascii, u16 color, u16 bgcolor);
/* show string */
void EIE3810_TFTLCD_ShowString(u16 x, u16 y, char *thestr, u16 color, u16 bgcolor, u16 size_flag);

#endif
