/*
* SSD1331.cpp
* A library for RGB OLED module
*
* Copyright (c) 2014 seeed technology inc.
* Copyright (c) 2012, Adafruit Industries.
*
* All rights reserved.
*
* This library is based on Adafruit's SSD1331-OLED-Driver-Library. Thanks to 
* their contribution to the code, we modify it and add more interface to 
* support our Seeed's Xadow RGB OLED 96*64 module.
*
* Below is the introduction of Adafruit's Color OLED module, we add it to here
* to express our thanks to them.
*
* ****************************************************************************
* This is a library for the 0.96" 16-bit Color OLED with SSD1331 driver chip
*
*  Pick one up today in the adafruit shop!
*  ------> http://www.adafruit.com/products/684
*
* These displays use SPI to communicate.
*
* Adafruit invests time and resources providing this open source code, 
* please support Adafruit and open-source hardware by purchasing 
* products from Adafruit!
*
* Written by Limor Fried/Ladyada for Adafruit Industries.
* Modifed by lawliet for Seeed Studio's RGB OLED module.
* BSD license, all text above must be included in any redistribution
* ******************************************************************************
*
* Software License Agreement (BSD License)
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holders nor the
* names of its contributors may be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/* adapted for kianv riscv soc (c) 2021 by Hirosh Dabui */
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "kianv_stdlib.h"
#include "SSD1331.h"



#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8

void drawPixel(uint16_t x, uint16_t y, uint16_t color);

const unsigned char SeeedLogo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x80, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x0F, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x00, 0x01, 0xE0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x01, 0xE0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x3E, 0x00, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7C, 0x00, 0x00, 0xF8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x7C, 0x00, 0x00, 0xF8,
    0x00, 0x60, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x00, 0x60, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x00,
    0xFC, 0x00, 0x00, 0xFC, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00,
    0xFC, 0x00, 0x00, 0xFC,
    0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0xFC, 0x00, 0x00, 0xFC,
    0x00, 0xE0, 0x00, 0x00,
    0x00, 0x00, 0x1C, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x00, 0xF0, 0x00, 0x00,
    0x00, 0x00, 0x1E, 0x00,
    0xFC, 0x00, 0x00, 0xFC, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00,
    0xFC, 0x00, 0x00, 0xFC,
    0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0xFC, 0x00, 0x00, 0xFC,
    0x01, 0xE0, 0x00, 0x00,
    0x00, 0x00, 0x1F, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x03, 0xE0, 0x00, 0x00,
    0x00, 0x00, 0x1F, 0x80,
    0xFC, 0x00, 0x00, 0xFC, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x80,
    0xFE, 0x00, 0x00, 0xFC,
    0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x7E, 0x00, 0x01, 0xFC,
    0x0F, 0xE0, 0x00, 0x00,
    0x00, 0x00, 0x0F, 0xC0, 0x7E, 0x00, 0x01, 0xF8, 0x0F, 0xC0, 0x00, 0x00,
    0x00, 0x00, 0x0F, 0xE0,
    0x7E, 0x00, 0x01, 0xF8, 0x1F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0,
    0x3F, 0x00, 0x01, 0xF0,
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x3F, 0x00, 0x03, 0xF0,
    0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x03, 0xF8, 0x1F, 0x00, 0x03, 0xE0, 0x7F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0xFC,
    0x1F, 0x80, 0x07, 0xE0, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE,
    0x0F, 0x80, 0x07, 0xC1,
    0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0xC0, 0x07, 0x83,
    0xFC, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7F, 0x83, 0xC0, 0x0F, 0x07, 0xF8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3F,
    0xC1, 0xE0, 0x0E, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
    0xF0, 0xE0, 0x1C, 0x1F,
    0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x70, 0x38, 0x7F,
    0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03, 0xFC, 0x38, 0x30, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x07, 0xF0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x3F, 0xC0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0xF8, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x08, 0x40, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3C, 0x00,
    0x00, 0x03, 0xF0, 0x0F, 0xC0, 0x0F, 0xC0, 0x0F, 0x80, 0x1F, 0xBC, 0x00,
    0x00, 0x0F, 0xF8, 0x3F,
    0xF0, 0x3F, 0xF0, 0x3F, 0xE0, 0x7F, 0xFC, 0x00, 0x00, 0x1F, 0xFC, 0x7F,
    0xF8, 0x7F, 0xF8, 0x7F,
    0xF8, 0xFF, 0xFC, 0x00, 0x00, 0x1F, 0x7E, 0xFE, 0xFC, 0xFD, 0xFC, 0xFD,
    0xFD, 0xFD, 0xFC, 0x00,
    0x00, 0x1E, 0x1E, 0xF0, 0x3F, 0xF0, 0x3D, 0xE0, 0x3D, 0xE0, 0x7C, 0x00,
    0x00, 0x1F, 0xF1, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xE0, 0x3C, 0x00, 0x00, 0x0F, 0xFD, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFD, 0xC0, 0x3C, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFD, 0xC0, 0x3C, 0x00,
    0x00, 0x3C, 0x1E, 0xF0, 0x31, 0xF0, 0x21, 0xE0, 0x21, 0xE0, 0x7C, 0x00,
    0x00, 0x3F, 0x3E, 0xFE,
    0xFC, 0xFC, 0xF8, 0xFD, 0xF9, 0xFD, 0xFC, 0x00, 0x00, 0x1F, 0xFE, 0x7F,
    0xFC, 0x7F, 0xF8, 0x7F,
    0xF8, 0xFF, 0xFC, 0x00, 0x00, 0x0F, 0xFC, 0x3F, 0xF0, 0x3F, 0xF0, 0x3F,
    0xE0, 0x7F, 0xFC, 0x00,
    0x00, 0x03, 0xF0, 0x0F, 0xC0, 0x0F, 0xC0, 0x0F, 0x80, 0x1F, 0xBC, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x64, 0xCD,
    0xF6, 0xE1, 0xC9, 0x3F,
    0x3F, 0xFB, 0xA6, 0x00, 0x00, 0x1F, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF, 0xBF,
    0xFF, 0xFF, 0xFE, 0x00,
    0x00, 0x1F, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF, 0xBF, 0xFF, 0xFF, 0xFC, 0x00,
    0x00, 0x1F, 0xFF, 0xED,
    0xFF, 0xFF, 0xFF, 0xB7, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0xC0, 0x00,
    0x20, 0x00, 0x01, 0x00,
    0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

