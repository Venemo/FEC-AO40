/* 
 * Reference encoder for proposed coded AO-40 telemetry format - v1.0  7 Jan 2002
 * Copyright 2002, Phil Karn, KA9Q
 * This software may be used under the terms of the GNU Public License (GPL)
 */

/*
 * Revised and modified by szabolor
 * 2015
 */

#include <stdint.h>
#include "enc_ref.h"

#if (LOW_MEMORY == 0)

#else
#endif

#define SYNC_POLY      0x48
#define SCRAMBLER_POLY 0x95
#define CPOLYA         0x4f // 79
#define CPOLYB         0x6d // 109
#define GF_POLY        0x187
#define A0             255

const uint8_t RS_poly[] = {249,59,66,4,43,126,251,97,30,3,213,50,66,170,5,24};
uint8_t RS_block[2][32];
static uint16_t Nbytes;
static uint8_t Bmask;
static uint16_t Bindex;
static uint8_t Scrambler;
static uint8_t Conv_sr;
uint8_t *Interleaver;

// memory workaround: to LUT or not to LUT...
#if (LOW_MEMORY == 0)
  // No need for low memory, use LUT
  #define INDEX_OF(x) ( Index_of[ (x) ] )
  #define ALPHA_TO(x) ( Alpha_to[ (x) ] )
  static const uint8_t Alpha_to[] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x87, 0x89, 0x95, 0xad, 0xdd, 0x3d, 0x7a, 0xf4,
    0x6f, 0xde, 0x3b, 0x76, 0xec, 0x5f, 0xbe, 0xfb, 0x71, 0xe2, 0x43, 0x86, 0x8b, 0x91, 0xa5, 0xcd,
    0x1d, 0x3a, 0x74, 0xe8, 0x57, 0xae, 0xdb, 0x31, 0x62, 0xc4, 0x0f, 0x1e, 0x3c, 0x78, 0xf0, 0x67,
    0xce, 0x1b, 0x36, 0x6c, 0xd8, 0x37, 0x6e, 0xdc, 0x3f, 0x7e, 0xfc, 0x7f, 0xfe, 0x7b, 0xf6, 0x6b,
    0xd6, 0x2b, 0x56, 0xac, 0xdf, 0x39, 0x72, 0xe4, 0x4f, 0x9e, 0xbb, 0xf1, 0x65, 0xca, 0x13, 0x26,
    0x4c, 0x98, 0xb7, 0xe9, 0x55, 0xaa, 0xd3, 0x21, 0x42, 0x84, 0x8f, 0x99, 0xb5, 0xed, 0x5d, 0xba,
    0xf3, 0x61, 0xc2, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x07, 0x0e, 0x1c, 0x38, 0x70, 0xe0,
    0x47, 0x8e, 0x9b, 0xb1, 0xe5, 0x4d, 0x9a, 0xb3, 0xe1, 0x45, 0x8a, 0x93, 0xa1, 0xc5, 0x0d, 0x1a,
    0x34, 0x68, 0xd0, 0x27, 0x4e, 0x9c, 0xbf, 0xf9, 0x75, 0xea, 0x53, 0xa6, 0xcb, 0x11, 0x22, 0x44,
    0x88, 0x97, 0xa9, 0xd5, 0x2d, 0x5a, 0xb4, 0xef, 0x59, 0xb2, 0xe3, 0x41, 0x82, 0x83, 0x81, 0x85,
    0x8d, 0x9d, 0xbd, 0xfd, 0x7d, 0xfa, 0x73, 0xe6, 0x4b, 0x96, 0xab, 0xd1, 0x25, 0x4a, 0x94, 0xaf,
    0xd9, 0x35, 0x6a, 0xd4, 0x2f, 0x5e, 0xbc, 0xff, 0x79, 0xf2, 0x63, 0xc6, 0x0b, 0x16, 0x2c, 0x58,
    0xb0, 0xe7, 0x49, 0x92, 0xa3, 0xc1, 0x05, 0x0a, 0x14, 0x28, 0x50, 0xa0, 0xc7, 0x09, 0x12, 0x24,
    0x48, 0x90, 0xa7, 0xc9, 0x15, 0x2a, 0x54, 0xa8, 0xd7, 0x29, 0x52, 0xa4, 0xcf, 0x19, 0x32, 0x64,
    0xc8, 0x17, 0x2e, 0x5c, 0xb8, 0xf7, 0x69, 0xd2, 0x23, 0x46, 0x8c, 0x9f, 0xb9, 0xf5, 0x6d, 0xda,
    0x33, 0x66, 0xcc, 0x1f, 0x3e, 0x7c, 0xf8, 0x77, 0xee, 0x5b, 0xb6, 0xeb, 0x51, 0xa2, 0xc3, 0x00,
  };

  static const uint8_t Index_of[] = {
   0xff, 0x00, 0x01, 0x63, 0x02, 0xc6, 0x64, 0x6a, 0x03, 0xcd, 0xc7, 0xbc, 0x65, 0x7e, 0x6b, 0x2a,
   0x04, 0x8d, 0xce, 0x4e, 0xc8, 0xd4, 0xbd, 0xe1, 0x66, 0xdd, 0x7f, 0x31, 0x6c, 0x20, 0x2b, 0xf3,
   0x05, 0x57, 0x8e, 0xe8, 0xcf, 0xac, 0x4f, 0x83, 0xc9, 0xd9, 0xd5, 0x41, 0xbe, 0x94, 0xe2, 0xb4,
   0x67, 0x27, 0xde, 0xf0, 0x80, 0xb1, 0x32, 0x35, 0x6d, 0x45, 0x21, 0x12, 0x2c, 0x0d, 0xf4, 0x38,
   0x06, 0x9b, 0x58, 0x1a, 0x8f, 0x79, 0xe9, 0x70, 0xd0, 0xc2, 0xad, 0xa8, 0x50, 0x75, 0x84, 0x48,
   0xca, 0xfc, 0xda, 0x8a, 0xd6, 0x54, 0x42, 0x24, 0xbf, 0x98, 0x95, 0xf9, 0xe3, 0x5e, 0xb5, 0x15,
   0x68, 0x61, 0x28, 0xba, 0xdf, 0x4c, 0xf1, 0x2f, 0x81, 0xe6, 0xb2, 0x3f, 0x33, 0xee, 0x36, 0x10,
   0x6e, 0x18, 0x46, 0xa6, 0x22, 0x88, 0x13, 0xf7, 0x2d, 0xb8, 0x0e, 0x3d, 0xf5, 0xa4, 0x39, 0x3b,
   0x07, 0x9e, 0x9c, 0x9d, 0x59, 0x9f, 0x1b, 0x08, 0x90, 0x09, 0x7a, 0x1c, 0xea, 0xa0, 0x71, 0x5a,
   0xd1, 0x1d, 0xc3, 0x7b, 0xae, 0x0a, 0xa9, 0x91, 0x51, 0x5b, 0x76, 0x72, 0x85, 0xa1, 0x49, 0xeb,
   0xcb, 0x7c, 0xfd, 0xc4, 0xdb, 0x1e, 0x8b, 0xd2, 0xd7, 0x92, 0x55, 0xaa, 0x43, 0x0b, 0x25, 0xaf,
   0xc0, 0x73, 0x99, 0x77, 0x96, 0x5c, 0xfa, 0x52, 0xe4, 0xec, 0x5f, 0x4a, 0xb6, 0xa2, 0x16, 0x86,
   0x69, 0xc5, 0x62, 0xfe, 0x29, 0x7d, 0xbb, 0xcc, 0xe0, 0xd3, 0x4d, 0x8c, 0xf2, 0x1f, 0x30, 0xdc,
   0x82, 0xab, 0xe7, 0x56, 0xb3, 0x93, 0x40, 0xd8, 0x34, 0xb0, 0xef, 0x26, 0x37, 0x0c, 0x11, 0x44,
   0x6f, 0x78, 0x19, 0x9a, 0x47, 0x74, 0xa7, 0xc1, 0x23, 0x53, 0x89, 0xfb, 0x14, 0x5d, 0xf8, 0x97,
   0x2e, 0x4b, 0xb9, 0x60, 0x0f, 0xed, 0x3e, 0xe5, 0xf6, 0x87, 0xa5, 0x17, 0x3a, 0xa3, 0x3c, 0xb7,
  };
