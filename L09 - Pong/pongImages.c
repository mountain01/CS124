//	pongImages.c	04/10/2014
//******************************************************************************
//
#include "msp430x22x4.h"
#include "RBX430-1.h"
#include "pong.h"

//	lcd_bitImage(one, x, y, 1);
//******************************************************************************
const uint8 one[] = { 24, 21,		// 1
		0x00, 0x3e, 0x00,	//	-------- --ooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0xfe, 0x00,	//	-------- ooooooo- --------
		0x01, 0xfe, 0x00,	//	-------o ooooooo- --------
		0x03, 0xfe, 0x00,	//	------oo ooooooo- --------
		0x07, 0xfe, 0x00,	//	-----ooo ooooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x00, 0x7e, 0x00,	//	-------- -oooooo- --------
		0x07, 0xff, 0xe0,	//	-----ooo oooooooo ooo-----
		0x07, 0xff, 0xe0,	//	-----ooo oooooooo ooo-----
		0x07, 0xff, 0xe0,	//	-----ooo oooooooo ooo-----
		0x07, 0xff, 0xe0	//	-----ooo oooooooo ooo-----
};

//	lcd_bitImage(two, x, y, 1);
const uint8 two[] = { 24, 21,		// 2
		0x00, 0xff, 0x00,	//	-------- oooooooo --------
		0x07, 0xff, 0xc0,	//	-----ooo oooooooo oo------
		0x1f, 0xff, 0xf0,	//	---ooooo oooooooo oooo----
		0x3f, 0xff, 0xf8,	//	--oooooo oooooooo ooooo---
		0x7f, 0x03, 0xfc,	//	-ooooooo ------oo oooooo--
		0x7e, 0x01, 0xfe,	//	-oooooo- -------o ooooooo-
		0x00, 0x00, 0xfe,	//	-------- -------- ooooooo-
		0x00, 0x00, 0xfe,	//	-------- -------- ooooooo-
		0x00, 0x01, 0xfe,	//	-------- -------o ooooooo-
		0x00, 0x03, 0xfc,	//	-------- ------oo oooooo--
		0x00, 0x0f, 0xf8,	//	-------- ----oooo ooooo---
		0x00, 0x3f, 0xe0,	//	-------- --oooooo ooo-----
		0x00, 0xff, 0x80,	//	-------- oooooooo o-------
		0x03, 0xfe, 0x00,	//	------oo ooooooo- --------
		0x0f, 0xfc, 0x00,	//	----oooo oooooo-- --------
		0x1f, 0xf8, 0x00,	//	---ooooo ooooo--- --------
		0x3f, 0xf0, 0x00,	//	--oooooo oooo---- --------
		0x7f, 0xff, 0xfc,	//	-ooooooo oooooooo ooooooo--
		0x7f, 0xff, 0xfc,	//	-ooooooo oooooooo ooooooo--
		0x7f, 0xff, 0xfc,	//	-ooooooo oooooooo ooooooo--
		0x7f, 0xff, 0xfc	//	-ooooooo oooooooo ooooooo--
};