const unsigned char simpleFont[][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00 },
    { 0x00, 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00, 0x00 },
    { 0x00, 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, 0x00 },
    { 0x00, 0x23, 0x13, 0x08, 0x64, 0x62, 0x00, 0x00 },
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50, 0x00, 0x00 },
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x08, 0x2A, 0x1C, 0x2A, 0x08, 0x00, 0x00 },
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00 },
    { 0x00, 0xA0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00 },
    { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00 },
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00 },
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00 },
    { 0x00, 0x62, 0x51, 0x49, 0x49, 0x46, 0x00, 0x00 },
    { 0x00, 0x22, 0x41, 0x49, 0x49, 0x36, 0x00, 0x00 },
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00 },
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00 },
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00 },
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00 },
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00 },
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00 },
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0xAC, 0x6C, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00 },
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00 },
    { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00 },
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06, 0x00, 0x00 },
    { 0x00, 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00, 0x00 },
    { 0x00, 0x7E, 0x09, 0x09, 0x09, 0x7E, 0x00, 0x00 },
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00 },
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x00 },
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00 },
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00 },
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00 },
    { 0x00, 0x3E, 0x41, 0x41, 0x51, 0x72, 0x00, 0x00 },
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00 },
    { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x00 },
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 },
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00 },
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, 0x00 },
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00, 0x00 },
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00 },
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00 },
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, 0x00 },
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00 },
    { 0x00, 0x26, 0x49, 0x49, 0x49, 0x32, 0x00, 0x00 },
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00 },
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00 },
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00 },
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, 0x00 },
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00 },
    { 0x00, 0x03, 0x04, 0x78, 0x04, 0x03, 0x00, 0x00 },
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00 },
    { 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00 },
    { 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00 },
    { 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00 },
    { 0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00 },
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00, 0x00 },
    { 0x00, 0x38, 0x44, 0x44, 0x28, 0x00, 0x00, 0x00 },
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00, 0x00 },
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00 },
    { 0x00, 0x08, 0x7E, 0x09, 0x02, 0x00, 0x00, 0x00 },
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C, 0x00, 0x00 },
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00 },
    { 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x80, 0x84, 0x7D, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00 },
    { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78, 0x00, 0x00 },
    { 0x00, 0x7C, 0x08, 0x04, 0x7C, 0x00, 0x00, 0x00 },
    { 0x00, 0x38, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00 },
    { 0x00, 0xFC, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00 },
    { 0x00, 0x18, 0x24, 0x24, 0xFC, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x7C, 0x08, 0x04, 0x00, 0x00, 0x00 },
    { 0x00, 0x48, 0x54, 0x54, 0x24, 0x00, 0x00, 0x00 },
    { 0x00, 0x04, 0x7F, 0x44, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x3C, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x00 },
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, 0x00 },
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, 0x00 },
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00 },
    { 0x00, 0x1C, 0xA0, 0xA0, 0x7C, 0x00, 0x00, 0x00 },
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00 },
    { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x00 },
    { 0x00, 0x02, 0x05, 0x05, 0x02, 0x00, 0x00, 0x00 }
};

