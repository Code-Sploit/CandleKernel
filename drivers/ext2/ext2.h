#ifndef EXT2_H
#define EXT2_H

#include "../../lib/stdlib.h"

#define EXT2_SUPER 1

#define INODE_SIZE 256

#define INODE_TYPE_FIFO 1
#define INODE_TYPE_CHAR_DEV 2
#define INODE_TYPE_DIR 0x4
#define INODE_TYPE_BLK_DEV 6
#define INODE_TYPE_FILE 8
#define INODE_TYPE_SYMB_LINK 0xA000
#define INODE_TYPE_UNIX_SOCKET 0xC000

#define ERRNO_FNF 0

typedef struct __kstd_ext2_superblock
{
    uint32 __inodes_count;
	uint32 __blocks_count;
	uint32 __r_blocks_count;
	uint32 __free_blocks_count;	
	uint32 __free_inodes_count;
	uint32 __first_data_block;
	uint32 __log_block_size;
	uint32 __log_frag_size;
	uint32 __blocks_per_group;
	uint32 __frags_per_group;
	uint32 __inodes_per_group;
	uint32 __mtime;
	uint32 __wtime;
	uint16 __mnt_count;
	uint16 __max_mnt_count;
	uint16 __magic;
	uint16 __state;
	uint16 __errors;
	uint16 __minor_rev_level;
	uint32 __lastcheck;
	uint32 __checkinterval;
	uint32 __creator_os;
	uint32 __rev_level;
	uint16 __def_resuid;
	uint16 __def_resgid;
} __kstd_ext2_superblock;

typedef struct __kstd_ext2_bgdt
{
    uint32 __blk_bmap;
    uint32 __inode_bmap;
    uint32 __inode_table_start;
    uint16 __unalloc_blocks;
    uint16 __unalloc_inodes;
    uint16 __num_of_dirs;
    uint16 __pad[7];
} __kstd_ext2_bgdt;

typedef struct __kstd_ext2_inode
{
    unsigned short __type_perm;
    unsigned short __user_id;
    unsigned int __size_low;
    unsigned int __last_access_time;
    unsigned int __creation_time;
    unsigned int __modification_time;
    unsigned int __deletion_time;
    unsigned short __group_id;
    unsigned short __hard_links;
    unsigned int __sector_usage;
    unsigned int __flags;
    unsigned int __OS_type;
    uint32 __direct_block_pointers[12];
    unsigned int __singly_indirect_block_pointer;
    unsigned int __doubly_indirect_block_pointer;
    unsigned int __triply_indirect_block_pointer;
    unsigned int __generation_num;
    unsigned int __extended_attribute_block;
    unsigned int __size_high_dir_acl;
    unsigned int __block_addr_of_fragment;
    unsigned char __os_specific[12];
} __kstd_ext2_inode;

typedef struct __kstd_ext2_dirent
{
    uint32 __inode;
    uint32 __dirent_size;
    uint32 __name_len;
    uint32 __type;

    char *__dname;
} __kstd_ext2_dirent;

uint32 __kstd_ext2_find_in_dir(uint32 __inode_n, char *__dname);

void __kstd_ext2_read_superblock(void);

void __kstd_ext2_init(void);

char *__kstd_ext2_read_file(char *__fpath);

char **__kstd_ext2_flist(uint32 __inode_n);

uint32 __kstd_ext2_path_to_inode(char *__path);

void __kstd_ext2_getdata(char *__data, uint32 __len, uint32 __start, uint32 *__sender);

char *__kstd_ext2_get_filedata(uint32 __inode_n);

__kstd_ext2_inode *__kstd_ext2_read_inode(uint32 __inode);

__kstd_ext2_bgdt *__kstd_ext2_parse_bgdt(uint32 __gblock);

__kstd_ext2_dirent __kstd_ext2_read_dirent(uint32 *__data, uint32 __index);

uint32 __kstd_ext2_get_inode_index(uint32 __inode);

uint32 __kstd_ext2_determine_blk_group(uint32 __inode);

static int __kstd_ext2_lba_to_block(int __block);

#endif