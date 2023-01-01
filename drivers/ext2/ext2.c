#include "ext2.h"

#include "../ide/ide.h"
#include "../console/console.h"

#include "../../lib/memory.h"
#include "../../lib/stdio.h"

#define DRIVE 0

__kstd_ext2_superblock *__kstd_ext2_head;

void __kstd_ext2_read_superblock(void)
{
    unsigned int *__sb = kstd_mem_malloc(sizeof(*__sb));

    __kstd_ide_read_sectors(DRIVE, 1, 2, __kstd_ext2_head);
}

int __kstd_ext2_lba_to_block(int __bnlba)
{
    int __eblock = __bnlba - 1;

    return __eblock;
}

unsigned int __kstd_ext2_determine_blk_group(unsigned int __inode)
{
    unsigned int __bgroup = (__inode - 1) / __kstd_ext2_head->__inodes_per_group;

    return __bgroup;
}

unsigned int __kstd_ext2_get_inode_index(unsigned int __inode)
{
    unsigned int __index = (__inode - 1) % __kstd_ext2_head->__inodes_per_group;

    return __index;
}

__kstd_ext2_bgdt *__kstd_ext2_parse_bgdt(unsigned int __gblock)
{
    __kstd_ext2_bgdt *__bgdt;

    unsigned int __gblock_start = (__gblock - 1) * __kstd_ext2_head->__blocks_per_group + 1;
    unsigned int __bgdt_start   = __gblock_start + 2;

    unsigned int *__bgdt_tmp = kstd_mem_malloc(sizeof(__bgdt));

    __kstd_ide_read_sectors(DRIVE, 1, __bgdt_start + 1, __bgdt);

    return __bgdt;
}

void __kstd_ext2_get_inode_type(uint32 __type){
	switch (__type)
    {
        case INODE_TYPE_FIFO:
            kstd_write("\nInode Type: FIFO");

            break;
        case INODE_TYPE_CHAR_DEV:
            kstd_write("\nInode Type: Character Device");

            break;
        case INODE_TYPE_DIR:
            kstd_write("\nInode Type: Directory");

            break;
        case INODE_TYPE_BLK_DEV:
            kstd_write("\nInode Type: Block Device");

            break;
        case INODE_TYPE_FILE:
            kstd_write("\nInode Type: Regular File");

            break;
        case INODE_TYPE_SYMB_LINK:
            kstd_write("\nInode Type: Symbolic Link");

            break;
        case INODE_TYPE_UNIX_SOCKET:
            kstd_write("\nInode Type: Unix Socket");

            break;

        default:
            kstd_write("\nUnknown Inode Type");	

            break;	
    }
}

__kstd_ext2_inode *__kstd_ext2_read_inode(unsigned int __inode)
{
    unsigned int __group = __kstd_ext2_determine_blk_group(__inode) + 1;
    unsigned int __index = __kstd_ext2_get_inode_index(__inode);

    __kstd_ext2_bgdt *__inode_bgdt;

    __inode_bgdt = __kstd_ext2_parse_bgdt(__group);

    unsigned int __inode_table_start = __inode_bgdt->__inode_table_start;
    unsigned int __containing_block  = (__index * INODE_SIZE) / (1024 << __kstd_ext2_head->__log_block_size);

    unsigned int *__inode_tmp = kstd_mem_malloc(sizeof(__inode_tmp));

    __kstd_ide_read_sectors(DRIVE, 2, 2 * (__inode_table_start + __containing_block), __inode_tmp);

    __kstd_ext2_inode *__inode_info;

    __inode_info = (__kstd_ext2_inode *) ((unsigned int) __inode_tmp + (__index % (1024 / INODE_SIZE)) * INODE_SIZE);

    return __inode_info;
}

