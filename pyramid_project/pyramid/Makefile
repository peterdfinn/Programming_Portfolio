CFLAGS = -g -O0
LDFLAGS = -lm -lcrypto
CC = gcc

OBJS = *.o ../access_file/*.o
PYRA_OPEN_OBJS = pyramid_init.o ../access_file/pyramid_open.o decrypt_pyramid.o verify_pyramid.o
PYRA_MODIFY_OBJS = pyramid_overflow.o how_many_ids.o add_id_pyramid.o move_id_in_pyramid.o randomly_select_id_pyramid.o sizeof_pyramid.o how_many_levels.o ../access_file/pyramid_read.o ../access_file/pyramid_write.o ../access_file/read_block.o ../access_file/determine_blocks.o ../access_file/write_block_to_big_buffer.o
PYRA_CLOSE_OBJS = pyramid_free.o ../access_file/pyramid_close.o encrypt_pyramid.o sign_pyramid.o
EXECUTABLES = pyramid_tests pyramid_overflow_tests open_close_test read_block_test read_write_test
TESTFILES = readblocktestfile readblocktestfile_pyramid test2file test2file_pyramid test3file test3file_pyramid readwritetestfile readwritetestfile_pyramid

pyramid_overflow_tests: pyramid_overflow_tests.o $(PYRA_MODIFY_OBJS) pyramid_init.o pyramid_free.o pyramid.h
	$(CC) $(CFLAGS) -o $@ $(PYRA_MODIFY_OBJS) pyramid_init.o pyramid_free.o $(LDFLAGS)

pyramid_tests: pyramid_tests.o pyramid.h $(PYRA_MODIFY_OBJS) pyramid_free.o pyramid_init.o
	$(CC) $(CFLAGS) -o $@ $(PYRA_MODIFY_OBJS) pyramid_free.o pyramid_init.o pyramid_tests.o $(LDFLAGS)

open_close_test: ../access_file/open_close_test.o ../access_file/open_close_test_main.o $(PYRA_OPEN_OBJS) $(PYRA_MODIFY_OBJS) $(PYRA_CLOSE_OBJS) pyramid.h
	$(CC) $(CFLAGS) -o $@ ../access_file/open_close_test.o ../access_file/open_close_test_main.o $(PYRA_OPEN_OBJS) $(PYRA_MODIFY_OBJS) $(PYRA_CLOSE_OBJS) $(LDFLAGS)

read_block_test: ../access_file/read_block_test.o ../access_file/open_close_test.o $(PYRA_OPEN_OBJS) $(PYRA_MODIFY_OBJS) $(PYRA_CLOSE_OBJS) pyramid.h
	$(CC) $(CFLAGS) -o $@ ../access_file/open_close_test.o ../access_file/read_block_test.o $(PYRA_OPEN_OBJS) $(PYRA_MODIFY_OBJS) $(PYRA_CLOSE_OBJS) $(LDFLAGS)

read_write_test: pyramid.h $(PYRA_OPEN_OBJS) $(PYRA_CLOSE_OBJS) $(PYRA_MODIFY_OBJS) ../access_file/read_write_test.o
	$(CC) $(CFLAGS) -o $@ ../access_file/read_write_test.o $(PYRA_OPEN_OBJS) $(PYRA_CLOSE_OBJS) $(PYRA_MODIFY_OBJS) $(LDFLAGS)

%.o: %.c pyramid.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLES) $(TESTFILES)
