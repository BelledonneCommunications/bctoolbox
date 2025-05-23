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

#ifndef BCTBX_REGEX_H
#define BCTBX_REGEX_H

#include "bctoolbox/port.h"

#ifdef __cplusplus
extern "C" {
#endif

BCTBX_PUBLIC bool_t bctbx_is_matching_regex(const char *entry, const char *regex);
BCTBX_PUBLIC bool_t bctbx_is_matching_regex_log(const char *entry, const char *regex, bool_t show_log);
BCTBX_PUBLIC bool_t bctbx_is_matching_regex_log_context(const char *entry,
                                                        const char *regex,
                                                        bool_t show_log,
                                                        const char *context);

#ifdef __cplusplus
}
#endif

#endif /* BCTBX_REGEX_H */
