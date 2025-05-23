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

#ifndef BCTBX_CHARCONV_H
#define BCTBX_CHARCONV_H

#include "bctoolbox/port.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set the default encoding for the application.
 *
 * @deprecated 2024/07/09: use setLocale() from <locale.h> to set the locale. On Windows, CP_APC will be used as
 * default.
 * @param[in] encoding default encoding, "locale" to set it to the system's locale
 */
BCTBX_DEPRECATED BCTBX_PUBLIC void bctbx_set_default_encoding(const char *encoding);

/**
 * @brief Return the default encoding for the application.
 *
 * @return a pointer to a null-terminated string containing the default encoding.
 */
BCTBX_PUBLIC const char *bctbx_get_default_encoding(void);

/**
 * @brief Convert the given string from system locale to UTF8.
 *
 * @param[in] str string to convert
 *
 * @return a pointer to a null-terminated string containing the converted string. This buffer must then be freed
 * by caller. NULL on failure.
 */
BCTBX_PUBLIC char *bctbx_locale_to_utf8(const char *str);

/**
 * @brief Convert the given string from UTF8 to system locale.
 *
 * @param[in] str string to convert
 *
 * @return a pointer to a null-terminated string containing the converted string. This buffer must then be freed
 * by caller. NULL on failure.
 */
BCTBX_PUBLIC char *bctbx_utf8_to_locale(const char *str);

/**
 * @brief Convert the given string.
 *
 * @param[in] str string to convert
 * @param[in] encoding charset of the string
 *
 * @return a pointer to a null-terminated string containing the converted string. This buffer must then be freed
 * by caller. NULL on failure.
 *
 * @note If encoding is equal to "locale" or NULL then it will use the system's locale
 * @note If encoding is UTF-8 then it returns a copy of str
 */
BCTBX_PUBLIC char *bctbx_convert_any_to_utf8(const char *str, const char *encoding);

/**
 * @brief Convert the given string.
 *
 * @param[in] str string to convert
 * @param[in] encoding charset of the string
 *
 * @return a pointer to a null-terminated string containing the converted string. This buffer must then be freed
 * by caller. NULL on failure.
 *
 * @note If encoding is equal to "locale" or NULL then it will use the system's locale
 * @note If encoding is UTF-8 then it returns a copy of str
 */
BCTBX_PUBLIC char *bctbx_convert_utf8_to_any(const char *str, const char *encoding);

/**
 * @brief Convert the given string.
 *
 * @param[in] str string to convert
 * @param[in] encoding charset of the string
 * @param[in] encoding charset of the string
 *
 * @return a pointer to a null-terminated string containing the converted string. This buffer must then be freed
 * by caller. NULL on failure.
 *
 * @note If from_encoding or to_encoding is equal to "locale" or NULL then it will use the system's locale. Use
 * bctbx_get_default_encoding() to get the application locale.
 * @note If encodings are the same then it returns a copy of str
 */
BCTBX_PUBLIC char *bctbx_convert_string(const char *str, const char *from_encoding, const char *to_encoding);

/**
 * @brief Convert the char string to wide char string using current locale.
 *
 * @param[in] str string to convert
 *
 * @return a pointer to a null-terminated string containing the converted string. This buffer must then be freed
 * by caller. NULL on failure.
 */
BCTBX_PUBLIC wchar_t *bctbx_string_to_wide_string(const char *s);

/**
 * @brief Convert the wide char string to char string using current locale.
 *
 * @param[in] wstr wide string to convert
 *
 * @return a pointer to a null-terminated string containing the converted string. This buffer must then be freed
 * by caller. NULL on failure.
 */
BCTBX_PUBLIC char *bctbx_wide_string_to_string(const wchar_t *wstr);

/**
 * @brief Return the code page from the encoding. Only available for Windows platform
 *
 * @param[in] encoding string to convert. If NULL or "", return the code page defined by bctbx_get_default_encoding().
 * @maybenil
 *
 * @return The code page associated to the encoding. Return system's locale if not found.
 */
BCTBX_PUBLIC unsigned int bctbx_get_code_page(const char *encoding);

#ifdef __cplusplus
}
#endif

#endif /* BCTBX_CHARCONV_H */
