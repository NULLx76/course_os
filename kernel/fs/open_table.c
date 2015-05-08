/**
 * @file Open File Table
 * @author  Ginevra Gaudioso
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This file contains the table of open files, implemented as an array
 * with methods like add_to_opentable and delete_from_opentable to be called in 
 * open and close Since methods are provided, other files should not touch 
 * neither the free LL or the arrray, but just use the premade methods. 
 *
 */

#include "file.h"
#include "klibc.h"
#include "open_table.h"
#include "bitvector.h"
#include <stdint.h>

/**
 * Initialize Open File Table
 *
 * Called by file.c initialization function, initializes free list and table
 */
//
void fs_table_init() {
        open_table_free_list = make_vector(SYSTEM_SIZE); //create bitvector of free indexes
        table = (struct file_descriptor**)kmalloc(SYSTEM_SIZE * (sizeof(struct file_descriptor*))); //malloc the table
	os_memset(table, 0, SYSTEM_SIZE * (sizeof(struct file_descriptor*)));
}

/**
 * Shutdown Open Table
 *
 * Clears out all files currently listed in the table (includes files
 * that users never closed) then, upon shutdown, memory with the free 
 * list is freed
 *
 */
//
void fs_table_shutdown() {
        bv_free(open_table_free_list);
        int i;
        for (i = 0; i < SYSTEM_SIZE; i++) {
                if (table[i] != 0x0) { //deal with users that forget to close files
                        if (table[i]->linked_file != 0x0) { //"if" added because we never know what can happen...
                                kfree(table[i]->linked_file);
                        }
                        kfree(table[i]);
                }
        }
        kfree(table);
}

/**
 * Get file descriptor index from table
 *
 * returns struct of descriptor at index fd
 * if invalid, returns NULL.
 *
 * @param  
 * 
 * int fd - index of file descriptor to get from the table
 *
 * @return 
 * If there is a corresponding file descriptor at the specified index
 * this function returns that descriptor; otherwise returns 0
 *
 */
struct file_descriptor* get_descriptor(int fd){
	if (file_is_open(fd)) {
		return table[fd];
	}
	return 0x0;
}

/**
 * Add an entry to the open table
 *
 * this function can be used to insert a file in the table
 * and returns the requested index if successful, else -1 
 *
 * @param  
 *
 * struct inode* f - the file decriptor to add to the open table
 *
 * @param  
 * 
 * char perm - the permissions belonging to the new entry 
 *
 * @return 
 * 
 * Returns the index to the entry in the open table if file was successfully opened
 * otherwise returns -1
 */
int add_to_opentable(struct inode * f, char perm) {
        int fd = (int) bv_firstFree(open_table_free_list); //gets free index from bitvector
        if (fd == -1) {
                return -1; //reached max num of files open
        }
        bv_set((uint32_t)fd, open_table_free_list); //index is now taken
        struct file_descriptor* to_add = (struct file_descriptor*) kmalloc(sizeof(struct file_descriptor)); //malloc new struct
        int inum = f->inum;
        int i;
        for (i=0; i<SYSTEM_SIZE; i++) {  //checks if file is already open in the table. If so, the linked file is the same. 
                if (table[i] && table[i]->linked_file->inum == inum) {
			os_printf("%d\n",i);
                        to_add->linked_file->fd_refs++; //increment the number of references
                        to_add->linked_file =  table[i]->linked_file;  //point to same file
                        to_add->permission = perm; //assign new permission
                        to_add->offset = 0; //restart offset from 0
                        table[fd] = to_add; //add to table
                        return fd;
                }
        }
        to_add->linked_file = f;
        to_add->linked_file->fd_refs = 1;
	to_add->permission = perm;
        to_add->offset = 0;
        table[fd] = to_add; //add to table
        if (perm == 'a') { //append, need to move offset to very end
                table[fd]->offset = table[fd]->linked_file->size;
        }
        return fd;
}

/**
 * Delete an entry from the open table
 *
 * this function can be used to delete a file from the list
 *
 * @param 
 * 
 *  int fd - index of the file descriptor to remove
 *
 * @return 
 * 
 * Returns 0 if the file descriptor was succefully removed; otherwise
 * returns -1
 */
int delete_from_opentable(int fd) {
        if (!file_is_open(fd)) {
                return -1; //invalid entry
        }
        table[fd]->linked_file->fd_refs--;
        if (table[fd]->linked_file->fd_refs == 0) { //free inode only if was referenced only once
                kfree(table[fd]->linked_file);
        }
        kfree(table[fd]); //free space in table
        bv_lower ((uint32_t)fd, open_table_free_list); //index is not taken anymore
        return 0;
}

/**
 * Checks to see whether the file is open or not
 *
 * @param  
 *
 * int fd - index of the file descriptor to check
 *
 * @return 
 * 
 * Returns 1 if the index exists in the open table, otherwise
 * returns 0
 */
//this function checks whether the file is open or not
int file_is_open(int fd) {
	if (fd<0 || fd>=SYSTEM_SIZE) { 
                return 0; 
        }
	// Doing it this way to avoid uxtb instruction from compiler.
	if (table[fd] == 0x0) {
		return 0;
	}
	return 1;
        //return (table[fd] != 0x0);
}

