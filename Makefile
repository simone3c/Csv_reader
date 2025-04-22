CC ?= gcc

lib: lib/libcsv_reader.a

lib/libcsv_reader.a: obj/csv_reader.o
	rm -rf $@
	ar cru $@ $^
	ranlib $@

obj/csv_reader.o: src/csv_reader.c
	$(CC) -c $^ -o $@