//	lcd_bitImage(three, x, y, 1);
const uint8 three[] = { 24, 21,		// 3
		0x00, 0xff, 0x00,	//	-------- oooooooo --------
		0x07, 0xff, 0xc0,	//	-----ooo oooooooo oo------
		0x1f, 0xff, 0xf0,	//	---ooooo oooooooo oooo----
		0x3f, 0xff, 0xf8,	//	--oooooo oooooooo ooooo---
		0x7f, 0x03, 0xfc,	//	-ooooooo ------oo oooooo--
		0x7e, 0x01, 0xfc,	//	-oooooo- -------o oooooo--
		0x00, 0x01, 0xfc,	//	-------- -------o oooooo--
		0x00, 0x03, 0xfc,	//	-------- ------oo oooooo--
		0x00, 0x07, 0xf8,	//	-------- -----ooo ooooo---
		0x00, 0x1f, 0xf0,	//	-------- ---ooooo oooo----
		0x00, 0x1f, 0xe0,	//	-------- ---ooooo ooo-----
		0x00, 0x1f, 0xf0,	//	-------- ---ooooo oooo----
		0x00, 0x07, 0xf8,	//	-------- -----ooo ooooo---
		0x00, 0x03, 0xfc,	//	-------- ------oo oooooo--
		0x00, 0x01, 0xfc,	//	-------- -------o oooooo--
		0x7e, 0x01, 0xfc,	//	-oooooo- -------o oooooo--
		0x7f, 0x03, 0xfc,	//	-ooooooo ------oo oooooo--
		0x3f, 0xff, 0xf8,	//	--oooooo oooooooo ooooo---
		0x1f, 0xff, 0xf0,	//	---ooooo oooooooo oooo----
		0x07, 0xff, 0xc0,	//	-----ooo oooooooo oo------
		0x00, 0xff, 0x00	//	-------- oooooooo --------
};


//	lcd_bitImage(go, x, y, 1);
const uint8 go[] = { 48, 21,		// GO!
		0x00, 0xfe, 0x00, 0x0f, 0xf0, 0x04,	//	-------- ooooooo- -------- ----oooo oooo---- -----o--
		0x03, 0xff, 0xc0, 0x3f, 0xfc, 0x0e,	//	------oo oooooooo oo------ --oooooo oooooo-- ----ooo-
		0x0f, 0xff, 0xf0, 0x7f, 0xfe, 0x1f,	//	----oooo oooooooo oooo---- -ooooooo ooooooo- ---ooooo
		0x3f, 0xff, 0xf8, 0xff, 0xff, 0x1f,	//	--oooooo oooooooo ooooo--- oooooooo oooooooo ---ooooo
		0x7f, 0x83, 0xf8, 0xfc, 0x3f, 0x1f,	//	-ooooooo o-----oo ooooo--- oooooo-- --oooooo ---ooooo
		0xfe, 0x00, 0xfc, 0xfc, 0x3f, 0x1f,	//	ooooooo- -------- oooooo-- oooooo-- --oooooo ---ooooo
		0xfc, 0x00, 0xfc, 0xfc, 0x3f, 0x1f,	//	ooooooo- -------- oooooo-- oooooo-- --oooooo ---ooooo
		0xfc, 0x00, 0x00, 0xfc, 0x3f, 0x1f,	//	oooooo-- -------- oooooo-- oooooo-- --oooooo ---ooooo
		0xfc, 0x00, 0x00, 0xfc, 0x3f, 0x1f,	//	oooooo-- -------- -------- oooooo-- --oooooo ---ooooo
		0xfc, 0x00, 0x00, 0xfc, 0x3f, 0x1f,	//	oooooo-- -------- -------- oooooo-- --oooooo ---ooooo
		0xfc, 0x00, 0x00, 0xfc, 0x3f, 0x1f,	//	oooooo-- -------- -------- oooooo-- --oooooo ---ooooo
		0xfc, 0x07, 0xfc, 0xfc, 0x3f, 0x1f,	//	oooooo-- -----ooo oooooo-- oooooo-- --oooooo ---ooooo
		0xfc, 0x07, 0xfc, 0xfc, 0x3f, 0x1f,	//	oooooo-- -----ooo oooooo-- oooooo-- --oooooo ---ooooo
		0xfc, 0x07, 0xfc, 0xfc, 0x3f, 0x1f,	//	oooooo-- -----ooo oooooo-- oooooo-- --oooooo ---ooooo
		0xfc, 0x00, 0xfc, 0xfc, 0x3f, 0x1f,	//	oooooo-- -------- oooooo-- oooooo-- --oooooo ---ooooo
		0x7e, 0x00, 0xfc, 0xfc, 0x3f, 0x00,	//	-oooooo- -------- oooooo-- oooooo-- --oooooo --------
		0x3f, 0x81, 0xfc, 0xfc, 0x3f, 0x0e,	//	--oooooo o------o oooooo-- oooooo-- --oooooo ----ooo-
		0x1f, 0xff, 0xf8, 0xff, 0xff, 0x1f,	//	---ooooo oooooooo ooooo--- oooooooo oooooooo ---ooooo
		0x0f, 0xff, 0xf0, 0x7f, 0xfe, 0x1f,	//	----oooo oooooooo oooo---- -ooooooo ooooooo- ---ooooo
		0x03, 0xff, 0xc0, 0x3f, 0xfc, 0x1f,	//	------oo oooooooo oo------ --oooooo oooooo-- ---ooooo
		0x00, 0xff, 0x00, 0x0f, 0xf0, 0x0e	//	-------- oooooooo -------- ----oooo oooo---- ----ooo-
};

