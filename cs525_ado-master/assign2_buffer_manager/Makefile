assignment2_test1: test_assign2_1.o storage_mgr.o buffer_mgr.o buffer_mgr_stat.o dberror.o
	gcc test_assign2_1.o buffer_mgr.o buffer_mgr_stat.o storage_mgr.o dberror.o -o assignment2_test1

assignment2_test2: test_assign2_2.o storage_mgr.o buffer_mgr.o buffer_mgr_stat.o dberror.o
	gcc test_assign2_2.o buffer_mgr.o storage_mgr.o dberror.o -o assignment2_test2

dberror.o: dberror.c dberror.h
	gcc -c dberror.c

storage_mgr.o: storage_mgr.c storage_mgr.h mgmt_info.h
	gcc -c storage_mgr.c

buffer_mgr.o: buffer_mgr.c buffer_mgr.h
	gcc -c buffer_mgr.c storage_mgr.c

buffer_mgr_stat.o: buffer_mgr_stat.c buffer_mgr_stat.h
	gcc -c buffer_mgr_stat.c

test_assign1_1.o: test_assign2_1.c test_assign2_2.c test_helper.h
	gcc -c test_assign2_1.c test_assign2_2.c

# misc.o: dt.h
# 	gcc -c test_assign1_1.c test_helper.h

clean:
	rm assignment2_test1 assignment2_test2 *.o