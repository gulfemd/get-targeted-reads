#ifndef READ_H
#define READ_H

#include <glib.h>
#include "htslib/htslib/hts.h"
#include "htslib/htslib/sam.h"

GPtrArray* get_reads(hts_idx_t* index, htsFile *file, bam_hdr_t *header, int tid, int beg, int end, int *read_count);

#endif /* READ_H */
