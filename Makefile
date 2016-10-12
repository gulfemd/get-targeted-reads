EXE_TRF = targeted-read-finder

SOURCES_TRF = main.c mate.c read.c pair.c util.c

trf:
	make clean
	gcc $(SOURCES_TRF) -Ihtslib -Lhtslib htslib/libhts.a -lz -lpthread `pkg-config --cflags --libs glib-2.0` -o $(EXE_TRF)

libs:
	make -C htslib

clean:
	rm -f $(EXE_TRF) *.o *~ \#*