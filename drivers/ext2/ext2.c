#include "ext2.h"

#include "../ide/ide.h"

#include "../../lib/memory.h"
#include "../../lib/stdlib.h"
#include "../../lib/stdio.h"

#define DRIVE 0

__kstd_ext2_superblock *__kstd_ext2_head_sb;

uint32 __kstd_ext2_find_in_dir(uint32 __inode_n, char *__dname)
{
    __kstd_ext2_inode *__dnode = __kstd_ext2_read_inode(__inode_n);

    uint32 __rinode = ERRNO_FNF;
    uint32 *__dir   = kstd_mem_malloc(sizeof(*__dir));
    uint32 __db0    = __dnode->__direct_block_pointers[0];

    __kstd_ide_read_sectors(DRIVE, 6, 2 * __db0, __dir);

    int __curr = 0;
    int __next = 0;
    int __i    = 0;

    while (1)
    {
        __kstd_ext2_dirent __cdirent;

        __cdirent = __kstd_ext2_read_dirent(__dir, __curr);

        if (__cdirent.__inode == 0)
        {
            break;
        }

        __next = __cdirent.__dirent_size / 4;

        if (!__kstd_strcmp(__cdirent.__dname, __dname))
        {
            __rinode = __cdirent.__inode;
        }

        __curr = (__curr + __next);

        __i++;
    }

    return __rinode;
}

void __kstd_ext2_read_superblock(void)
{
    uint32 *__sbcopy = kstd_mem_malloc(sizeof(*__sbcopy));

    __kstd_ide_read_sectors(DRIVE, 1, 2, __kstd_ext2_head_sb);
}

void __kstd_ext2_init(void)
{
    if (__kstd_ext2_head_sb->__magic != 0xEF53)
    {
        kstd_write("DISK READ: Error: [Filesystem != EXT2]\n");
    }
    else
    {
        /* Do nothing cause everything ok */

        kstd_write("DISK READ: Ok!");
    }
}

char *__kstd_ext2_read_file(char *__fpath)
{
    char *__fbuffer = kstd_mem_malloc(sizeof(*__fbuffer));

    uint32 __finode = __kstd_ext2_path_to_inode(__fpath);

    __fbuffer = __kstd_ext2_get_filedata(__finode);

    return __fbuffer;
}

char **__kstd_ext2_flist(uint32 __inode_n)
{
    char **__rnames = kstd_mem_malloc(sizeof(**__rnames));

    __kstd_ext2_inode *__dnode = __kstd_ext2_read_inode(__inode_n);

    uint32 *__dir = kstd_mem_malloc(sizeof(*__dir));
    uint32 __db0  = __dnode->__direct_block_pointers[0];

    __kstd_ide_read_sectors(DRIVE, 6, 2 * __db0, __dir);

    int __curr = 0;
    int __next = 0;
    int __i    = 0;

    while (1)
    {
        __kstd_ext2_dirent __cdirent;

        __cdirent = __kstd_ext2_read_dirent(__dir, __curr);

        if (__cdirent.__inode == 0)
        {
            break;
        }

        __next = __cdirent.__dirent_size / 4;

        __rnames[__i] = __cdirent.__dname;

        __curr = (__curr + __next);
        __i++;
    }

    return __rnames;
}

uint32 __kstd_ext2_path_to_inode(char *__path)
{
    int i = 0;

    uint32 __linode = 2;

    BOOL __found = FALSE;

    int __start = 0;
    int __end   = 0;

    if (!__kstd_strcmp(__path, "/"))
    {
        return 2;
    }

    while (__path[i])
    {
        if (__path[i] == '/' && __found == FALSE)
        {
            __start = i;

            __found = TRUE;
        }
        else if (__path[i] == '/' && __found == TRUE)
        {
            __end = i;

            char *__lname = kstd_mem_malloc(__end - __start);

            int __y = 0;

            for (int x = __start + 1; x < __end; x++)
            {
                __lname[__y] = __path[x];

                __y++;
            }

            __start = (__end - 1);

            __linode = __kstd_ext2_find_in_dir(__linode, __lname);
        }

        i++;
    }

    return __linode;
}

