CPSC 457 - Spring 2016 - Assignment 1
=====================================

A "graphical ls" program that recursively prints all files and folders in a
directory, along with their filetype, size, and MD5 checksum. Symbolic links
will also be listed with their relative and absolute paths.

Example output
--------------

| A (regular file - 302 - 87086e42b2fb69077955ecc75cf01f6b)  
| B (regular file - 42239 - 3cac0309369dd4fccf0b1ac23b091c9a)  
| C (directory - 16770)  
   | D (regular file - 8192 - b2c930a89fc6b5d8334e9ba4af75428c)  
   | E (regular file - 0 - a3d01d479bf1906e8bdc6d0d6f196ef5)  
---| F (directory - 0)  
      | H (symbolic link - points to '../D', absolute path : '/home/cliff/teaching/457/dir_asgt/C/D')  
---| G (directory - 8578)  
      | H (regular file - 8221 - d9c2dbfb8e588b41f7b2a77800115d62)  
      | I (regular file - 57 - 6150fe5c4271c87e90a6bd105363c39a)  
      | J (regular file - 300 - d9cd94e7f7ea05530c0c92166dc96484)  
      | printer_port (fifo (named pipe))  
      | chr_dev (character special device)  
      | my_printer (symbolic link - points to 'system_printer', absolute path : '/dev/printer')  
      | blk_dev (block special device)  
      | system_printer (symbolic link - points to '/dev/printer', absolute path : '/dev/printer')  
| gls (regular file - 2125 - f016e215ece68365d2d4d1f6fd22d1fe)  
| SRC.ar (regular file - 17232 - 390bf6593cdb990acc9facdb3377c3a6)  
| gls.c (regular file - 3982 - 11fbf99649d98436c5589d6afee63c56)  
| typical_run (regular file - 32981 - e51e4699504ad688be0a19bd539a8e75)  
| gls.c.orig (regular file - 4218 - 192de841a1eed04093658b47928bea13)  
| NOTES (regular file - 1332 - 6978ff49cb9ae5c077542816f38a14b3)  

Usage
-----
`gls [parameters] [directory]`

Parameters
----------
`-a   Includes hidden files and folders.`

Compilation
-----------

Simply run

./make

in the directory containing the make file and gls.c


Citations
---------

_Directory Pointer_

https://www.gnu.org/software/libc/manual/html_node/Opening-a-Directory.html

_Directory Entries_

https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html

_Finding relative and absolute paths for symbolic links_

http://www.gnu.org/software/libc/manual/html_node/Symbolic-Links.html

_stat system call and struct_

http://man7.org/linux/man-pages/man2/stat.2.html

_stat attributes_

http://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html

_MD5 Hashing in C_

https://www.openssl.org/docs/manmaster/crypto/md5.html

_Announciation of Linux by Linus Torvalds_

http://www.thelinuxdaily.com/2010/04/the-first-linux-announcement-from-linus-torvalds/
