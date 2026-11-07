/**
  ******************************************************************************
  * 简介：适用于pal库oled驱动的字体文件
  *       该文件由bdf2c软件自动生成，工具获取方式 Bilibili关注"铁头山羊"
  * 字体名称：-FreeType-HYJinKaiJ-Medium-R-Normal--11-80-100-100-P-92-ISO10646-1
  * 字体字号：8磅
  * 字符数量：5/7911
  * 备注：123
  ******************************************************************************
*/

#ifndef hyjka8_H_
#define hyjka8_H_

#include "oled_font.h"

/* 字形数据 */
static const uint8_t hyjka8_GlyphBitmap_524D[] = {0x4C,0x08,0x7F,0x80,0xEA,0xEA,0xAA,0xAA,0xA2,0xA6,}; // 前
static const uint8_t hyjka8_GlyphBitmap_5F0F[] = {0x0A,0x00,0x09,0x00,0x0A,0x00,0x7C,0x00,0x14,0x00,0x64,0x00,0x26,0x00,0x32,0x80,0xC1,0x80,0x00,0x80,}; // 式
static const uint8_t hyjka8_GlyphBitmap_5F53[] = {0x10,0x92,0x54,0x52,0x3E,0xC2,0x7A,0x02,0x1E,0x60,}; // 当
static const uint8_t hyjka8_GlyphBitmap_6A21[] = {0x20,0x80,0x24,0xC0,0x27,0x00,0x7A,0x80,0x7E,0x80,0x67,0x00,0xBF,0xC0,0x25,0x00,0x24,0x80,0x28,0x60,}; // 模
static const uint8_t hyjka8_GlyphBitmap_FF1A[] = {0x80,0x00,0x80,}; // ：

/* 映射表 */
static const uint32_t hyjka8_Map[] = {21069,24335,24403,27169,65306,};

/* 字形 */
static const Glyph_TypeDef hyjka8_Glyphs[] = {
	// 前
	{
		.Name = "524D", 
		.Encoding = 21069, 
		.Swx0 = 990, 
		.Swy0 = 0, 
		.Dwx0 = 11, 
		.Dwy0 = 0, 
		.BBw = 8, 
		.BBh = 10, 
		.BBxoff0x = 2, 
		.BByoff0y = -2, 
		.nBytes = 10, 
		.Bitmap = hyjka8_GlyphBitmap_524D, 
	},
	// 式
	{
		.Name = "5F0F", 
		.Encoding = 24335, 
		.Swx0 = 990, 
		.Swy0 = 0, 
		.Dwx0 = 11, 
		.Dwy0 = 0, 
		.BBw = 9, 
		.BBh = 10, 
		.BBxoff0x = 1, 
		.BByoff0y = -2, 
		.nBytes = 20, 
		.Bitmap = hyjka8_GlyphBitmap_5F0F, 
	},
	// 当
	{
		.Name = "5F53", 
		.Encoding = 24403, 
		.Swx0 = 990, 
		.Swy0 = 0, 
		.Dwx0 = 11, 
		.Dwy0 = 0, 
		.BBw = 7, 
		.BBh = 10, 
		.BBxoff0x = 2, 
		.BByoff0y = -1, 
		.nBytes = 10, 
		.Bitmap = hyjka8_GlyphBitmap_5F53, 
	},
	// 模
	{
		.Name = "6A21", 
		.Encoding = 27169, 
		.Swx0 = 990, 
		.Swy0 = 0, 
		.Dwx0 = 11, 
		.Dwy0 = 0, 
		.BBw = 11, 
		.BBh = 10, 
		.BBxoff0x = 0, 
		.BByoff0y = -2, 
		.nBytes = 20, 
		.Bitmap = hyjka8_GlyphBitmap_6A21, 
	},
	// ：
	{
		.Name = "FF1A", 
		.Encoding = 65306, 
		.Swx0 = 990, 
		.Swy0 = 0, 
		.Dwx0 = 11, 
		.Dwy0 = 0, 
		.BBw = 1, 
		.BBh = 3, 
		.BBxoff0x = 1, 
		.BByoff0y = -2, 
		.nBytes = 3, 
		.Bitmap = hyjka8_GlyphBitmap_FF1A, 
	},
};

/* 字体文件 */
const Font_TypeDef hyjka8 =
{
	.SpecVersion = "2.1",
	.FontName = "-FreeType-HYJinKaiJ-Medium-R-Normal--11-80-100-100-P-92-ISO10646-1",
	.MetricsSet = 0,
	.FontSize = 8,
	.Xres = 100,
	.Yres = 100,
	.FBBx = 12,
	.FBBy = 16,
	.FBBXoff = 0,
	.FBBYoff = -4,
	.nChars = 5,
	.Map = hyjka8_Map,
	.Glyphs = hyjka8_Glyphs,
};

#endif