void swap(uint16_t * a, uint16_t * b)
{
    uint16_t t = *a;
    *a = *b;
    *b = t;
};

uint16_t _width = RGB_OLED_WIDTH;
uint16_t _height = RGB_OLED_HEIGHT;

//#include "SGL.h"

#define pgm_read_byte *

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
void SGL(uint16_t width, uint16_t height)
{
    _width = width;
    _height = height;
}

void
drawLineSGL(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
	    uint16_t color)
{
    int x = x1 - x0;
    int y = y1 - y0;
    int dx = abs(x), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    for (;;) {
	drawPixel(x0, y0, color);
	e2 = 2 * err;
	if (e2 >= dy) {
	    if (x0 == x1)
		break;
	    err += dy;
	    x0 += sx;
	}
	if (e2 <= dx) {
	    if (y0 == y1)
		break;
	    err += dx;
	    y0 += sy;
	}
    }
}

void
drawVerticalLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color)
{
    uint16_t y1 = min(y + height, _height - 1);
    for (int16_t i = y; i < y1; i++) {
	drawPixel(x, i, color);
    }
}

void
drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
    uint16_t x1 = min(x + width, _width - 1);
    for (int16_t i = x; i < x1; i++) {
	drawPixel(i, y, color);
    }
}

void
drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
	      uint16_t color)
{
    drawHorizontalLine(x, y, width, color);
    drawHorizontalLine(x, y + height, width, color);
    drawVerticalLine(x, y, height, color);
    drawVerticalLine(x + width, y, height, color);
}

void
fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
	      uint16_t color)
{
    for (uint16_t i = 0; i < height; i++) {
	for (uint16_t j = 0; j < width; j++) {
	    drawPixel(x + j, y + i, color);
	}
    }
}

void drawCircle(uint16_t poX, uint16_t poY, uint16_t r, uint16_t color)
{
    int x = -r, y = 0, err = 2 - 2 * r, e2;
    do {
	drawPixel(poX - x, poY + y, color);
	drawPixel(poX + x, poY + y, color);
	drawPixel(poX + x, poY - y, color);
	drawPixel(poX - x, poY - y, color);
	e2 = err;
	if (e2 <= y) {
	    err += ++y * 2 + 1;
	    if (-x == y && e2 <= x)
		e2 = 0;
	}
	if (e2 > x)
	    err += ++x * 2 + 1;
    }
    while (x <= 0);
}

