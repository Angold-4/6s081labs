#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char const *path, char const *target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
	fprintf(2, "find: cannot open %s\n", path);
	exit(1);
    }

    if (fstat(fd, &st) < 0) {
	// fstat system call -> retrieves information from the inode that a file descriptor refers to
	fprintf(2, "find: cannot stat %s\n", path);
	exit(1);
    }

    switch(st.type) {
	case T_FILE:
	    fprintf(2, "Usage: find dir file\n");
	    exit(1);
	case T_DIR:
	    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
		printf("find: path too long\n");
		break;
	    }

	    strcpy(buf, path);
	    p = buf + strlen(buf);
	    // printf("%s\n", p);
	    *p++ = '/';
	    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
		// read from a dirtionary fd, which contains all fds inside it
		if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
		memmove(p, de.name, DIRSIZ); // copy name to p (will overlapping)

		p[DIRSIZ] = 0;

		if (stat(buf, &st) < 0) {
		    // just like fstat
		    printf("find: cannot stat %s\n", buf);
		    continue;
		}

		if (st.type == T_DIR) {
		    find(buf, target);
		} else if (st.type == T_FILE) {
		    if (strcmp(de.name, target) == 0) {
			// find one of our target
			printf("%s\n", buf);
		    }
		}

	    }
	    break;
    }
    close(fd);

}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
	fprintf(2, "Usage: find dir file\n");
	exit(1);
    }

    char const *path = argv[1];
    char const *target = argv[2];
    find(path, target);
    exit(0);
}
