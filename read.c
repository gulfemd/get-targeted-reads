#include "read.h"

#include <assert.h>
#include <stdio.h>

GPtrArray* get_reads(hts_idx_t* index, htsFile *file, bam_hdr_t *header, int tid, int beg, int end, int *read_count) {
    GPtrArray *reads = g_ptr_array_new();

    char region[100];
    sprintf(region, "chr%d:%d-%d", tid, beg, end);

    hts_itr_t *iter = sam_itr_querys(index, header, region);
    assert(iter != NULL);

    while (1) {
        bam1_t *b = bam_init1();

        if (sam_itr_next(file, iter, b) < 0) {
            break;
        }

        int flag_val = (b)->core.flag;

        // eliminate if 0x100/0x200/0x400/0x800
        if ((flag_val & BAM_FSUPPLEMENTARY) || (flag_val & BAM_FDUP) || (flag_val & BAM_FQCFAIL) || (flag_val & BAM_FSECONDARY)) {
            continue;
        }
        // eliminate read itself and the mate are both unmapped
        if ((flag_val & BAM_FUNMAP) && (flag_val & BAM_FMUNMAP)) {
            continue;
        }

        g_ptr_array_add(reads, b);
        (*read_count)++;
    }

    return reads;
}
