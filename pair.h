#ifndef PAIR_H
#define PAIR_H

#include <stdbool.h>
#include "htslib/htslib/sam.h"
#include "htslib/htslib/faidx.h"
#include "htslib/htslib/kstring.h"
#include "htslib/htslib/khash.h"
#include "htslib/htslib/hts.h"

#define MAX_STR_SIZE 1000

// Structure for pair reads information: sequences, quality, positions/names/tids, cigar, flag.
typedef struct {
    unsigned char qname[40];
    int32_t tid, mtid;
    int32_t pos, mpos;
    uint32_t flag:16, mflag:16;
    int32_t l_qseq, ml_qseq;
    uint8_t seq[MAX_STR_SIZE], mseq[MAX_STR_SIZE];
    uint8_t qual[MAX_STR_SIZE], mqual[MAX_STR_SIZE];
    uint32_t cigar[MAX_STR_SIZE], mcigar[MAX_STR_SIZE];
    uint32_t n_cigar:16, mn_cigar:16;
    bool reverse, mreverse;
} bam1_pair;

bam1_pair pair_make(bam1_t *b, bam1_t *m);
char* pair_serialize(bam1_pair pair);

#endif /* PAIR_H */
