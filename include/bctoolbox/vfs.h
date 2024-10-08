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

#ifndef BCTBX_VFS_H
#define BCTBX_VFS_H

#include <fcntl.h>

#include <bctoolbox/port.h>

#if !defined(_WIN32_WCE)
#include <sys/stat.h>
#include <sys/types.h>
#if _MSC_VER
#include <io.h>
#endif
#endif /*_WIN32_WCE*/

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _WIN32

#ifndef S_IRUSR
#define S_IRUSR S_IREAD
#endif

#ifndef S_IWUSR
#define S_IWUSR S_IWRITE
#endif

#endif /*!_WIN32*/

#define BCTBX_VFS_OK 0 /* Successful result */

#define BCTBX_VFS_ERROR -255 /* Some kind of disk I/O error occurred */

#define BCTBX_VFS_PRINTF_PAGE_SIZE 4096   /* Size of the page hold in memory by fprintf */
#define BCTBX_VFS_GETLINE_PAGE_SIZE 17385 /* Size of the page hold in memory by getnextline */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Methods associated with the bctbx_vfs_t.
 */
typedef struct bctbx_io_methods_t bctbx_io_methods_t;

/**
 * VFS file handle.
 */
typedef struct bctbx_vfs_file_t bctbx_vfs_file_t;
struct bctbx_vfs_file_t {
	const struct bctbx_io_methods_t
	    *pMethods; /* Methods for an open file: all Developpers must supply this field at open step*/
	/*the fields below are used by the default implementation. Developpers are not required to supply them, but may use
	 * them if they find them useful*/
	void *pUserData; /* Developpers can store private data under this pointer */
	off_t offset;    /* File offset used by bctbx_file_fprintf and bctbx_file_get_nxtline */
	/* fprintf cache */
	char fPage[BCTBX_VFS_PRINTF_PAGE_SIZE]; /* Buffer storing the current page cached by fprintf */
	off_t fPageOffset;                      /* The original offset of the cached page */
	size_t fSize;                           /* number of bytes in cache */
	/* get_nxtline cache */
	char gPage[BCTBX_VFS_GETLINE_PAGE_SIZE +
	           1];     /* Buffer storing the current page cachec by get_nxtline +1 to hold the \0 */
	off_t gPageOffset; /* The offset of the cached page */
	size_t gSize;      /* actual size of the data in cache */
};

/**
 */
struct bctbx_io_methods_t {
	int (*pFuncClose)(bctbx_vfs_file_t *pFile);
	ssize_t (*pFuncRead)(bctbx_vfs_file_t *pFile, void *buf, size_t count, off_t offset);
	ssize_t (*pFuncWrite)(bctbx_vfs_file_t *pFile, const void *buf, size_t count, off_t offset);
	int (*pFuncTruncate)(bctbx_vfs_file_t *pFile, int64_t size);
	ssize_t (*pFuncFileSize)(bctbx_vfs_file_t *pFile);
	int (*pFuncSync)(bctbx_vfs_file_t *pFile);
	int (*pFuncGetLineFromFd)(bctbx_vfs_file_t *pFile, char *s, int count);
	bool_t (*pFuncIsEncrypted)(bctbx_vfs_file_t *pFile);
};

/**
 * VFS definition
 */
typedef struct bctbx_vfs_t bctbx_vfs_t;
struct bctbx_vfs_t {
	const char *vfsName; /* Virtual file system name */
	int (*pFuncOpen)(bctbx_vfs_t *pVfs, bctbx_vfs_file_t *pFile, const char *fName, int openFlags);
};

/**
 * Attempts to read count bytes from the open file given by pFile, at the position starting at offset
 * in the file and and puts them in the buffer pointed by buf.
 * @param  pFile  bctbx_vfs_file_t File handle pointer.
 * @param  buf    Buffer holding the read bytes.
 * @param  count  Number of bytes to read.
 * @param  offset Where to start reading in the file (in bytes).
 * @return        Number of bytes read on success, BCTBX_VFS_ERROR otherwise.
 */
BCTBX_PUBLIC ssize_t bctbx_file_read(bctbx_vfs_file_t *pFile, void *buf, size_t count, off_t offset);

/**
 * Attempts to read count bytes from the open file given by pFile, at the position starting at its offset
 * in the file and and puts them in the buffer pointed by buf.
 * The file offset shall be incremented by the number of bytes actually read.
 * @param  pFile  bctbx_vfs_file_t File handle pointer.
 * @param  buf    Buffer holding the read bytes.
 * @param  count  Number of bytes to read.
 * @return        Number of bytes read on success, BCTBX_VFS_ERROR otherwise.
 */
BCTBX_PUBLIC ssize_t bctbx_file_read2(bctbx_vfs_file_t *pFile, void *buf, size_t count);

/**
 * Close the file from its descriptor pointed by thw bctbx_vfs_file_t handle.
 * @param  pFile File handle pointer.
 * @return      	return value from the pFuncClose VFS Close function on success,
 *                  BCTBX_VFS_ERROR otherwise.
 */
BCTBX_PUBLIC int bctbx_file_close(bctbx_vfs_file_t *pFile);

/**
 * Allocates a bctbx_vfs_file_t file handle pointer. Opens the file fName
 * with the mode specified by the mode argument. Calls bctbx_file_open.
 * @param  pVfs  Pointer to the vfs instance in use.
 * @param  fName Absolute file path.
 * @param  mode  File access mode (char*).
 * @return  pointer to  bctbx_vfs_file_t on success, NULL otherwise.
 */
