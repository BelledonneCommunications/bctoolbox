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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "bctoolbox/logging.h"
#include "bctoolbox/parser.h"
#include "bctoolbox/port.h"

char *bctbx_escape(const char *buff, const bctbx_noescape_rules_t noescapes) {
	size_t outbuf_size = strlen(buff);
	size_t orig_size = outbuf_size;
	char *output_buff = (char *)bctbx_malloc(outbuf_size + 1);
	int i;
	size_t out_buff_index = 0;

	for (i = 0; buff[i] != '\0'; i++) {
		int c = ((unsigned char *)buff)[i];
		if (outbuf_size < out_buff_index + 3) {
			// we will possibly add 3 chars
			outbuf_size += MAX(orig_size / 2, 3);
			output_buff = bctbx_realloc(output_buff, outbuf_size + 1);
		}
		if (noescapes[c] == 1) {
			output_buff[out_buff_index++] = c;
		} else {
			// this will write 3 characters
			out_buff_index += snprintf(output_buff + out_buff_index, outbuf_size + 1 - out_buff_index, "%%%02x", c);
		}
	}
	output_buff[out_buff_index] = '\0';
	return output_buff;
}

void bctbx_noescape_rules_add_list(bctbx_noescape_rules_t noescapes, const char *allowed) {
	while (*allowed) {
		noescapes[(unsigned int)*allowed] = 1;
		++allowed;
	}
}

void bctbx_noescape_rules_add_range(bctbx_noescape_rules_t noescapes, char first, char last) {
	memset(noescapes + (unsigned int)first, 1, last - first + 1);
}

void bctbx_noescape_rules_add_alfanums(bctbx_noescape_rules_t noescapes) {
	bctbx_noescape_rules_add_range(noescapes, '0', '9');
	bctbx_noescape_rules_add_range(noescapes, 'A', 'Z');
	bctbx_noescape_rules_add_range(noescapes, 'a', 'z');
}

static int is_escaped_char(const char *a) {
	return a[0] == '%' && a[1] != '\0' && a[2] != '\0';
}

size_t bctbx_get_char(const char *a, char *out) {
	if (is_escaped_char(a)) {
		unsigned int tmp;
		sscanf(a + 1, "%02x", &tmp);
		*out = (char)tmp;
		return 3;
	} else {
		*out = *a;
		return 1;
	}
}

char *bctbx_unescaped_string(const char *buff) {
	char *output_buff = bctbx_malloc(strlen(buff) + 1);
	size_t i;
	size_t out_buff_index = 0;

	for (i = 0; buff[i] != '\0'; out_buff_index++) {
		i += bctbx_get_char(buff + i, output_buff + out_buff_index);
	}

	output_buff[out_buff_index] = '\0';
	return output_buff;
}

char *bctbx_unescaped_string_only_chars_in_rules(const char *buff, const bctbx_noescape_rules_t unescape) {
	size_t max_len = strlen(buff) + 1;
	char *output_buff = bctbx_malloc(max_len);
	size_t i;
	size_t out_buff_index = 0;

	for (i = 0; buff[i] != '\0';) {
		i += bctbx_get_char(buff + i, output_buff + out_buff_index);

		int c = ((unsigned char *)output_buff)[out_buff_index];
		if (unescape[c] == 0 && is_escaped_char(buff + i)) {
			// we unescaped a character that should stay escaped
			max_len += 3;
			output_buff = bctbx_realloc(output_buff, max_len);
			out_buff_index += snprintf(output_buff + out_buff_index, max_len - out_buff_index, "%%%02x", c);
		} else {
			out_buff_index += 1;
		}
	}

	output_buff[out_buff_index] = '\0';
	return output_buff;
}