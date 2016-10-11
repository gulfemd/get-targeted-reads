#include "util.h"

#include <stdlib.h>
#include "htslib/htslib/sam.h"

char complement(char base);

void creverse(char *sequence, int32_t len) {
    reverse(sequence, len);

    for (int k = 0; k < len; ++k) {
        sequence[k] = complement(sequence[k]);
    }
}

void reverse(char *sequence, int32_t len) {
    int i = 0, j = len - 1;

    while (i < j) {
        char c = sequence[i];
        sequence[i++] = sequence[j];
        sequence[j--] = c;
    }
}

char* seq_to_string(int32_t l_qseq, uint8_t *seq, bool reverse) {
    char *sequence = (char *) malloc(l_qseq + 1);

    for (int i = 0; i < l_qseq; i++) {
        sequence[i] = seq_nt16_str[bam_seqi(seq,i)];
    }

    sequence[l_qseq] = '\0';

    if (reverse) {
        creverse(sequence, l_qseq);
    }

    return sequence;
}

void qual_to_ascii(uint8_t* qual, int32_t len) {
    for (int i = 0; i < len; i++) {
        qual[i] = qual[i] + 33;
    }
}

char complement(char base) {
    switch (base) {
        case 'A':
            return 'T';
        case 'C':
            return 'G';
        case 'G':
            return 'C';
        case 'T':
            return 'A';
        default:
            return 'N';
    }

    return 'X';
}