void fillCircle(uint16_t poX, uint16_t poY, uint16_t r, uint16_t color)
{
    int x = -r, y = 0, err = 2 - 2 * r, e2;
    do {
	drawVerticalLine(poX - x, poY - y, 2 * y, color);
	drawVerticalLine(poX + x, poY - y, 2 * y, color);
	e2 = err;
	if (e2 <= y) {
	    err += ++y * 2 + 1;
	    if (-x == y && e2 <= x)
		e2 = 0;
	}
	if (e2 > x)
	    err += ++x * 2 + 1;
    }
    while (x <= 0);
}

void
drawTraingle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
	     uint16_t x2, uint16_t y2, uint16_t color)
{
    drawLineSGL(x0, y0, x1, y1, color);
    drawLineSGL(x1, y1, x2, y2, color);
    drawLineSGL(x2, y2, x0, y0, color);
}

void
fillTraingle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
	     uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t a, b, y, last;

    if (y0 > y1) {
	swap(&y0, &y1);
	swap(&x0, &x1);
    }
    if (y1 > y2) {
	swap(&y2, &y1);
	swap(&x2, &x1);
    }
    if (y0 > y1) {
	swap(&y1, &y0);
	swap(&x1, &x0);
    }

    if (y0 == y2) {
	x0 = min(x0, x1) < x2 ? min(x0, x1) : x2;
	x2 = max(x0, x1) > x2 ? max(x0, x1) : x2;
	drawHorizontalLine(x0, y0, x2 - x0, color);
	return;
    }

    int16_t dx01 = x1 - x0, dy01 = y1 - y0,
	dx02 = x2 - x0, dy02 = y2 - y0, dx12 = x2 - x1, dy12 = y2 - y1;
    int16_t sa = 0, sb = 0;

    if (y1 == y2)
	last = y1;
    else
	last = y1 - 1;

    for (y = y0; y <= last; y++) {
	a = x0 + sa / dy01;
	b = x0 + sb / dy02;
	sa += dx01;
	sb += dx02;
	if (a > b)
	    swap(&a, &b);
	drawHorizontalLine(a, y, b - a + 1, color);
    }

    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++) {
	a = x1 + sa / dy12;
	b = x0 + sb / dy02;
	sa += dx12;
	sb += dx02;
	if (a > b)
	    swap(&a, &b);
	drawHorizontalLine(a, y, b - a + 1, color);
    }
}

void
drawChar(uint8_t ascii, uint16_t x, uint16_t y, uint16_t size,
	 uint16_t color)
{
    if ((ascii < 32) || (ascii >= 127)) {
	return;
    }

    for (int8_t i = 0; i < FONT_X; i++) {
	uint8_t temp = pgm_read_byte(&simpleFont[ascii - 0x20][i]);
	int8_t inrun = 0;
	int8_t runlen = 0;
	int8_t endrun = 0;

	for (int8_t f = 0; f < FONT_Y; f++) {
	    if ((temp >> f) & 0x01) {
		if (inrun)
		    runlen += 1;
		else {
		    inrun = 1;
		    runlen = 1;
		}
	    } else if (inrun) {
		endrun = 1;
		inrun = 0;
	    }

	    if (f == FONT_Y - 1 && inrun) {
		endrun = 1;
		// need the +1 b/c we this code is normally
		// only triggered  when f == FONT_Y, due to the
		// edge-triggered nature of this algorithm
		f += 1;
	    }

	    if (endrun) {
		fillRectangle(x + i * size, y + (f - runlen) * size, size,
			      runlen * size, color);
		inrun = 0;
		runlen = 0;
		endrun = 0;
	    }
	}
    }
}

void
drawString(char *string, uint16_t x, uint16_t y, uint16_t size,
	   uint16_t color)
{
    while (*string) {
	drawChar(*string, x, y, size, color);
	*string++;
	x += FONT_SPACE * size;
	if (x >= _width - 1) {
	    y += FONT_Y * size;
	    x = 0;
	}
    }
}

