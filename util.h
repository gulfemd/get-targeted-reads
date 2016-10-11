#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <inttypes.h>

void creverse(char *sequence, int32_t len);

void reverse(char *sequence, int32_t len);

char* seq_to_string(int32_t l_qseq, uint8_t *seq, bool reverse);

void qual_to_ascii(uint8_t* qual, int32_t len);

#endif /* UTIL_H */
