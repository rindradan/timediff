#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include "file.c"
#include "filelistener.c"

static DIR* finddir(char *);
static Fileattr* listdircontent(char *, Fileattr *);
static void showfilelist(Fileattr *);
int findinlist(Fileattr *, char []);
static void differencebetweenfilescdate(char *, char *);

int main(int argc, char *argv[])
{
    /*
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);
    */

    Fileattr *files = NULL;

    files = listdircontent(argv[1], files);
    files = listdircontent(argv[1], files);

    printf("\n");
    showfilelist(files);

    return (0);
}