void
drawBitMap(uint16_t x, uint16_t y, const uint8_t * bitmap, uint16_t width,
	   int16_t height, uint16_t color)
{
    uint16_t i, j, byteWidth = (width + 7) / 8;
    for (j = 0; j < height; j++) {
	for (i = 0; i < width; i++) {
	    if (pgm_read_byte(bitmap + j * byteWidth + i / 8) &
		(128 >> (i & 7))) {
		drawPixel(x + i, y + j, color);
	    }
	}
    }
}

void fillScreen(uint16_t color)
{
    fillRectangle(0, 0, _width, _height, color);
}

void _sendCmd(uint8_t c)
{
    //digitalWrite(_dc,LOW);
    //digitalWrite(_cs,LOW);
    // SPI.transfer(c);
    *((volatile uint32_t *) VIDEO_RAW) = ((0x00) << 8) | c;
    //digitalWrite(_cs,HIGH);
}

void _sendData(uint8_t c)
{
    //digitalWrite(_dc,LOW);
    //digitalWrite(_cs,LOW);
    // SPI.transfer(c);
    *((volatile uint32_t *) VIDEO_RAW) = ((0x01) << 8) | c;
    //digitalWrite(_cs,HIGH);
}

void init(void)
{
    //pinMode(_dc, OUTPUT);
    //pinMode(_cs, OUTPUT);

    //SPI.begin();

    _sendCmd(CMD_DISPLAY_OFF);	//Display Off
    _sendCmd(CMD_SET_CONTRAST_A);	//Set contrast for color A
    _sendCmd(0x91);		//145
    _sendCmd(CMD_SET_CONTRAST_B);	//Set contrast for color B
    _sendCmd(0x50);		//80
    _sendCmd(CMD_SET_CONTRAST_C);	//Set contrast for color C
    _sendCmd(0x7D);		//125
    _sendCmd(CMD_MASTER_CURRENT_CONTROL);	//master current control
    _sendCmd(0x06);		//6
    _sendCmd(CMD_SET_PRECHARGE_SPEED_A);	//Set Second Pre-change Speed For ColorA
    _sendCmd(0x64);		//100
    _sendCmd(CMD_SET_PRECHARGE_SPEED_B);	//Set Second Pre-change Speed For ColorB
    _sendCmd(0x78);		//120
    _sendCmd(CMD_SET_PRECHARGE_SPEED_C);	//Set Second Pre-change Speed For ColorC
    _sendCmd(0x64);		//100
    _sendCmd(CMD_SET_REMAP);	//set remap & data format
    _sendCmd(0x72);		//0x72              
    _sendCmd(CMD_SET_DISPLAY_START_LINE);	//Set display Start Line
    _sendCmd(0x0);
    _sendCmd(CMD_SET_DISPLAY_OFFSET);	//Set display offset
    _sendCmd(0x0);
    _sendCmd(CMD_NORMAL_DISPLAY);	//Set display mode
    _sendCmd(CMD_SET_MULTIPLEX_RATIO);	//Set multiplex ratio
    _sendCmd(0x3F);
    _sendCmd(CMD_SET_MASTER_CONFIGURE);	//Set master configuration
    _sendCmd(0x8E);
    _sendCmd(CMD_POWER_SAVE_MODE);	//Set Power Save Mode
    _sendCmd(0x00);		//0x00
    _sendCmd(CMD_PHASE_PERIOD_ADJUSTMENT);	//phase 1 and 2 period adjustment
    _sendCmd(0x31);		//0x31
    _sendCmd(CMD_DISPLAY_CLOCK_DIV);	//display clock divider/oscillator frequency
    _sendCmd(0xF0);
    _sendCmd(CMD_SET_PRECHARGE_VOLTAGE);	//Set Pre-Change Level
    _sendCmd(0x3A);
    _sendCmd(CMD_SET_V_VOLTAGE);	//Set vcomH
    _sendCmd(0x3E);
    _sendCmd(CMD_DEACTIVE_SCROLLING);	//disable scrolling
    _sendCmd(CMD_NORMAL_BRIGHTNESS_DISPLAY_ON);	//set display on
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x < 0) || (x >= RGB_OLED_WIDTH) || (y < 0)
	|| (y >= RGB_OLED_HEIGHT))
	return;
    //set column point
    _sendCmd(CMD_SET_COLUMN_ADDRESS);
    _sendCmd(x);
    _sendCmd(RGB_OLED_WIDTH - 1);
    //set row point
    _sendCmd(CMD_SET_ROW_ADDRESS);
    _sendCmd(y);
    _sendCmd(RGB_OLED_HEIGHT - 1);
    //fill 16bit colour
    //digitalWrite(_dc,HIGH);
    //digitalWrite(_cs,LOW);
    //    SPI.transfer(color >> 8);
    //   SPI.transfer(color);
    _sendData(color >> 8);
    _sendData(color);
    //digitalWrite(_cs,HIGH);
}

