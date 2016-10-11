#ifndef MATE_H
#define MATE_H

#include <glib.h>
#include "htslib/htslib/hts.h"
#include "htslib/htslib/sam.h"

GPtrArray* find_mates(hts_idx_t* index, htsFile *file, bam1_t *b, int *mate_count);

#endif /* MATE_H */