void __kstd_ext2_getdata(char *__data, uint32 __len, uint32 __start, uint32 *__sender)
{
    for (int i = 0; i < __len; i++)
    {
        uint32 __index = (i % 4);

        if (__index == 0)
        {
            __data[i] = __sender[i + __start - 3 * (i / 4)] & 0xFF;
        }
        else if (__index == 1)
        {
            __data[i] = (__sender[i + __start - 1 - 3 * (i / 4)] >> 8) & 0xFF;
        }
        else if (__index == 2)
        {
            __data[i] = (__sender[i + __start - 2 - 3 * (i / 4)] >> 16) & 0xFF;
        }
        else if (__index == 3)
        {
            __data[i] = (__sender[i + __start - 3 - 3 * (i / 4)] >> 24) & 0xFF;
        }
    }
}

char *__kstd_ext2_get_filedata(uint32 __inode_n)
{
    char *__fbuffer = kstd_mem_malloc(sizeof(*__fbuffer));

    __kstd_ext2_inode *__inode = __kstd_ext2_read_inode(__inode_n);

    uint32 *__fdata = kstd_mem_malloc(sizeof(__fdata));
    uint32 __db0    = __inode->__direct_block_pointers[0];

    __kstd_ide_read_sectors(DRIVE, 1, 2 * __db0, __fdata);

    __kstd_ext2_getdata(__fbuffer, 1024, 0, __fdata);

    return __fbuffer;
}

__kstd_ext2_inode *__kstd_ext2_read_inode(uint32 __inode)
{
    uint32 __group = __kstd_ext2_determine_blk_group(__inode) + 1;
    uint32 __index = __kstd_ext2_get_inode_index(__inode);

    __kstd_ext2_bgdt *__inode_bgdt;

    __inode_bgdt = __kstd_ext2_parse_bgdt(__group);

    uint32 __inode_table_start = __inode_bgdt->__inode_table_start;

    uint32 __containing_block = (__index * INODE_SIZE) / (1024 << __kstd_ext2_head_sb->__log_block_size);
    uint32 *__inode_tmp = kstd_mem_malloc(sizeof(__inode_tmp));

    __kstd_ide_read_sectors(DRIVE, 2, 2 * (__inode_table_start + __containing_block), __inode_tmp);

    __kstd_ext2_inode *__inode_info;

    __inode_info = (__kstd_ext2_inode *) ((uint32) __inode_tmp + (__index % (1024 / INODE_SIZE)) * INODE_SIZE);

    uint32 __index_in_block = __inode % 4;

    return __inode_info;
}

__kstd_ext2_bgdt *__kstd_ext2_parse_bgdt(uint32 __gblock)
{
    __kstd_ext2_bgdt *__bgdt;

    uint32 __gblk_start_block = (__gblock - 1) * __kstd_ext2_head_sb->__blocks_per_group + 1;
    uint32 __bgdt_start       = __gblk_start_block + 2;

    uint32 *__bgdt_tmp = kstd_mem_malloc(sizeof(__bgdt));

    return __bgdt;
}

__kstd_ext2_dirent __kstd_ext2_read_dirent(uint32 *__data, uint32 __index)
{
    __kstd_ext2_dirent __dent;

    __dent.__inode       = __data[__index];
    __dent.__dirent_size = __data[__index + 1] & 0xFF;
    __dent.__name_len    = (__data[__index + 1] & 0xFF000) >> 16;

    __kstd_ext2_getdata(__dent.__dname, __dent.__name_len, __index + 2, __data);

    char *__tmp = kstd_mem_malloc(__dent.__name_len + 1);

    kstd_memcpy(__tmp, __dent.__dname, __dent.__name_len);

    __dent.__dname = __tmp;

    return __dent;
}

uint32 __kstd_ext2_get_inode_index(uint32 __inode)
{
    uint32 __index = (__inode - 1) % __kstd_ext2_head_sb->__inodes_per_group;

    return __index;
}

uint32 __kstd_ext2_determine_blk_group(uint32 __inode)
{
    uint32 __gblock = (__inode - 1) / __kstd_ext2_head_sb->__inodes_per_group;

    return __gblock;
}

static int __kstd_ext2_lba_to_block(int __block)
{
    int __ext2block = __block - 1;

    return __ext2block;
}