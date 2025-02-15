/*
 * Copyright (c) 2016-2020 Belledonne Communications SARL.
 *
 * This file is part of bctoolbox.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "bctoolbox/port.h"
#include "bctoolbox/vfs.h"
#include "bctoolbox_tester.h"
#include <inttypes.h>
#include <stdio.h>

static void bytes_to_from_hexa_strings(void) {
	const uint8_t a55aBytes[2] = {0xa5, 0x5a};
	const uint8_t a55aString[5] = "a55a";
	const uint8_t upBytes[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	const uint8_t upString[17] = "0123456789abcdef";
	const uint8_t downBytes[8] = {0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
	const uint8_t downString[17] = "fedcba9876543210";
	uint8_t outputBytes[16];
	uint8_t outputString[17];

	BC_ASSERT_EQUAL(bctbx_char_to_byte("1"[0]), 1, uint8_t, "%d");
	BC_ASSERT_EQUAL(bctbx_char_to_byte("5"[0]), 5, uint8_t, "%d");
	BC_ASSERT_EQUAL(bctbx_char_to_byte("a"[0]), 10, uint8_t, "%d");
	BC_ASSERT_EQUAL(bctbx_char_to_byte("e"[0]), 14, uint8_t, "%d");
	BC_ASSERT_EQUAL(bctbx_char_to_byte("B"[0]), 11, uint8_t, "%d");
	BC_ASSERT_EQUAL(bctbx_char_to_byte("F"[0]), 15, uint8_t, "%d");

	BC_ASSERT_EQUAL(bctbx_byte_to_char(0), "0"[0], char, "%c");
	BC_ASSERT_EQUAL(bctbx_byte_to_char(2), "2"[0], char, "%c");
	BC_ASSERT_EQUAL(bctbx_byte_to_char(5), "5"[0], char, "%c");
	BC_ASSERT_EQUAL(bctbx_byte_to_char(0x0a), "a"[0], char, "%c");
	BC_ASSERT_EQUAL(bctbx_byte_to_char(0x0c), "c"[0], char, "%c");
	BC_ASSERT_EQUAL(bctbx_byte_to_char(0x0e), "e"[0], char, "%c");

	bctbx_str_to_uint8(outputBytes, a55aString, 4);
	BC_ASSERT_NSTRING_EQUAL((char *)outputBytes, (char *)a55aBytes, 2);
	bctbx_str_to_uint8(outputBytes, upString, 16);
	BC_ASSERT_NSTRING_EQUAL((char *)outputBytes, (char *)upBytes, 8);
	bctbx_str_to_uint8(outputBytes, downString, 16);
	BC_ASSERT_NSTRING_EQUAL((char *)outputBytes, (char *)downBytes, 8);

	bctbx_int8_to_str(outputString, a55aBytes, 2);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, (char *)a55aString, 4);
	bctbx_int8_to_str(outputString, upBytes, 8);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, (char *)upString, 16);
	bctbx_int8_to_str(outputString, downBytes, 8);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, (char *)downString, 16);

	bctbx_uint32_to_str(outputString, 0x5aa5c376);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, "5aa5c376", 8);
	bctbx_uint32_to_str(outputString, 0x01234567);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, "01234567", 8);
	bctbx_uint32_to_str(outputString, 0xfedcba98);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, "fedcba98", 8);

	BC_ASSERT_EQUAL(bctbx_str_to_uint32((uint8_t *)"5aa5c376"), 0x5aa5c376, uint32_t, "0x%08x");
	BC_ASSERT_EQUAL(bctbx_str_to_uint32((uint8_t *)"01234567"), 0x01234567, uint32_t, "0x%08x");
	BC_ASSERT_EQUAL(bctbx_str_to_uint32((uint8_t *)"fedcba98"), 0xfedcba98, uint32_t, "0x%08x");

	bctbx_uint64_to_str(outputString, 0xfa5c37643cde8de0);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, "fa5c37643cde8de0", 16);
	bctbx_uint64_to_str(outputString, 0x0123456789abcdef);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, "0123456789abcdef", 16);
	bctbx_uint64_to_str(outputString, 0xfedcba9876543210);
	BC_ASSERT_NSTRING_EQUAL((char *)outputString, "fedcba9876543210", 16);

	BC_ASSERT_EQUAL(bctbx_str_to_uint64((uint8_t *)"fa5c37643cde8de0"), 0xfa5c37643cde8de0, uint64_t, "0x%" PRIx64);
	BC_ASSERT_EQUAL(bctbx_str_to_uint64((uint8_t *)"0123456789abcdef"), 0x0123456789abcdef, uint64_t, "0x%" PRIx64);
	BC_ASSERT_EQUAL(bctbx_str_to_uint64((uint8_t *)"fedcba9876543210"), 0xfedcba9876543210, uint64_t, "0x%" PRIx64);
}

static void time_functions(void) {
	bctoolboxTimeSpec testTs;
	bctoolboxTimeSpec y2k, monday6Feb2017;
	y2k.tv_sec = 946684800;
	y2k.tv_nsec = 123456789;
	monday6Feb2017.tv_sec = 1486347823;
	monday6Feb2017.tv_nsec = 0;

	memcpy(&testTs, &y2k, sizeof(bctoolboxTimeSpec));
	BC_ASSERT_EQUAL(bctbx_timespec_compare(&y2k, &testTs), 0, int, "%d");
	bctbx_timespec_add_secs(&testTs, 604800);
	BC_ASSERT_EQUAL(testTs.tv_sec, y2k.tv_sec + 604800, int64_t, "%" PRIi64);
	BC_ASSERT_EQUAL(testTs.tv_nsec, y2k.tv_nsec, int64_t, "%" PRIi64);
	BC_ASSERT_TRUE(bctbx_timespec_compare(&y2k, &testTs) < 0);

	memcpy(&testTs, &y2k, sizeof(bctoolboxTimeSpec));
	bctbx_timespec_add_secs(&testTs, -604800);
	BC_ASSERT_EQUAL(testTs.tv_sec, y2k.tv_sec - 604800, int64_t, "%" PRIi64);
	BC_ASSERT_EQUAL(testTs.tv_nsec, y2k.tv_nsec, int64_t, "%" PRIi64);
	BC_ASSERT_TRUE(bctbx_timespec_compare(&y2k, &testTs) > 0);

	memcpy(&testTs, &y2k, sizeof(bctoolboxTimeSpec));
	bctbx_timespec_add_secs(&testTs, -946684801);
	BC_ASSERT_EQUAL(testTs.tv_sec, 0, int64_t, "%" PRIi64);
	BC_ASSERT_EQUAL(testTs.tv_nsec, 0, int64_t, "%" PRIi64);

	memcpy(&testTs, &y2k, sizeof(bctoolboxTimeSpec));
	bctbx_timespec_add_millisecs(&testTs, 432);
	BC_ASSERT_EQUAL(testTs.tv_sec, 946684800, int64_t, "%" PRIi64);
	BC_ASSERT_EQUAL(testTs.tv_nsec, 555456789, int64_t, "%" PRIi64);

	memcpy(&testTs, &y2k, sizeof(bctoolboxTimeSpec));
	bctbx_timespec_add_millisecs(&testTs, 1098);
	BC_ASSERT_EQUAL(testTs.tv_sec, 946684801, int64_t, "%" PRIi64);
	BC_ASSERT_EQUAL(testTs.tv_nsec, 221456789, int64_t, "%" PRIi64);

	memcpy(&testTs, &y2k, sizeof(bctoolboxTimeSpec));
	bctbx_timespec_add_millisecs(&testTs, -300);
	BC_ASSERT_EQUAL(testTs.tv_sec, 946684799, int64_t, "%" PRIi64);
	BC_ASSERT_EQUAL(testTs.tv_nsec, 823456789, int64_t, "%" PRIi64);

	/* test the get utc time function
	 * there is no easy way to ensure we get the correct time, just check it is at least not the time from last boot,
	 * check it is greater than the current time as this test was written(6feb2017) */
	bctbx_get_utc_cur_time(&testTs);
	BC_ASSERT_TRUE(bctbx_timespec_compare(&testTs, &monday6Feb2017) > 0);

	BC_ASSERT_EQUAL(bctbx_time_string_to_sec(NULL), 0, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec(""), 0, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("0"), 0, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("1500"), 1500, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("2500s"), 2500, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("10m"), 600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("5h"), 5 * 3600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("2d"), 2 * 24 * 3600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("3W"), 3 * 7 * 24 * 3600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("6M"), 6 * 30 * 24 * 3600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("7Y"), 7 * 365 * 24 * 3600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("7Y6M2W"), (7 * 365 + 6 * 30 + 2 * 7) * 24 * 3600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("2m30"), 2 * 60 + 30, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("15d1M"), (15 + 1 * 30) * 24 * 3600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("15d5z"), 15 * 24 * 3600, uint32_t, "%d");
	BC_ASSERT_EQUAL(bctbx_time_string_to_sec("15dM12h"), (15 * 24 + 12) * 3600, uint32_t, "%d");
}

