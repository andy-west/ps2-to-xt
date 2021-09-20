// An Arduino-based PS/2 to PC/XT keyboard adapter

// Originally based on pcxtkbd:
// https://github.com/kesrut/pcxtkbd

// This program was created for and tested on a Tandy 1000 TL/2, but it should
// work on any system that takes an IBM PC/XT keyboard, at least in theory.

#include <PS2KeyAdvanced.h>

#define ps2_clk 3  // PS/2 clock pin
#define ps2_data 4  // PS/2 data pin

#define xt_clk 2  // XT clock pin
#define xt_data 5  // XT data pin

PS2KeyAdvanced keyboard;

void setup()
{
  keyboard.begin(ps2_data, ps2_clk);

  digitalWrite(xt_clk, HIGH);
  pinMode(xt_clk, OUTPUT);

  digitalWrite(xt_data, HIGH);
  pinMode(xt_data, OUTPUT); 

  report_successful_self_test();
}

void report_successful_self_test()
{
  do
  {
    write1(0xAA);
    delay(10);
  } while (digitalRead(xt_clk) == LOW);
}

void write1(unsigned char value)
{
  write1(value, false);
}

void write2(unsigned char value1, unsigned char value2)
{
  write2(value1, value2, false);
}

void write2(unsigned char value1, unsigned char value2, bool is_brk)
{
  write1(value1, false);
  write1(value2, is_brk);
}

void write3(unsigned char value1, unsigned char value2, unsigned char value3)
{
  write3(value1, value2, value3, false);
}

void write3(unsigned char value1, unsigned char value2, unsigned char value3, bool is_brk)
{
  write1(value1, false);
  write1(value2, is_brk);
  write1(value3, is_brk);
}

void write4(unsigned char value1, unsigned char value2, unsigned char value3, unsigned char value4)
{
  write1(value1, false);
  write1(value2, false);
  write1(value3, false);
  write1(value4, false);
}

void write1(unsigned char value, bool is_brk)
{
  if (is_brk) value |= 0x80;
  
  while (digitalRead(xt_clk) != HIGH);

  int i;
  unsigned char bits[8] ;
 
  for (i = 0 ; i < 8; i++)
  {
    bits[i] = value & 1;
    value = value >> 1 ;
  }
  
  digitalWrite(xt_clk, LOW);
  digitalWrite(xt_data, HIGH);
  delayMicroseconds(120);
  digitalWrite(xt_clk, HIGH);
  delayMicroseconds(66);
  digitalWrite(xt_clk, LOW);
  delayMicroseconds(30);
  digitalWrite(xt_clk, HIGH);

  for (i = 0; i < 8; i++)
  {
    digitalWrite(xt_clk, HIGH);
    digitalWrite(xt_data, bits[i]);
    delayMicroseconds(95);
    digitalWrite(xt_clk, LOW);
    digitalWrite(xt_data, LOW);
  }
  
  digitalWrite(xt_clk, HIGH);
  digitalWrite(xt_data, LOW);
  delay(1);
}

