gls
===
gls is a "graphical ls" program that recursively prints all files and folders in a
directory, along with their filetype, size, and MD5 checksum. Symbolic links
will also be listed with their relative and absolute paths.

Example Output
--------------
![Alt text](example.png?raw=true "Title")

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
