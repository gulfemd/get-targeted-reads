#include "mate.h"

#include <assert.h>
#include <string.h>

GPtrArray* find_mates(hts_idx_t* index, htsFile *file, bam1_t *b, int *mate_count) {
    GPtrArray *mates = g_ptr_array_new();

    hts_itr_t *mate_iter = sam_itr_queryi(index, (b)->core.mtid, (b)->core.mpos, (b)->core.mpos + (b)->core.l_qseq);
    assert(mate_iter != NULL);

    int flag_val = (b)->core.flag;

    while (1) {
        bam1_t *m = bam_init1();

        if (sam_itr_next(file, mate_iter, m) < 0) {
            break;
        }

        int mate_flag_val = (m)->core.flag;

        if (strcmp(bam_get_qname(b), bam_get_qname(m)) != 0) {
            continue;
        }
        if ((b)->core.tid != (m)->core.tid) {
            continue;
        }
        if ((mate_flag_val & BAM_FSUPPLEMENTARY) || (mate_flag_val & BAM_FDUP) || (mate_flag_val & BAM_FQCFAIL) || (mate_flag_val & BAM_FSECONDARY)) {
            continue;
        }

        if ((mate_flag_val & BAM_FREAD1) == (flag_val & BAM_FREAD1)) {
            continue;
        }
        if ((mate_flag_val & BAM_FREAD2) == (flag_val & BAM_FREAD2)) {
            continue;
        }

        g_ptr_array_add(mates, m);
        (*mate_count)++;
    }

    return mates;
}