#else
  // Keep memory footprint low
  #define INDEX_OF(x) ( index_of_func( (x) ) )
  #define ALPHA_TO(x) ( alpha_to_func( (x) ) )
  static inline uint8_t index_of_func(uint8_t x) {
    uint16_t sr = 1;
    uint8_t i = 0;

    if (x == 0)
      return A0;

    while (sr != x) {
      sr <<= 1;
      if (sr & 0x100)
        sr ^= GF_POLY;
      sr &= 0xff;
      ++i;
    }

    return i;
  }

  static inline uint8_t alpha_to_func(uint8_t x) {
    uint16_t sr = 1;
    uint8_t i;

    if (x == A0)
      return 0;

    for (i = 0; i < x; ++i) {
      sr <<= 1;
      if (sr & 0x100)
        sr ^= GF_POLY;
      sr &= 0xff;
    }

    return sr;
  }

#endif /* LOW_MEMORY */

static inline uint8_t mod255(uint16_t x){
  while (x >= 255)
    x -= 255;
  return x;
}

static inline uint8_t parity(uint8_t x){
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return x & 1;
}

static void interleave_symbol(uint8_t c){
  if (c)
    Interleaver[Bindex] |= Bmask;
  else
    Interleaver[Bindex] &= ~Bmask;

  Bindex += 10;
  if (Bindex >= 650){
    Bindex -= 650;
    Bmask >>= 1;
    if (Bmask == 0){
      Bmask = 0x80;
      ++Bindex;
    }
  }
}