void
drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
	 uint16_t color)
{
    if ((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0))
	return;

    if (x0 >= RGB_OLED_WIDTH)
	x0 = RGB_OLED_WIDTH - 1;
    if (y0 >= RGB_OLED_HEIGHT)
	y0 = RGB_OLED_HEIGHT - 1;
    if (x1 >= RGB_OLED_WIDTH)
	x1 = RGB_OLED_WIDTH - 1;
    if (y1 >= RGB_OLED_HEIGHT)
	y1 = RGB_OLED_HEIGHT - 1;

    _sendCmd(CMD_DRAW_LINE);	//draw line
    _sendCmd(x0);		//start column
    _sendCmd(y0);		//start row
    _sendCmd(x1);		//end column
    _sendCmd(y1);		//end row
    _sendCmd((uint8_t) ((color >> 11) & 0x1F));	//R
    _sendCmd((uint8_t) ((color >> 5) & 0x3F));	//G
    _sendCmd((uint8_t) (color & 0x1F));	//B
}

void
drawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
	  uint16_t outColor, uint16_t fillColor)
{
    if ((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0))
	return;

    if (x0 >= RGB_OLED_WIDTH)
	x0 = RGB_OLED_WIDTH - 1;
    if (y0 >= RGB_OLED_HEIGHT)
	y0 = RGB_OLED_HEIGHT - 1;
    if (x1 >= RGB_OLED_WIDTH)
	x1 = RGB_OLED_WIDTH - 1;
    if (y1 >= RGB_OLED_HEIGHT)
	y1 = RGB_OLED_HEIGHT - 1;

    _sendCmd(CMD_FILL_WINDOW);	//fill window
    _sendCmd(ENABLE_FILL);
    _sendCmd(CMD_DRAW_RECTANGLE);	//draw rectangle
    _sendCmd(x0);		//start column
    _sendCmd(y0);		//start row
    _sendCmd(x1);		//end column
    _sendCmd(y1);		//end row
    _sendCmd((uint8_t) ((outColor >> 11) & 0x1F));	//R
    _sendCmd((uint8_t) ((outColor >> 5) & 0x3F));	//G
    _sendCmd((uint8_t) (outColor & 0x1F));	//B
    _sendCmd((uint8_t) ((fillColor >> 11) & 0x1F));	//R
    _sendCmd((uint8_t) ((fillColor >> 5) & 0x3F));	//G
    _sendCmd((uint8_t) (fillColor & 0x1F));	//B
}

void
copyWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2,
	   uint16_t y2)
{
    _sendCmd(CMD_COPY_WINDOW);	//copy window
    _sendCmd(x0);		//start column
    _sendCmd(y0);		//start row
    _sendCmd(x1);		//end column
    _sendCmd(y1);		//end row
    _sendCmd(x2);		//new column
    _sendCmd(y2);		//new row
}

void dimWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _sendCmd(CMD_DIM_WINDOW);	//copy area
    _sendCmd(x0);		//start column
    _sendCmd(y0);		//start row
    _sendCmd(x1);		//end column
    _sendCmd(y1);		//end row
}

void clearWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _sendCmd(CMD_CLEAR_WINDOW);	//clear window
    _sendCmd(x0);		//start column
    _sendCmd(y0);		//start row
    _sendCmd(x1);		//end column
    _sendCmd(y1);		//end row
}