static void bctbx_addrinfo_sort_test(void) {
	struct addrinfo *res1 = bctbx_name_to_addrinfo(AF_INET6, SOCK_DGRAM, "sip3.linphone.org", 27256);
	struct addrinfo *res2 = bctbx_ip_address_to_addrinfo(AF_INET6, SOCK_DGRAM, "91.121.209.194", 27256);
	struct addrinfo *res3 = bctbx_ip_address_to_addrinfo(AF_INET, SOCK_DGRAM, "91.121.209.194", 27256);
	bool_t searching_for_v6 = TRUE;
	struct addrinfo *ai = NULL;
	char printable_ip[256];
	struct addrinfo *res = res3;
	struct addrinfo *to_free = NULL;

	for (ai = res2; ai != NULL; ai = ai->ai_next) {
		if (IN6_IS_ADDR_V4MAPPED(&((struct sockaddr_in6 *)(ai->ai_addr))->sin6_addr)) {
			res->ai_next = ai;
			break;
		}
	}
	res->ai_next->ai_next = res1;
	to_free = res1->ai_next;
	res->ai_next->ai_next->ai_next = NULL;

	// So now, res as ipv4 first, then v4 mapped, then v6
	for (ai = res; ai != NULL; ai = ai->ai_next) {
		bctbx_addrinfo_to_printable_ip_address(ai, printable_ip, sizeof(printable_ip));
		bctbx_message("bctbx_getaddrinfo origin address:%s", printable_ip);
	}

	// now apply bctbx_addrinfo_sort
	res = bctbx_addrinfo_sort(res);
	for (ai = res; ai != NULL; ai = ai->ai_next) {
		if (ai->ai_family == AF_INET6) {
			if (!searching_for_v6) {
				BC_ASSERT_FALSE(IN6_IS_ADDR_V4MAPPED(&((struct sockaddr_in6 *)(ai->ai_addr))->sin6_addr));
			}
			if (IN6_IS_ADDR_V4MAPPED(&((struct sockaddr_in6 *)(ai->ai_addr))->sin6_addr)) {
				searching_for_v6 = FALSE; // stating from now, all subsequent IP shall be pure V6
			}
		}
		bctbx_addrinfo_to_printable_ip_address(ai, printable_ip, sizeof(printable_ip));
		bctbx_message("bctbx_getaddrinfo sorted address:%s", printable_ip);
	}

	bctbx_freeaddrinfo(res);
	bctbx_freeaddrinfo(to_free);
}

