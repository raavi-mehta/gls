#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <openssl/md5.h>

void printHierarchy(DIR *, char *, int, int);
DIR * getdir(char *);
long getDirSize(DIR *dir, char *path, int a_flag);
void getIndent(char *dest, int level, int isDir);
void getFileType(char *, unsigned char);
long getFileSize(const char *path);
int getMD5(const char *path, unsigned char *dest);

int main(int argc, char **argv) {

	// Checks for correct usage
	if (argc < 2 || (!strcmp(argv[1], "-a") && argc < 3)) {
		printf("usage: \'gls -<flags> <directory_name>\'\n");
		exit(1);
	}
	
	// Check if '-a' parameter was included
	int a_flag = 0;
	char cwd[PATH_MAX + 1];
	if (argc > 2 && !strcmp(argv[1], "-a")) {
		a_flag = 1;
		strcpy(cwd, argv[2]);
	}
	else strcpy(cwd, argv[1]);

	// Print hierarchy
	DIR *dir = getdir(cwd);
	if (!dir) return -1;
	printf("\n");
	printHierarchy(dir, cwd, 0, a_flag);
	printf("\n");
	closedir(dir);
	return 0;
}

// Recursively prints the filesystem hierarchy
void printHierarchy(DIR *dir, char *path, int level, int a_flag) {
	for (struct dirent *entry = readdir(dir); entry; entry = readdir(dir)) {

		// Always skip "." and ".." files
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

		// Skip "hidden" files if a_flag is false
		if ((entry->d_name)[0] == '.' && !a_flag) continue;

		// Get path of directory entry
		char new_path[PATH_MAX + 1];
    		size_t size = snprintf(new_path, sizeof(new_path) - 1, "%s/%s", path, entry->d_name);
    		new_path[size] = '\0';

    		// get file type of directory entry
		char type[32];
		getFileType(type, entry->d_type);

		// Get indent
		char indent[1024] = "";
		getIndent(indent, level, entry->d_type == DT_DIR);

		// If entry is a symbolic link, this is printed
		if (entry->d_type == DT_LNK) {
    		char pointsTo[PATH_MAX + 1]; // What link points to
    		char absolutePath[PATH_MAX + 1]; // Absolute path of link
    		if (readlink(new_path, pointsTo, PATH_MAX) == -1 || !realpath(new_path, absolutePath))
    			printf("%s| %s (%s - broken link)\n", indent, entry->d_name, type);
			else 
				printf("%s| %s (%s - points to \'%s\', absolute path : \'%s\')\n", 
					indent, entry->d_name, type, pointsTo, absolutePath);
			continue;
		}
		
		// Print information of directory entry
		if (entry->d_type != DT_DIR) {
			printf("%s| %s (%s", indent, entry->d_name, type);
			if (entry->d_type == DT_REG) {

				long fsize = getFileSize(new_path);
				printf(" - %ld", fsize);

				// Get MD5 checksum
				unsigned char checksum[MD5_DIGEST_LENGTH];
				getMD5(new_path, checksum);

				// Print checksum in hexadecimal
				printf(" - ");
				for (int i = 0; i < MD5_DIGEST_LENGTH; i++) 
					printf("%02x", checksum[i]);
			}
			printf(")\n");
		}

		// If current directory entry is a directory, make a recursive call
		if (entry->d_type == DT_DIR) {
			DIR *newdir = getdir(new_path);
			if (!newdir) continue;
			long dirSize = getDirSize(newdir, new_path, a_flag);
			printf("%s| %s (directory - %ld)\n", indent, entry->d_name, dirSize);
			closedir(newdir);
			newdir = getdir(new_path);
			printHierarchy(newdir, new_path, level + 1, a_flag); // recursive call
		}
	}
}

// Recursively gets sum the size of all 'regular files' in a directory
long getDirSize(DIR *dir, char *path, int a_flag) {
	long totalSize = 0;
	for (struct dirent *entry = readdir(dir); entry; entry = readdir(dir)) {

		// Always skip "." and ".." files
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

		// Skip "hidden" files if a_flag is false
		if ((entry->d_name)[0] == '.' && !a_flag) continue;

		// Get path of directory entry
		char new_path[PATH_MAX + 1];
    	size_t size = snprintf(new_path, sizeof(new_path) - 1, "%s/%s", path, entry->d_name);
    	new_path[size] = '\0';

    	// Get size of 'regular files'
		if (entry->d_type == DT_REG) totalSize += getFileSize(new_path);

		// If current directory entry is a directory, make a recursive call
		if (entry->d_type == DT_DIR) {
			DIR *newdir = getdir(new_path);
			if (!newdir) continue;
			totalSize += getDirSize(newdir, new_path, a_flag);
		}
	}
	return totalSize;
}

// Returns the directory stream for a given path. If permission to view directory is denied,
// or directory does not exist, a null pointer is returned
DIR * getdir(char *cwd) {
	DIR *dir = NULL;
	if (!(dir = opendir(cwd))) {
		fprintf(stderr, "Error: Could not access directory \"%s\"\n", cwd);
		return 0;
	}
	return dir;
}

// Creates the indent based the directory entry type and the level
void getIndent(char *dest, int level, int isDir) {
	if (level > 0) {
		for (int i = 0; i < level; i++) {
			if ((i == level - 1) && isDir) strcat(dest, "---");
			else strcat(dest, "   ");
		}
	}
}

// Calculates MD5 checksum of a file
int getMD5(const char *path, unsigned char *dest) {

	// Attempt to access file
	FILE *fp = NULL;
	if (!(fp = fopen(path, "rb"))) {
		fprintf(stderr, "Could not access %s\n", path);
		return 0;
	}

	// Get file size
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	// Read data from file
	unsigned char data[size];
	fread(data, size, 1, fp);

	// Compute MD5 checksum
	MD5(data, size, dest);
	fclose(fp);
	return 1;
}

long getFileSize(const char *path) {
	// Attempt to access file
	FILE *fp = NULL;
	if (!(fp = fopen(path, "rb"))) {
		fprintf(stderr, "Could not access %s\n", path);
		return 0;
	}

	// Get file size
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	fclose(fp);
	return size;
}

// Store the type of file using the dirent d_type
void getFileType(char *type, unsigned char d_type) {
	
	switch (d_type) {
		
		case DT_UNKNOWN:
			strcpy(type, "unknown file");
			break;
			
		case DT_REG:
			strcpy(type, "regular file");
			break;
		
		case DT_DIR:
			strcpy(type, "directory");
			break;
			
		case DT_FIFO:
			strcpy(type, "fifo (named pipe)");
			break;
			
		case DT_SOCK:
			strcpy(type, "local-domain socket");
			break;
			
		case DT_CHR:
			strcpy(type, "character device");
			break;
			
		case DT_BLK:
			strcpy(type, "block device");
			break;
			
		case DT_LNK:
			strcpy(type, "symbolic link");
			break;
			
		default:
			strcpy(type, "unknown file");
			break;
	}
}
