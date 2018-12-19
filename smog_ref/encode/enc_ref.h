
#ifndef ENC_REF_H
#define ENC_REF_H

#include <stdint.h>

//#define AO40_LOW_MEMORY          // low memory workaround to avoid LUT (and preserve 512byte)
//#define AO40_ENABLE_BIT_OUTPUT  // enable debug bit output mode

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void encode_data(const uint8_t *data, uint8_t *encoded);

#ifdef AO40_ENABLE_BIT_OUTPUT
void encode_data_bit(const uint8_t *data, uint8_t *bit_encoded);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