BCTBX_PUBLIC bctbx_vfs_file_t *bctbx_file_open(bctbx_vfs_t *pVfs, const char *fName, const char *mode);

/**
 * Allocates a bctbx_vfs_file_t file handle pointer. Opens the file fName
 * with the mode specified by the mode argument. Calls bctbx_file_open.
 * @param  pVfs  		Pointer to the vfs instance in use.
 * @param  fName 		Absolute file path.
 * @param  openFlags  	File access flags(integer).
 * @return  pointer to  bctbx_vfs_file_t on success, NULL otherwise.
 */
BCTBX_PUBLIC bctbx_vfs_file_t *bctbx_file_open2(bctbx_vfs_t *pVfs, const char *fName, const int openFlags);

/**
 * Returns the file size.
 * @param  pFile  bctbx_vfs_file_t File handle pointer.
 * @return       BCTBX_VFS_ERROR if an error occured, file size otherwise.
 */
BCTBX_PUBLIC ssize_t bctbx_file_size(bctbx_vfs_file_t *pFile);

/**
 * Truncates/ Extends a file.
 * @param  pFile bctbx_vfs_file_t File handle pointer.
 * @param  size  New size of the file.
 * @return       BCTBX_VFS_ERROR if an error occured, 0 otherwise.
 */
BCTBX_PUBLIC int bctbx_file_truncate(bctbx_vfs_file_t *pFile, int64_t size);

/**
 * Write count bytes contained in buf to a file associated with pFile at the position
 * offset. Calls pFuncWrite (set to bc_Write by default).
 * @param  pFile 	File handle pointer.
 * @param  buf    	Buffer hodling the values to write.
 * @param  count  	Number of bytes to write to the file.
 * @param  offset 	Position in the file where to start writing.
 * @return        	Number of bytes written on success, BCTBX_VFS_ERROR if an error occurred.
 */
BCTBX_PUBLIC ssize_t bctbx_file_write(bctbx_vfs_file_t *pFile, const void *buf, size_t count, off_t offset);

/**
 * Write count bytes contained in buf to a file associated with pFile at the position starting at its
 * offset. Calls pFuncWrite (set to bc_Write by default).
 * The file offset shall be incremented by the number of bytes actually written.
 * @param  pFile 	File handle pointer.
 * @param  buf    	Buffer hodling the values to write.
 * @param  count  	Number of bytes to write to the file.
 * @return        	Number of bytes written on success, BCTBX_VFS_ERROR if an error occurred.
 */
BCTBX_PUBLIC ssize_t bctbx_file_write2(bctbx_vfs_file_t *pFile, const void *buf, size_t count);

/**
 * Writes to file.
 * @param  pFile  File handle pointer.
 * @param  offset where to write in the file
 * @param  fmt    format argument, similar to that of printf
 * @return        Number of bytes written if success, BCTBX_VFS_ERROR otherwise.
 */
BCTBX_PUBLIC ssize_t bctbx_file_fprintf(bctbx_vfs_file_t *pFile, off_t offset, const char *fmt, ...);

/**
 * Wrapper to pFuncGetNxtLine. Returns a line with at most maxlen characters
 * from the file associated to pFile and  writes it into s.
 * @param  pFile  File handle pointer.
 * @param  s      Buffer where to store the read line.
 * @param  maxlen Number of characters to read to find a line in the file.
 * @return        BCTBX_VFS_ERROR if an error occurred, size of line read otherwise.
 */
BCTBX_PUBLIC int bctbx_file_get_nxtline(bctbx_vfs_file_t *pFile, char *s, int maxlen);

/**
 * Simply sync the file contents given through the file handle
 * to the persistent media.
 * @param  pFile  File handle pointer.
 * @return   BCTBX_VFS_OK on success, BCTBX_VFS_ERROR otherwise
 */
BCTBX_PUBLIC int bctbx_file_sync(bctbx_vfs_file_t *pFile);

/**
 * Set the position to offset in the file, this position is used only by the function
 * bctbx_file_get_nxtline. Read and write give their own offset as param and won't modify this one
 * @param  pFile  File handle pointer.
 * @param  offset File offset where to set the position to.
 * @param  whence Either SEEK_SET, SEEK_CUR,SEEK_END
 * @return        BCTBX_VFS_ERROR on error, offset otherwise.
 */
BCTBX_PUBLIC off_t bctbx_file_seek(bctbx_vfs_file_t *pFile, off_t offset, int whence);

/**
 * Get the file encryption status
 * @param  pFile  File handle pointer.
 * @return true if the file is encrypted
 */
BCTBX_PUBLIC bool_t bctbx_file_is_encrypted(bctbx_vfs_file_t *pFile);

/**
 * Set default VFS pointer pDefault to my_vfs.
 * By default, the global pointer is set to use VFS implemnted in vfs.c
 * @param my_vfs Pointer to a bctbx_vfs_t structure.
 */
BCTBX_PUBLIC void bctbx_vfs_set_default(bctbx_vfs_t *my_vfs);

/**
 * Returns the value of the global variable pDefault,
 * pointing to the default vfs used.
 * @return Pointer to bctbx_vfs_t set to operate as default VFS.
 */
BCTBX_PUBLIC bctbx_vfs_t *bctbx_vfs_get_default(void);

/**
 * Return pointer to standard VFS impletentation.
 * @return  pointer to bcVfs
 */
BCTBX_PUBLIC bctbx_vfs_t *bctbx_vfs_get_standard(void);

#ifdef __cplusplus
}
#endif

#endif /* BCTBX_VFS_H */
