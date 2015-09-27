static DIR* finddir(char *path)
{
    DIR *dirp;
    if ((dirp = opendir(path)) == NULL) {
        printf("Cannot find path '%s'", path);
        perror(" ");
        exit(-1);
    }
    return dirp;
}

static Fileattr* listdircontent(char *path, Fileattr *fileattr)
{
    struct dirent *dp;
    struct tm* clock;
    struct tm filedate;
    struct stat attrib;
    char filename[25];
    int nbcount = 0;

    DIR *dirp = finddir(path);

    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            // TODO : check if the object is a file / if it is a directory => errno != 0
            FILE *f = fopen(dp->d_name, "r");
            if(errno == 0)
            {
                fclose(f);
                stat(dp->d_name, &attrib);
                clock = gmtime(&(attrib.st_ctime));

                // save filename in char[]
                strcpy(filename, dp->d_name);

                if (findinlist(fileattr, filename) == 0)
                {
                    // add desc of the file
                    Fileattr *file = (Fileattr *) malloc (sizeof(struct fileattributs));
                    strcpy(file->name, filename);
                    file->day = clock->tm_mday;
                    filedate.tm_mday = clock->tm_mday;
                    file->month = clock->tm_mon+1;
                    filedate.tm_mon = clock->tm_mon;
                    filedate.tm_year = clock->tm_year;
                    file->year = clock->tm_year;
                    filedate.tm_hour = (clock->tm_hour+3+24)%24;
                    file->hour = (clock->tm_hour+3+24)%24;
                    filedate.tm_min = clock->tm_min;
                    file->minute = clock->tm_min;
                    filedate.tm_sec = clock->tm_sec;
                    file->second = clock->tm_sec;
                    //file->cdate = ctime(&(attrib.st_ctime));
                    file->cdate = mktime(&filedate);
                    file->next = fileattr;
                    fileattr = file;

                    printf("File : '%s' ==> added\n", file->name);

                }
                /*
                else
                    printf("File : '%s' ==> already exist\n", filename);
                */
            }
            /*
            else
                printf("'%s' is not a file\n", dp->d_name);
            */

        }

    }
    while (dp != NULL);

    closedir(dirp);
    return fileattr;
}

static void showfilelist(Fileattr *filelist)
{
    if (filelist != NULL)
    {
        printf("file : '%s', \t\t last_modified : |%d-%d-%d %d:%d:%d|\t %s", filelist->name,
           filelist->day, filelist->month, filelist->year,
           filelist->hour, filelist->minute, filelist->second,
           ctime(&filelist->cdate));

           showfilelist(filelist->next);
    }
}

int findinlist(Fileattr *filelist, char nametofind[])
{
    if (filelist != NULL)
    {
        if (strcmp(filelist->name, nametofind) == 0)
            return 1;
        else
            return findinlist(filelist->next, nametofind);
    }

    return 0;
}

/*
static void differencebetweenfilescdate(char *filename1, char *filename2)
{
    Fileattr *file1;
    Fileattr *file2;

    file1->name = filename1;
    file2->name = filename2;

    printf("The file1 name is : '%s'.\n", file1->name);
    printf("The file2 name is : '%s'.\n", file2->name);
}
*/