static void bctbx_directory_utils_test(void) {
	// Create a directory in the writeable one
	char *tmpDirPath = bctbx_strdup_printf("%s/tmp_dir/", bc_tester_get_writable_dir_prefix());
	// Check the directory does not exists
	bool_t tmpDirExists = bctbx_directory_exists(tmpDirPath);
	BC_ASSERT_FALSE(tmpDirExists); // This directory must not exists when performing this test, if it does, it means the
	                               // test failed before, delete it manually
	if (tmpDirExists) return;

	// Create it
	BC_ASSERT_EQUAL(bctbx_mkdir(tmpDirPath), 0, int, "%d");
	// Check it is there
	BC_ASSERT_TRUE(bctbx_directory_exists(tmpDirPath));

	// Try to create 2 layers of subdirectory, it shall fail
	char *tmpDirPath2 = bctbx_strdup_printf("%s/a/b/", tmpDirPath);
	BC_ASSERT_FALSE(bctbx_directory_exists(tmpDirPath2));
	BC_ASSERT_NOT_EQUAL(bctbx_mkdir(tmpDirPath2), 0, int, "%d");
	BC_ASSERT_FALSE(bctbx_directory_exists(tmpDirPath2));
	bctbx_free(tmpDirPath2);

	// Create a subdirectory
	tmpDirPath2 = bctbx_strdup_printf("%s/a/", tmpDirPath);
	BC_ASSERT_FALSE(bctbx_directory_exists(tmpDirPath2));
	BC_ASSERT_EQUAL(bctbx_mkdir(tmpDirPath2), 0, int, "%d");
	BC_ASSERT_TRUE(bctbx_directory_exists(tmpDirPath2));

	// Create empty files in tmpDirPath and tmpDirPath2
	char *filename1 = bctbx_strdup_printf("%s/filename1.txt", tmpDirPath);
	char *filename2 = bctbx_strdup_printf("%s/.filename2.txt", tmpDirPath);
	char *filename3 = bctbx_strdup_printf("%s/filename3", tmpDirPath2);
	bctbx_vfs_t *stdVfs = bctbx_vfs_get_standard();

	bctbx_vfs_file_t *fp = bctbx_file_open(stdVfs, filename1, "w");
	bctbx_file_close(fp);
	fp = bctbx_file_open(stdVfs, filename2, "w");
	bctbx_file_close(fp);
	fp = bctbx_file_open(stdVfs, filename3, "w");
	bctbx_file_close(fp);

	// Check the files exist
	BC_ASSERT_EQUAL(bctbx_file_exist(filename1), 0, int, "%d");
	BC_ASSERT_EQUAL(bctbx_file_exist(filename2), 0, int, "%d");
	BC_ASSERT_EQUAL(bctbx_file_exist(filename3), 0, int, "%d");

	// Try to delete the directory non recursively, it shall fail
	BC_ASSERT_NOT_EQUAL(bctbx_rmdir(tmpDirPath, FALSE), 0, int, "%d");

	// Delete using recursivity
	BC_ASSERT_EQUAL(bctbx_rmdir(tmpDirPath, TRUE), 0, int, "%d");
	// Check it is not there anymore
	BC_ASSERT_FALSE(bctbx_directory_exists(tmpDirPath));

	// cleaning
	bctbx_free(tmpDirPath);
	bctbx_free(tmpDirPath2);
	bctbx_free(filename1);
	bctbx_free(filename2);
	bctbx_free(filename3);
}

static test_t utils_tests[] = {
    TEST_NO_TAG("Bytes to/from Hexa strings", bytes_to_from_hexa_strings), TEST_NO_TAG("Time", time_functions),
    TEST_NO_TAG("Addrinfo sort", bctbx_addrinfo_sort_test), TEST_NO_TAG("Directory utils", bctbx_directory_utils_test)};

test_suite_t utils_test_suite = {"Utils",     NULL, NULL, NULL, NULL, sizeof(utils_tests) / sizeof(utils_tests[0]),
                                 utils_tests, 0};