void
setScolling(ScollingDirection direction, uint8_t rowAddr, uint8_t rowNum,
	    uint8_t timeInterval)
{
    uint8_t scolling_horizontal = 0x0;
    uint8_t scolling_vertical = 0x0;
    switch (direction) {
    case Horizontal:
	scolling_horizontal = 0x01;
	scolling_vertical = 0x00;
	break;
    case Vertical:
	scolling_horizontal = 0x00;
	scolling_vertical = 0x01;
	break;
    case Diagonal:
	scolling_horizontal = 0x01;
	scolling_vertical = 0x01;
	break;
    default:
	break;
    }
    _sendCmd(CMD_CONTINUOUS_SCROLLING_SETUP);
    _sendCmd(scolling_horizontal);
    _sendCmd(rowAddr);
    _sendCmd(rowNum);
    _sendCmd(scolling_vertical);
    _sendCmd(timeInterval);
    _sendCmd(CMD_ACTIVE_SCROLLING);
}

void enableScolling(int enable)
{
    if (enable)
	_sendCmd(CMD_ACTIVE_SCROLLING);
    else
	_sendCmd(CMD_DEACTIVE_SCROLLING);
}

void setDisplayMode(DisplayMode mode)
{
    _sendCmd(mode);
}

void setDisplayPower(DisplayPower power)
{
    _sendCmd(power);
}

#define delay wait_cycles
#define cs     5
#define dc     3
#define mosi   16
#define sclk   15

void setup()
{
    init();
    fillScreen(COLOR_BLACK);
}

void main()
{
    for (;;) {
        print_str_ln("run...");
        setup();
        delay(450000);
        drawString("Seeed", 15, 25, 2, COLOR_GREEN);
        delay(450000);
           // setScolling(Horizontal, 0, 64, 1);
            setScolling(Horizontal, 0, 64, 1);
            delay(45000000);
            delay(45000000);
        setup();
            drawBitMap(0, 0, SeeedLogo, 96, 64, COLOR_YELLOW);
            setScolling(Vertical, 0, 64, 0);
            delay(45000000);
            delay(45000000);
        setup();

        for (int i = 30; i > 0; i--) {
            drawCircle(48, 32, i, COLOR_CYAN);
            delay(450000);
        }
        for (int i = 1; i <= 30; i++) {
            drawCircle(48, 32, i, COLOR_RED);
            delay(450000);
        }
        for (int i = 30; i > 0; i--) {
            drawCircle(48, 32, i, COLOR_PURPLE);
            delay(450000);
        }
        for (int i = 1; i <= 30; i++) {
            drawCircle(48, 32, i, COLOR_GOLDEN);
            delay(450000);
        }
        setup();
        //nothing to do
        for (int i = 0; i < 100; i++) {
            uint8_t x0 = rand() % 96;
            uint8_t y0 = rand() % 64;
            uint8_t x1 = rand() % 96;
            uint8_t y1 = rand() % 64;
            uint8_t x2 = rand() % 96;
            uint8_t y2 = rand() % 64;
            drawTraingle(x0, y0, x1, y1, x2, y2, rand() % 65536);
            //  delay(300);
            delay(450000);
        }
        setup();
        for (int i = 0; i < 5; i++) {
            for (int i = 1; i < 16; i++) {
          drawRectangle(3 * i, 2 * i, 95 - 6 * i, 63 - 4 * i,
                  COLOR_YELLOW);
          delay(450000);
            }
            delay(450000);
            for (int i = 15; i > 0; i--) {
          drawRectangle(3 * i, 2 * i, 95 - 6 * i, 63 - 4 * i,
                  COLOR_BLUE);
          delay(450000);
            }
            delay(200000);
        }

        setup();
        delay(2000);
            drawLine(5, 7, 65, 48, COLOR_BLUE);
            delay(4500000);
            drawLine(5, 40, 48, 7, COLOR_RED);
            delay(4500000);
            drawLine(65, 5, 47, 50, COLOR_GREEN);
            delay(4500000);
            drawLine(3, 10, 80, 21, COLOR_YELLOW);
        delay(45000000);
    }

}