const uint8* count_down[] = { go, one, two, three };


//	lcd_wordImage(pong_image, x, y, 1);
const uint16 pong_image[] = { 111, 99,
  0x0eff,0x7000,0xc410,0x02fe,0x87df,0x7390,0x11ff,0x0dff,0x7000,0xffd0,0x05fe,0x841f,
  0x10ff,0x0dff,0xffd0,0xc7df,0x4418,0x02f0,0x4208,0xc208,0xc618,0x01fe,0x871f,0x000e,
  0x0eff,0x0cff,0xfb80,0x01fe,0x4398,0x05f0,0x4208,0xc610,0x01fe,0x741f,0x0eff,0x0cff,
  0xffce,0xc61f,0x07f0,0x4208,0xfe18,0x01fe,0x0390,0x0dff,0x0cff,0x01fe,0x4218,0x08f0,
  0x4208,0xffd8,0x87df,0x0dff,0x0bff,0x8000,0xc7df,0x09f0,0x4208,0xc208,0x01fe,0x039f,
  0x0cff,0x0bff,0xf800,0x461f,0x0af0,0x4208,0xfe08,0x87df,0x0cff,0x0bff,0xfb80,0x4218,
  0x0bf0,0x4208,0xffd8,0x039f,0x0bff,0x0bff,0xfc00,0x4218,0x0bf0,0x4208,0xfe08,0x87df,
  0x0bff,0x0bff,0xfc00,0x0df0,0x4208,0x01fe,0x0010,0x0aff,0x0bff,0xfc00,0x0df0,0x4208,
  0xfe0e,0x77df,0x0aff,0x0bff,0xfc00,0x0df0,0x4208,0xc208,0x01fe,0x000e,0x09ff,0x0bff,
  0xffc0,0x0ef0,0x4208,0xffd8,0x039f,0x09ff,0x0bff,0xfe00,0x0ef0,0x4208,0xfa08,0x77df,
  0x09ff,0x0bff,0xfc00,0x4218,0x0df0,0x4208,0xc208,0x01fe,0x000e,0x08ff,0x0bff,0xfb80,
  0x4218,0x0ef0,0x4208,0xffd8,0x001f,0x08ff,0x0aff,0x8380,0xc390,0x421f,0x0ef0,0x4208,
  0xfe08,0x061f,0x08ff,0x09ff,0x7000,0xffd2,0x7416,0x461f,0x0ef0,0x4208,0xc208,0x87df,
  0x08ff,0x09ff,0xfc0e,0x959f,0x77d2,0x4710,0x0ff0,0x4208,0xffd8,0x0010,0x07ff,0x08ff,
  0x7000,0xb7df,0x9492,0x9592,0x87ce,0x0ff0,0x4208,0xfe08,0x039f,0x07ff,0x08ff,0xfc00,
  0x959f,0x9492,0xfc92,0xe60e,0x0ff0,0x4208,0xfa08,0x77df,0x07ff,0x08ff,0xffd6,0x9492,
  0x1492,0xb492,0xfb96,0x4218,0x0ef0,0x4208,0xc208,0x87df,0x07ff,0x07ff,0xb380,0x959f,
  0x9492,0x1492,0x9492,0x841f,0x421f,0x0ff0,0x4208,0xffd8,0x000e,0x06ff,0x07ff,0xffce,
  0x9496,0x9492,0x1492,0x9492,0x7596,0x461f,0x0ff0,0x4208,0xffc8,0x0018,0x06ff,0x06ff,
  0x7000,0xb7df,0x9492,0x9082,0x9492,0x9482,0x87d2,0xc7d0,0x0ff0,0x4208,0xfe08,0x039f,
  0x06ff,0x06ff,0xfb80,0x949f,0x9492,0x9082,0x9092,0x9492,0xfc92,0xffd0,0x4218,0x0ef0,
  0x4208,0xc208,0x041f,0x06ff,0x06ff,0xffce,0x9496,0x1492,0x1082,0x1492,0x9492,0xb492,
  0xfb92,0x4218,0x0ef0,0x4208,0xc208,0x07df,0x06ff,0x06ff,0xb7df,0x1492,0x9082,0x1492,
  0x9482,0x9182,0x9492,0x841f,0x461f,0x0ff0,0x4208,0x07d8,0x06ff,0x05ff,0x8000,0x959f,
  0x3092,0x9186,0x04f0,0x9492,0x77d6,0x47df,0x0ff0,0x4208,0x77d8,0x06ff,0x05ff,0xfc00,
  0x9496,0x9492,0x1086,0x9086,0x9492,0xb596,0x9496,0xfd92,0xe7ce,0x0ff0,0x4208,0x87d8,
  0x06ff,0x05ff,0xffce,0x9492,0x3192,0x3082,0x9482,0xfd96,0x01fe,0xb7df,0xb492,0xfb90,
  0x4218,0x0ef0,0x4208,0x77d8,0x06ff,0x05ff,0xb7d6,0x9492,0x3092,0x1182,0xb492,0xb7df,
  0x000e,0xb40e,0x949f,0x841f,0x461f,0x0ef0,0x4208,0x77d8,0x06ff,0x04ff,0x8000,0x959f,
  0x9492,0x3082,0x9086,0xfc92,0x041f,0x02ff,0x858e,0x77d2,0xc7d0,0x0ef0,0x4208,0x77d8,
  0x06ff,0x04ff,0xfb80,0x949f,0x1192,0x9082,0x9086,0xffd2,0x0010,0x02ff,0x7000,0xb590,
  0xfc0e,0x0df0,0x4208,0x8388,0x07df,0x06ff,0x04ff,0xfc00,0x9496,0x1086,0x1482,0x9182,
  0x87d2,0x04ff,0xb490,0x7396,0x421c,0x0bf0,0x4208,0x8608,0x8000,0x041f,0x06ff,0x04ff,
  0xb7ce,0x9492,0x9186,0x1492,0x9192,0x77d2,0x04ff,0x948e,0x77d6,0x460e,0x0af0,0x4208,
  0x738e,0x038e,0x8000,0x039f,0x06ff,0x04ff,0xb7d0,0x9492,0x9186,0x9492,0x9086,0x0496,
  0x04ff,0x9480,0xfd92,0x7390,0x08f0,0x4208,0xc608,0x77df,0x02ff,0x8000,0x0016,0x06ff,
  0x04ff,0x97d2,0x9492,0x9486,0x9492,0x9186,0x0416,0x04ff,0x9400,0xb492,0x769f,0x420e,
  0x06f0,0x4208,0xc208,0x861f,0x038e,0x02ff,0xfb80,0x000e,0x06ff,0x04ff,0x959f,0x9492,
  0x9486,0x9492,0x9482,0x0596,0x04ff,0x948e,0x9492,0xffd2,0x7390,0x05f0,0x4208,0xc208,
  0xc7d8,0x000e,0x03ff,0x8400,0x07ff,0x03ff,0x7000,0x959f,0x9492,0x9186,0x9492,0x9482,
  0x07d6,0x04ff,0x9596,0x9492,0xb492,0x769f,0x738e,0x03f0,0x4208,0xc208,0xff18,0x0398,
  0x04ff,0x77c0,0x07ff,0x03ff,0x7000,0x949f,0x3186,0x03f0,0x9492,0x87d2,0x03ff,0x8000,
  0x949f,0x02f0,0x9492,0xffd2,0x739a,0x7380,0x02f0,0x4208,0xc618,0x7398,0x05ff,0x0380,
  0x07ff,0x03ff,0x7000,0x949f,0x3186,0x9082,0x02f0,0x9492,0xfd92,0x03ff,0xb400,0x9496,
  0x9092,0x9082,0x9492,0x97d6,0x0390,0x7380,0x02f0,0x038e,0x000e,0x0dff,0x03ff,0x7000,
  0x949f,0x3186,0x9082,0x9492,0x9082,0xb492,0x0010,0x01ff,0x7000,0xffd0,0x3492,0x9486,
  0x02f0,0x9092,0xb492,0x97d6,0x0390,0x08ff,0x0400,0x07ff,0x03ff,0x7000,0x949f,0x9186,
  0x9082,0x9492,0x1082,0x9492,0x0396,0x01ff,0x8380,0x97df,0x3192,0x9482,0x1092,0x3186,
  0x02f0,0x9492,0xb596,0x8412,0x0390,0x06ff,0x8580,0x07ff,0x03ff,0x7000,0x959f,0x9486,
  0x02f0,0x9492,0x1182,0x9492,0x8492,0x838e,0xffd6,0x949f,0x3192,0x9486,0x9492,0x9086,
  0x9186,0x02f0,0x9492,0xfd96,0x87df,0x000e,0x05ff,0xf800,0x0010,0x06ff,0x04ff,0x959f,
  0x03f0,0x9492,0x3186,0x1486,0x9482,0xb492,0xb596,0x9492,0x3192,0x9092,0x9492,0x1092,
  0x9082,0x03f0,0x9492,0xffd6,0x87df,0x038e,0x04ff,0x7000,0x039f,0x06ff,0x04ff,0x97d6,
  0x1492,0x9492,0x9082,0x3186,0x9186,0x9082,0x03f0,0x9492,0x1082,0x3192,0x9492,0x1092,
  0x1082,0x9492,0x1492,0x9492,0xffd6,0x01fe,0x87df,0x000e,0x04ff,0x77d0,0x06ff,0x04ff,
  0xffd0,0x3492,0x1082,0x1492,0x1082,0x9482,0x03f0,0x9492,0x1492,0x9482,0x9492,0x9192,
  0x1092,0x02f0,0x9492,0x1492,0x9492,0x01fe,0x7390,0xffd0,0x039f,0x04ff,0xfc00,0x06ff,
  0x04ff,0xffc0,0x9496,0x9492,0x1092,0x1082,0x9482,0x1492,0x9192,0x9492,0x1092,0x9082,
  0x9492,0x3186,0x02f0,0x9492,0x1492,0x9492,0xb492,0x87df,0x01ff,0xf800,0x77df,0x04ff,
  0xf800,0x0010,0x05ff,0x04ff,0xfb80,0xb7df,0x9492,0x3182,0x3186,0x9492,0x1492,0x9192,
  0x9492,0x1092,0x9082,0x3186,0x3492,0x02f0,0x9492,0x1486,0x9492,0xb492,0x77df,0x02ff,
  0xffd0,0x05ff,0x041f,0x05ff,0x05ff,0xfc8e,0x959f,0x9492,0x9082,0x02f0,0x9492,0x9486,
  0x9492,0x1492,0x1082,0x3492,0x9082,0x9492,0x3192,0x3186,0x9492,0xfd92,0x041f,0x02ff,
  0xfc00,0x0010,0x04ff,0x77ce,0x05ff,0x06ff,0xfc0e,0x9496,0x02f0,0x9492,0x1092,0x9482,
  0x02f0,0x9492,0x9092,0x1082,0x9082,0x9492,0x3192,0x3186,0x9492,0xffd2,0x0396,0x02ff,
  0x8000,0x041f,0x04ff,0xfc00,0x000e,0x04ff,0x06ff,0x7000,0xfc90,0x9496,0x9492,0x9092,
  0x03f0,0x9492,0x3492,0x1092,0x9082,0x9492,0x3186,0x3486,0x9492,0xffd6,0x000e,0x03ff,
  0x07d0,0x04ff,0x8000,0x0010,0x04ff,0x05ff,0x0380,0x01ff,0x7000,0xfc90,0x9496,0x9492,
  0x9082,0x1182,0x9486,0x03f0,0x9492,0x3492,0x3092,0x9082,0xb492,0x87df,0x04ff,0x7380,
  0x05ff,0x041f,0x04ff,0x05ff,0x7400,0x02ff,0x7000,0xb590,0x9496,0x1492,0x3092,0x1082,
  0x1482,0x9482,0x9492,0x3492,0x1186,0x9486,0xfc92,0x741f,0x0aff,0x87ce,0x04ff,0x05ff,
  0x8000,0x000e,0x02ff,0x7000,0xb7d0,0x9492,0x1182,0x1486,0x9192,0x02f0,0x9492,0x1492,
  0x3092,0x9492,0xffd2,0x039f,0x0aff,0xfc00,0x000e,0x03ff,0x06ff,0x039f,0x01ff,0x7380,
  0x01ff,0x7000,0x9490,0x9492,0x02f0,0x1082,0x04f0,0x9492,0xb492,0xffd6,0x000e,0x0aff,
  0x8000,0x039f,0x03ff,0x06ff,0x0600,0x7000,0x0010,0x02ff,0x738e,0x03f0,0x9492,0x1092,
  0x9482,0x02f0,0x9492,0xfd92,0x741f,0x0cff,0x77df,0x03ff,0x06ff,0x7400,0x8380,0x02ff,
  0x8000,0x839f,0xb416,0x02f0,0x9492,0x3492,0x9186,0x02f0,0x9492,0xffd6,0x0010,0x0cff,
  0x87ce,0x03ff,0x06ff,0x8380,0x740e,0x02ff,0xfc00,0xfb90,0x7010,0xb7d2,0x9496,0x1092,
  0x9186,0x9492,0xfc92,0x87df,0x0dff,0xfb80,0x0010,0x02ff,0x06ff,0xf800,0x0396,0x02ff,
  0x02f0,0xffce,0x01ff,0x8380,0xb596,0x03f0,0x9492,0xffd6,0x0390,0x0dff,0x7000,0x041f,
  0x02ff,0x06ff,0xf800,0x000e,0x01ff,0x7000,0x77df,0x87d0,0x02ff,0x8380,0x8410,0x9492,
  0xfd92,0x77df,0x0fff,0x87ce,0x02ff,0x06ff,0x8400,0x02ff,0x8000,0x041f,0x07df,0x03ff,
  0x7000,0xfd90,0x01fe,0x0010,0x0fff,0xfb80,0x0010,0x01ff,0x06ff,0x0400,0x02ff,0xfc00,
  0x7010,0x0392,0x03ff,0x940e,0x01fe,0x0412,0x10ff,0x7000,0x039f,0x01ff,0x06ff,0x040e,
  0x02ff,0x87ce,0x7000,0x000e,0x7000,0x840e,0xffd0,0xb7df,0x0390,0x12ff,0x77ce,0x01ff,
  0x06ff,0x0390,0x02ff,0x07d0,0x01ff,0x840e,0x02fe,0x7410,0x14ff,0x8380,0x000e,0x05ff,
  0x7000,0x0010,0x01ff,0x7000,0x039f,0xfb80,0x01fe,0x741f,0x000e,0x16ff,0x0010,0x05ff,
  0x8000,0x02ff,0xfb80,0x000e,0xffd0,0x77df,0x19ff,0x05ff,0x8380,0x02ff,0x8400,0x7000,
  0x01fe,0x000e,0x17ff,0x7000,0x001f,0x05ff,0x07c0,0x02ff,0x77d0,0xfb80,0x041f,0x18ff,
  0xfc00,0x0010,0x05ff,0x0390,0x01ff,0x7000,0x041f,0xfc00,0x039f,0x17ff,0x7000,0x87df,
  0x01ff,0x04ff,0x7000,0x0010,0x01ff,0x8000,0x0010,0xffd0,0x000e,0x16ff,0x038e,0xfc00,
  0x041f,0x01ff,0x04ff,0xf800,0x000e,0x01ff,0xfb80,0x7000,0x87df,0x16ff,0x7000,0x7390,
  0xffd0,0x0010,0x01ff,0x04ff,0x8400,0x02ff,0x87c0,0x8000,0x07df,0x17ff,0xfb80,0x77df,
  0x02ff,0x04ff,0x0410,0x02ff,0x0410,0xfb80,0x039f,0x16ff,0x7000,0x01fe,0x0390,0x02ff,
  0x03ff,0x7000,0x001f,0x01ff,0x7000,0x0390,0xffc0,0x0010,0x16ff,0xffce,0x741f,0x03ff,
  0x03ff,0xf800,0x000e,0x01ff,0x8000,0x0010,0xffd0,0x000e,0x16ff,0x738e,0x000e,0x03ff,
  0x03ff,0x8400,0x02ff,0x8000,0x700e,0x87df,0x1cff,0x03ff,0x0410,0x02ff,0x8400,0x8000,
  0x041f,0x1cff,0x02ff,0x7000,0x0010,0x02ff,0x07ce,0xfb80,0x039f,0x1cff,0x02ff,0xfb80,
  0x03ff,0x0410,0xfc00,0x0010,0x1cff,0x02ff,0x7400,0x02ff,0x7000,0x0390,0xffce,0x000e,
  0x1cff,0x02ff,0x0390,0x03ff,0x000e,0x87df,0x1dff,0x01ff,0x8000,0x0010,0x03ff,0x7000,
  0x07df,0x1dff,0x01ff,0x8400,0x03ff,0x8380,0xf800,0x041f,0x1dff,0x01ff,0x040e,0x03ff,
  0x07c0,0xfc00,0x001f,0x1dff,0x01ff,0x0010,0x03ff,0x040e,0xffc0,0x0010,0x1dff,0x8000,
  0x000e,0x03ff,0x039f,0xffd0,0x1eff,0x7400,0x03ff,0x8000,0x001f,0x77df,0x1eff,0x0380,
  0x03ff,0xf800,0x8010,0x07df,0x1eff,0x04ff,0xfb80,0xf800,0x039f,0x1eff,0x04ff,0x87c0,
  0xfc00,0x001f,0x1eff,0x01ff,0x87ce,0x000e,0x01ff,0x07ce,0xffc0,0x000e,0x1eff,0x01ff,
  0xfb80,0x041f,0x01ff,0x0390,0x87d0,0x1fff,0x02ff,0xffd0,0x0390,0x700e,0x77df,0x1fff,
  0x02ff,0x8000,0x87df,0xfc0e,0x041f,0x1fff,0x03ff,0xfc0e,0x01fe,0x039f,0x1fff,0x04ff,
  0xffd0,0x0010,0x1fff
};