void loop()
{
  uint16_t c;
  uint8_t code;
  bool is_brk;
  bool is_ctrl;
  bool is_alt;
  bool is_shift;
  
  if (keyboard.available())
  {
    c = keyboard.read();
    code = c & 0xFF;

    is_brk = c & PS2_BREAK;
    is_ctrl = c & PS2_CTRL;
    is_alt = c & PS2_ALT;
    is_shift = c & PS2_SHIFT;

    switch (code)
    {
      // Row 1
      case PS2_KEY_ESC: write1(0x01, is_brk); break;
      case PS2_KEY_F1: write1(0x3B, is_brk); break;
      case PS2_KEY_F2: write1(0x3C, is_brk); break;
      case PS2_KEY_F3: write1(0x3D, is_brk); break;
      case PS2_KEY_F4: write1(0x3E, is_brk); break;
      case PS2_KEY_F5: write1(0x3F, is_brk); break;
      case PS2_KEY_F6: write1(0x40, is_brk); break;
      case PS2_KEY_F7: write1(0x41, is_brk); break;
      case PS2_KEY_F8: write1(0x42, is_brk); break;
      case PS2_KEY_F9: write1(0x43, is_brk); break;
      case PS2_KEY_F10: write1(0x44, is_brk); break;
      case PS2_KEY_F11: write1(0x57, is_brk); break;
      case PS2_KEY_F12: write1(0x58, is_brk); break;
      case PS2_KEY_PRTSCR:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x37);
        else write4(0xE0, 0xB7, 0xE0, 0xAA);
        break;
      case PS2_KEY_SCROLL: write2(0x46, 0x46, true); break;
      case PS2_KEY_BREAK:
          write3(0xE1, 0x1D, 0x45);
          write3(0xE1, 0x1D, 0x45, true);
        break;

      // Row 2
      case PS2_KEY_SINGLE: write1(0x29, is_brk); break;
      case PS2_KEY_1: write1(0x02, is_brk); break;
      case PS2_KEY_2: write1(0x03, is_brk); break;
      case PS2_KEY_3: write1(0x04, is_brk); break;
      case PS2_KEY_4: write1(0x05, is_brk); break;
      case PS2_KEY_5: write1(0x06, is_brk); break;
      case PS2_KEY_6: write1(0x07, is_brk); break;
      case PS2_KEY_7: write1(0x08, is_brk); break;
      case PS2_KEY_8: write1(0x09, is_brk); break;
      case PS2_KEY_9: write1(0x0A, is_brk); break;
      case PS2_KEY_0: write1(0x0B, is_brk); break;
      case PS2_KEY_MINUS: write1(0x0C, is_brk); break;
      case PS2_KEY_EQUAL: write1(0x0D, is_brk); break;
      case PS2_KEY_BS: write1(0x0E, is_brk); break;
      case PS2_KEY_INSERT:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x52);
        else write4(0xE0, 0xD2, 0xE0, 0xAA);
        break;
      case PS2_KEY_HOME:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x47);
        else write4(0xE0, 0xC7, 0xE0, 0xAA);
        break;
      case PS2_KEY_PGUP:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x49);
        else write4(0xE0, 0xC9, 0xE0, 0xAA);
        break;
      case PS2_KEY_NUM: write2(0x45, 0x45, true); break;
      case PS2_KEY_KP_DIV: write2(0xE0, 0x35, is_brk); break;
      case PS2_KEY_KP_TIMES: write1(0x37, is_brk); break;
      case PS2_KEY_KP_MINUS: write1(0x4A, is_brk); break;

      // Row 3
      case PS2_KEY_TAB: write1(0x0F, is_brk); break;
      case PS2_KEY_Q: write1(0x10, is_brk); break;
      case PS2_KEY_W: write1(0x11, is_brk); break;
      case PS2_KEY_E: write1(0x12, is_brk); break;
      case PS2_KEY_R: write1(0x13, is_brk); break;
      case PS2_KEY_T: write1(0x14, is_brk); break;
      case PS2_KEY_Y: write1(0x15, is_brk); break;
      case PS2_KEY_U: write1(0x16, is_brk); break;
      case PS2_KEY_I: write1(0x17, is_brk); break;
      case PS2_KEY_O: write1(0x18, is_brk); break;
      case PS2_KEY_P: write1(0x19, is_brk); break;
      case PS2_KEY_OPEN_SQ: write1(0x1A, is_brk); break;
      case PS2_KEY_CLOSE_SQ: write1(0x1B, is_brk); break;
      case PS2_KEY_BACK: write1(0x2B, is_brk); break;
      case PS2_KEY_DELETE:
        // Handle the three-finger salute.
        if (!is_brk && is_ctrl && is_alt)
        {
          keyboard.resetKey();
          report_successful_self_test();
        }
        else
        {
          if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x53);
          else write4(0xE0, 0xD3, 0xE0, 0xAA);
        }
        break;
      case PS2_KEY_END:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x4F);
        else write4(0xE0, 0xCF, 0xE0, 0xAA);
        break;
      case PS2_KEY_PGDN:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x51);
        else write4(0xE0, 0xD1, 0xE0, 0xAA);
        break;
      case PS2_KEY_KP7: write1(0x47, is_brk); break;
      case PS2_KEY_KP8: write1(0x48, is_brk); break;
      case PS2_KEY_KP9: write1(0x49, is_brk); break;
      case PS2_KEY_KP_PLUS: write1(0x4E, is_brk); break;

      // Row 4
      case PS2_KEY_CAPS: write2(0x3A, 0x3A, true); break;
      case PS2_KEY_A: write1(0x1E, is_brk); break;
      case PS2_KEY_S: write1(0x1F, is_brk); break;
      case PS2_KEY_D: write1(0x20, is_brk); break;
      case PS2_KEY_F: write1(0x21, is_brk); break;
      case PS2_KEY_G: write1(0x22, is_brk); break;
      case PS2_KEY_H: write1(0x23, is_brk); break;
      case PS2_KEY_J: write1(0x24, is_brk); break;
      case PS2_KEY_K: write1(0x25, is_brk); break;
      case PS2_KEY_L: write1(0x26, is_brk); break;
      case PS2_KEY_SEMI: write1(0x27, is_brk); break;
      case PS2_KEY_APOS: write1(0x28, is_brk); break;
      case PS2_KEY_ENTER: write1(0x1C, is_brk); break;
      case PS2_KEY_KP4: write1(0x4B, is_brk); break;
      case PS2_KEY_KP5: write1(0x4C, is_brk); break;
      case PS2_KEY_KP6: write1(0x4D, is_brk); break;

      // Row 5
      case PS2_KEY_L_SHIFT:
        if (!is_brk)
        {
          // Unmodified pause key appears as left shift, so check if this really is the shift key.
          if (is_shift) write1(0x2A);
          else
          {
            // Pause
            write3(0xE1, 0x1D, 0x45);
            write3(0xE1, 0x1D, 0x45, true);
          }
        }
        else write1(0x2A, true);
        break;
      case PS2_KEY_Z: write1(0x2C, is_brk); break;
      case PS2_KEY_X: write1(0x2D, is_brk); break;
      case PS2_KEY_C: write1(0x2E, is_brk); break;
      case PS2_KEY_V: write1(0x2F, is_brk); break;
      case PS2_KEY_B: write1(0x30, is_brk); break;
      case PS2_KEY_N: write1(0x31, is_brk); break;
      case PS2_KEY_M: write1(0x32, is_brk); break;
      case PS2_KEY_COMMA: write1(0x33, is_brk); break;
      case PS2_KEY_DOT: write1(0x34, is_brk); break;
      case PS2_KEY_DIV: write1(0x35, is_brk); break;
      case PS2_KEY_R_SHIFT: write1(0x36, is_brk); break;
      case PS2_KEY_UP_ARROW:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x48);
        else write4(0xE0, 0xC8, 0xE0, 0xAA);
        break;
      case PS2_KEY_KP1: write1(0x4F, is_brk); break;
      case PS2_KEY_KP2: write1(0x50, is_brk); break;
      case PS2_KEY_KP3: write1(0x51, is_brk); break;

      // Row 6
      case PS2_KEY_L_CTRL: write1(0x1D, is_brk); break;
      case PS2_KEY_L_ALT: write1(0x38, is_brk); break;
      case PS2_KEY_SPACE: write1(0x39, is_brk); break;
      case PS2_KEY_R_ALT: write2(0xE0, 0x38, is_brk); break;
      case PS2_KEY_R_CTRL: write2(0xE0, 0x1D, is_brk); break;
      case PS2_KEY_L_ARROW:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x4B);
        else write4(0xE0, 0xCB, 0xE0, 0xAA);
        break;
      case PS2_KEY_DN_ARROW:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x50);
        else write4(0xE0, 0xD0, 0xE0, 0xAA);
        break;
      case PS2_KEY_R_ARROW:
        if (!is_brk) write4(0xE0, 0x2A, 0xE0, 0x4D);
        else write4(0xE0, 0xCD, 0xE0, 0xAA);
        break;
      case PS2_KEY_KP0: write1(0x52, is_brk); break;
      case PS2_KEY_KP_DOT: write1(0x53, is_brk); break;
      case PS2_KEY_KP_ENTER: write2(0xE0, 0x1C, is_brk); break;
    }
  }
}
