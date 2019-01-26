
#ifndef AO40_ENC_H
#define AO40_ENC_H

#include <stdint.h>

//#define AO40_LOW_MEMORY          // low memory workaround to avoid LUT (and preserve 512byte)
//#define AO40_ENABLE_BIT_OUTPUT  // enable debug bit output mode

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define AO40_DATA_SIZE      256
#define AO40_CODE_LENGTH    650

void encode_data_ao40(const uint8_t data[AO40_DATA_SIZE], uint8_t encoded[AO40_CODE_LENGTH]);

#ifdef AO40_ENABLE_BIT_OUTPUT
void encode_data_bit(const uint8_t *data, uint8_t *bit_encoded);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
