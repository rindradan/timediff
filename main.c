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

DIR* finddir(char *);
static Fileattr* listdircontent(char *, Fileattr *);
void showfilelist(Fileattr *);
int findinlist(Fileattr *, char []);
Fileattr* inserttete(Fileattr *, Fileattr *);
Fileattr* insertplace(Fileattr *, Fileattr *);
Fileattr* insertinplace(Fileattr *, Fileattr *, Fileattr *, Fileattr *);

int main(int argc, char *argv[])
{
    printf("%s\n", argv[1]);
    //printf("%s\n", argv[2]);

    Fileattr *files = NULL;
    int i = 0;

    for ( ; i < 50000 ; i++)
    {
        files = listdircontent(argv[1], files);
    }
    //files = listdircontent(argv[1], files);

    printf("\n\n");
    printf("**********************************************************************************************************************\n");
    showfilelist(files);

    return (0);
}