void __kstd_ext2_getdata(char *__data, unsigned int __len, unsigned int __start, unsigned int *__sender)
{
    for (int i = 0; i < __len; i++)
    {
        unsigned int __index = (i % 4);

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

__kstd_ext2_dirent __kstd_ext2_read_dirent(unsigned int *__data, unsigned int __index)
{
    __kstd_ext2_dirent __dirent;

    __dirent.__inode       = __data[__index] & 0xFF;
    __dirent.__dirent_size = __data[__index + 1] & 0xFF;
    __dirent.__name_len    = (__data[__index + 1] & 0x0FF000) >> 16;

    __kstd_ext2_getdata(__dirent.__dname, __dirent.__name_len, __index + 2, __data);

    char *__tmp = kstd_mem_malloc(__dirent.__name_len + 1);

    kstd_memcpy(__tmp, __dirent.__dname, __dirent.__name_len);

    __dirent.__dname = __tmp;

    return __dirent;
}

char **__kstd_ext2_flist(unsigned int __ninode)
{
    char **__rnames = kstd_mem_malloc(sizeof(**__rnames));

    __kstd_ext2_inode *__inode = __kstd_ext2_read_inode(__ninode);

    unsigned int *__dir = kstd_mem_malloc(sizeof(*__dir));
    unsigned int __db0  = __inode->__direct_block_pointers[0];

    __kstd_ide_read_sectors(DRIVE, 6, 2 * __db0, __dir);

    int __curr = 0;
    int __next = 0;
    
    int i = 0;

    while (1)
    {
        __kstd_ext2_dirent __dcurr;

        __dcurr = __kstd_ext2_read_dirent(__dir, __curr);

        if (__dcurr.__inode == 0)
        {
            break;
        }

        __next = __dcurr.__dirent_size / 4;

        __rnames[i] = __dcurr.__dname;

        __curr = __curr + __next;

        i++;
    }

    return __rnames;
}

unsigned int __kstd_ext2_find_in_dir(unsigned int __ninode, char *__dname)
{
    unsigned int __rinode = ERRNO_FNF;

    __kstd_ext2_inode *__inode = __kstd_ext2_read_inode(__ninode);

    unsigned int *__dir = kstd_mem_malloc(sizeof(*__dir));
    unsigned int __db0  = __inode->__direct_block_pointers[0];

    __kstd_ide_read_sectors(DRIVE, 6, 2 * __db0, __dir);

    int __curr = 0;
    int __next = 0;
    
    int i = 0;

    while (1)
    {
        __kstd_ext2_dirent __dcurr;

        __dcurr = __kstd_ext2_read_dirent(__dir, __curr);

        if (__dcurr.__inode == 0)
        {
            break;
        }

        __next = __dcurr.__dirent_size / 4;

        if (!__kstd_strcmp(__dcurr.__dname, __dname))
        {
            __rinode = __dcurr.__inode;
        }

        __curr = __curr + __next;

        i++;
    }

    return __rinode;
}

char *__kstd_ext2_get_filedata(uint32 __ninode)
{
    char *__fbuf = kstd_mem_malloc(sizeof(*__fbuf));

    __kstd_ext2_inode *__inode = __kstd_ext2_read_inode(__ninode);

    unsigned int *__fdata = kstd_mem_malloc(sizeof(__fdata));
    unsigned int __db0    = __inode->__direct_block_pointers[0];

    __kstd_ide_read_sectors(DRIVE, 1, 2 * __db0, __fdata);

    __kstd_ext2_getdata(__fbuf, 1024, 0, __fdata);

    return __fbuf;
}


uint32 __kstd_ext2_path_to_inode(char* path){
	int i = 0;
	uint32 last_inode = 2;
	BOOL found = FALSE;
	int start = 0;
	int end = 0;
	if(!__kstd_strcmp(path, "/"))
		return 2;

	while(path[i]){
		if(path[i] == '/' && found == FALSE){
			start = i;
			found = TRUE;	
		}
		// Parsing Path
		else if(path[i] == '/' && found == TRUE){
			end = i;
			char* lookup_name = kstd_mem_malloc(end - start);
			int y = 0;
			for(int x = start + 1; x < end; x++){
				lookup_name[y] = path[x];
				y++;
			}
			start = end;
			last_inode = __kstd_ext2_find_in_dir(last_inode, lookup_name);
		}
		// This is the last name
		else if(i == __kstd_strlen(path) - 1 && found == TRUE){
			end = i;
			char* lookup_name = kstd_mem_malloc(end - start);
			int y = 0;
			for(int x = start + 1; x <= end; x++){
				lookup_name[y] = path[x];
				y++;
			}
			start = end - 1;
			last_inode = __kstd_ext2_find_in_dir(last_inode, lookup_name);
		}
		i++;
	}
	return last_inode;
}

char* __kstd_ext2_read_file(char* fpath){
	char* file_buf = kstd_mem_malloc(sizeof(*file_buf));

	uint32 file_inode = __kstd_ext2_path_to_inode(fpath);
	file_buf = __kstd_ext2_get_filedata(file_inode);

	return file_buf;
}

void __kstd_ext2_rewrite_bgds(uint32 group, __kstd_ext2_bgdt new_bgdt){
	uint32* data = kstd_mem_malloc(sizeof(*data));
	
    kstd_memcpy(data, &new_bgdt, sizeof(new_bgdt));
	
    uint32 blk_group_start_block = (group - 1) * __kstd_ext2_head->__blocks_per_group + 1;
	uint32 bgdt_start = blk_group_start_block + 2;
	
    __kstd_ide_write_sectors(DRIVE, 1, bgdt_start + 1, data);
}

void __kstd_ext2_init(){
	if(__kstd_ext2_head->__magic != 0xEF53){
		kstd_write("Filesystem is Not EXT2!\n");
	}
	else {
		kstd_write("Filesystem is EXT2!\n");
	}
}
