#include "pair.h"
#include "util.h"

bam1_pair pair_make(bam1_t *b, bam1_t *m) {
    bam1_pair pair;

    strcpy((char *) pair.qname, bam_get_qname(b));
    pair.tid = (b)->core.tid;
    pair.mtid = (m)->core.tid;
    pair.pos = (b)->core.pos;
    pair.mpos = (m)->core.pos;
    pair.l_qseq = (b)->core.l_qseq;
    pair.ml_qseq = (m)->core.l_qseq;
    pair.n_cigar = (b)->core.n_cigar;
    pair.mn_cigar = (m)->core.n_cigar;
    pair.flag = (b)->core.flag;
    pair.mflag = (m)->core.flag;

    pair.reverse = (b)->core.flag & BAM_FREVERSE;
    pair.mreverse = (m)->core.flag & BAM_FREVERSE;

    memcpy(pair.seq, bam_get_seq(b), pair.l_qseq);
    memcpy(pair.mseq, bam_get_seq(m), pair.ml_qseq);

    memcpy(pair.qual, bam_get_qual(b), pair.l_qseq);
    memcpy(pair.mqual, bam_get_qual(m), pair.ml_qseq);

    qual_to_ascii(pair.qual, pair.l_qseq);
    qual_to_ascii(pair.mqual, pair.ml_qseq);

    if (pair.reverse) {
        reverse((char *) pair.qual, pair.l_qseq);
    }

    if (pair.mreverse) {
        reverse((char *) pair.mqual, pair.ml_qseq);
    }

    memcpy(pair.cigar, bam_get_cigar(b), pair.n_cigar);
    memcpy(pair.mcigar, bam_get_cigar(m), pair.mn_cigar);

    return pair;
}

char* pair_serialize(bam1_pair pair) {
    char* str = (char *) malloc(sizeof(char) * 10000);
    char *sequence = seq_to_string(pair.l_qseq, pair.seq, pair.reverse);
    char *mate_sequence = seq_to_string(pair.ml_qseq, pair.mseq, pair.mreverse);

    sprintf(str, "%s\t%d\t%d\t%d\t%d\t%lu\t%s\t%d\t%d\t%d\t%d\t%s", pair.qname, pair.flag, pair.tid, pair.pos, pair.l_qseq, strlen(sequence), sequence, pair.mflag, pair.mtid, pair.mpos, pair.ml_qseq, mate_sequence);

    free(sequence);
    free(mate_sequence);

    return str;
}
