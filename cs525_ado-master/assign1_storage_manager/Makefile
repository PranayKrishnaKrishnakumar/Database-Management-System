assignment: test_assign1_1.o storage_mgr.o dberror.o
	gcc test_assign1_1.o storage_mgr.o dberror.o -o assignment

dberror.o: dberror.c dberror.h
	gcc -c dberror.c

storage_mgr.o: storage_mgr.c storage_mgr.h
	gcc -c storage_mgr.c

test_assign1_1.o: test_assign1_1.c
	gcc -c test_assign1_1.c test_helper.h

clean:
	rm assignment