static void encode_and_interleave(uint8_t c, uint8_t cnt){
  while(cnt-- != 0){
    Conv_sr = (Conv_sr << 1) | (c >> 7);
    c <<= 1;
    interleave_symbol(parity(Conv_sr & CPOLYA));
    interleave_symbol(!parity(Conv_sr & CPOLYB)); /* Second encoder symbol is inverted */
  }    
}

static void scramble_and_encode(uint8_t c){
  uint8_t i;

  c ^= Scrambler;
  for (i = 0; i < 8; ++i)
    Scrambler = (Scrambler << 1) | parity(Scrambler & SCRAMBLER_POLY);
  encode_and_interleave(c, 8);
}

void reset_encoder(void){
  uint8_t i;

  Nbytes = 0;
  Conv_sr = 0;
  Scrambler = 0xff;
  Bmask = 0x40;
  Bindex = 0;
  for(i=0;i<32;i++) {
    RS_block[0][i] = 0;
    RS_block[1][i] = 0;
  }
}

void init_encoder(void){
  uint8_t i;
  uint16_t sr;

  for (sr = 0; sr < 650; ++sr)   // sr is used because its 16bit width
    Interleaver[sr] = 0;         // to preserve memory
  
  // TODO: write a little script for this to get the output
  sr = 0x7f;
  for (i = 0; i < 65; ++i) {
    if(sr & 64) { // TODO: 0x40
      Interleaver[10*i] |= 0x80;
    }
    sr = (sr << 1) | parity(sr & SYNC_POLY);
  }
  reset_encoder();
}

void encode_byte(uint8_t c){
  uint8_t *rp;
  uint8_t i;
  uint8_t feedback;
  uint8_t t;

  rp = RS_block[Nbytes & 1];
  feedback = INDEX_OF(c ^ rp[0]);
  
  if (feedback != A0){
    for (i = 0; i < 15; ++i) {
      t = ALPHA_TO(mod255(feedback + RS_poly[i]));
      rp[i+1] ^= t;
      rp[31-i] ^= t;
    }
    rp[16] ^= ALPHA_TO(mod255(feedback + RS_poly[15]));
  }
  
  for (i = 0; i < 31; ++i)
    rp[i] = rp[i+1];

  if (feedback != A0){
    rp[31] = ALPHA_TO(feedback);
  } else {
    rp[31] = 0;
  }
  scramble_and_encode(c);
  ++Nbytes;
}  

void encode_parity(void) {
  uint8_t c;

  c =  RS_block[Nbytes & 1][(Nbytes >> 1) - 128];
  scramble_and_encode(c);
  if (++Nbytes == 320) {
    encode_and_interleave(0, 6);
  }
}

/**** ENCODER CODE ENDED ****/

/*
 * Encoding data with the prevously described method
 *  - data:    exactly 256 byte sized uint8_t array
 *             It stores the data to be encoded
 *  - encoded: exactly 650 byte sized uint8_t array
 *             It holds the encoded data in byte format
 */ 

void encode_data(uint8_t (*data)[256], uint8_t (*encoded)[650]) {
  uint16_t i;

  // Use already allocated array to store encoded data
  Interleaver = (*encoded);

  init_encoder();
  reset_encoder();

  for (i = 0; i < 256; ++i) {
    encode_byte((*data)[i]);
  }

  for (i = 0; i < 64; ++i) {
    encode_parity();
  }
}

// for testing purpose enable built-in byte->bit converter
#if (ENABLE_BIT_OUTPUT > 0)
void encode_data_bit(uint8_t (*data)[256], uint8_t (*bit_encoded)[5200]) {
  uint8_t encoded[650] = {0};
  uint16_t i;

  // encode to byte format
  encode_data(data, &encoded);

  // convert to bit format: simply put 0 and 1 to the corresponding byte
  for (i = 0; i < 5200; ++i) {
    (*bit_encoded)[i] = ( (encoded[i >> 3] & (1 << (7 - (i & 7)))) != 0 );
  }
}
#endif
