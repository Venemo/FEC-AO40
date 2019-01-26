#ifndef AO40SHORT_ENC_H
#define AO40SHORT_ENC_H

#include <stdint.h>

#define AO40SHORT_DATA_SIZE      128
#define AO40SHORT_CODE_LENGTH    332

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void encode_data_ao40short(const uint8_t data[AO40SHORT_DATA_SIZE], uint8_t encoded[AO40SHORT_CODE_LENGTH]);

#ifdef AO40SHORT_ENABLE_BIT_OUTPUT
void encode_short_data_bit(uint8_t *data, uint8_t *bit_encoded);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // AO40SHORT_ENC_H
