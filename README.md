# get-targeted-reads

on Linux:
gcc -std=c99 main.c mate.c read.c pair.c util.c -Ihtslib -Lhtslib htslib/libhts.a -lz -lpthread `pkg-config --cflags --libs glib-2.0` -o targeted-read-finder

on Mac:
gcc main.c mate.c read.c pair.c util.c -Wl,-stack_size -Wl,8000000 -Ihtslib -Lhtslib htslib/libhts.a -lz -lpthread `pkg-config --cflags --libs glib-2.0` -o targeted-read-finder

