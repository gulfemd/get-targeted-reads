#include "pair.h"
#include "mate.h"
#include "read.h"
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
#include <unistd.h>

void print(FILE *out, bam1_pair pairs[], int length) {
    for (int i = 0; i < length; i++) {
        char *line = pair_serialize(pairs[i]);
        fprintf(out,"%s\n", line);
        free(line);
    }
}

void iterator(gpointer key, gpointer value, gpointer user_data) {
    printf(user_data, key, value);
}

samFile* file_for_str(char *output_dir, int tid, int beg, int end, bam_hdr_t *header) {
    char name[1000];
    sprintf(name, "%s/%d_%d-%d.sam", output_dir, tid, beg, end);

    samFile *fp = sam_open(name, "w");
    sam_hdr_write(fp, header);

    return fp;
}

void write_metadata_for_str(char *output_dir, int tid, int beg, int end, int repeat_length, float copy_number, float match_percentage, char *repeat_unit) {
    char name[1000];
    sprintf(name, "%s/%d_%d-%d.metadata", output_dir, tid, beg, end);

    FILE *fp = fopen(name, "w");

    fprintf(fp, "%d\t%d\t%d\t%d\t%.1f\t%.1f\t%s\n", tid, beg, end, repeat_length, copy_number, match_percentage, repeat_unit);

    fclose(fp);
}

int main(int argc, char *argv[]) {
    hts_itr_t *iter = NULL;
    hts_idx_t *idx = NULL;
    htsFile *infile = NULL;

    bam_hdr_t *header = NULL;

    int k_INSERT_SIZE = 0, p_INCLUDE_MATES = 0;
    char *str_file_path, *bam_file_path, *output_dir;

    static struct option long_options[] = {
        { "str",           required_argument, 0, 's' },
        { "bam",           required_argument, 0, 'b' },
        { "out",           required_argument, 0, 'o' },
        { "include-mates", no_argument,       0, 'm' },
        { "insert-size",   required_argument, 0, 'i' },
        { 0,               0,                 0,  0  }
    };

    int opt, option_index = 0;
    while ((opt = getopt_long(argc, argv, "s:b:mi:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 's':
                str_file_path = optarg;
                break;
            case 'b':
                bam_file_path = optarg;
                break;
            case 'o':
                output_dir = optarg;
                break;
            case 'm':
                p_INCLUDE_MATES = 1;
                break;
            case 'i':
                k_INSERT_SIZE = atoi(optarg);
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    FILE *strFile = fopen(str_file_path, "r");
    assert(strFile != NULL);

    infile = hts_open(bam_file_path, "r");
    assert(infile != NULL);

    htsFile *mate_file = hts_open(bam_file_path, "r");
    assert(mate_file != NULL);

    idx = sam_index_load(infile, bam_file_path);
    assert(idx != NULL);

    hts_idx_t *mate_idx = sam_index_load(mate_file, bam_file_path);
    assert(mate_idx != NULL);

    header = bam_hdr_read((infile->fp).bgzf);
    assert(header != NULL);

    // iterate over str file, get only chr-start-end positions of the str
    char line[300];

    while (fgets(line, sizeof line, strFile) != NULL) {
        int tid, beg, end, repeat_length;
        float copy_number, match_percentage;
        char repeat_unit[100];

        sscanf(line, "%*c %*c %*c %d %d %d %d %f %f %s", &tid, &beg, &end, &repeat_length, &copy_number, &match_percentage, repeat_unit);
        printf("%d %d %d %d %f %f %s\n", tid, beg, end, repeat_length, copy_number, match_percentage, repeat_unit);

        int read_count = 0;
        GPtrArray *reads = get_reads(idx, infile, header, tid, beg - k_INSERT_SIZE, end + k_INSERT_SIZE, &read_count);

        samFile *reads_sam = file_for_str(output_dir, tid, beg, end, header);
        GHashTable *visited = g_hash_table_new(g_str_hash, g_str_equal); // <qname, count>

        for (int i = 0; i < read_count; ++i) {
            bam1_t *b = g_ptr_array_index(reads, i);

            gchar *qname = g_strdup(bam_get_qname(b));

            // hash table to hold reads with their qname's
            if (g_hash_table_contains(visited, qname) == TRUE) {
                continue;
            }

            int val = 1;
            g_hash_table_replace(visited, qname, GINT_TO_POINTER(val));

            if (p_INCLUDE_MATES) {
                int mate_count = 0;
                GPtrArray *mates = find_mates(mate_idx, mate_file, b, &mate_count);

                if (mate_count > 0) {
                    bam1_t *m = g_ptr_array_index(mates, 0);
                    sam_write1(reads_sam, header, b);
                    sam_write1(reads_sam, header, m);
                }
            }

            else {
                sam_write1(reads_sam, header, b);
            }

            bam_destroy1(b);
        }

        printf("------------------------------------------------------------------\n");
        printf("Read count: %d\n", read_count);
        printf("Different Qnames' for this region: %d\n", g_hash_table_size(visited));

        sam_close(reads_sam);

        write_metadata_for_str(output_dir, tid, beg, end, repeat_length, copy_number, match_percentage, repeat_unit);
    }

    hts_itr_destroy(iter);
    bam_hdr_destroy(header);
    sam_close(infile);
    sam_close(mate_file);
    fclose(strFile);

    return 0;
}
