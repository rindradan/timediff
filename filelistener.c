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

Fileattr* inserttete(Fileattr *filelist, Fileattr *filetoinserttete)
{
    filetoinserttete->next = filelist;
    return filetoinserttete;
}

void showfilelist(Fileattr *filelist)
{
    if (filelist != NULL)
    {
        printf("file : '%s', \t\t last_modified : |%d-%d-%d %d:%d:%d|\t %s", filelist->name,
           filelist->tmdate.tm_mday, filelist->tmdate.tm_mon, filelist->tmdate.tm_year,
           filelist->tmdate.tm_hour, filelist->tmdate.tm_min, filelist->tmdate.tm_sec,
           ctime(&filelist->cdate));

           if (filelist->next != NULL)
           {
               double difference = difftime(filelist->next->cdate, filelist->cdate);
               printf("DIFFERENCE = %10.2lf sec\n", difference);
           }

           showfilelist(filelist->next);
    }
}

Fileattr* insertinplace(Fileattr *filelist, Fileattr *filetoinsert, Fileattr *fileprevious, Fileattr *filefinal)
{
    double difference = 0;

    if (filelist != NULL)
    {
        difference = difftime(filelist->cdate, filetoinsert->cdate);
        if (difference > 0)
        {
            filetoinsert->next = filelist;
            if (fileprevious != NULL)
                fileprevious->next = filetoinsert;
        }
        else
        {
            insertinplace(filelist->next, filetoinsert, filelist, filefinal);
            return filefinal;
        }
    }
    else
    {
        if (fileprevious != NULL)
        {
            filetoinsert->next = filelist;
            fileprevious->next = filetoinsert;
            return filefinal;
        }
        else
        {
            return inserttete(filelist, filetoinsert);
        }
    }

    return fileprevious;
}

Fileattr* insertplace(Fileattr *filelist, Fileattr *filetoinsert)
{
    return insertinplace(filelist, filetoinsert, NULL, filelist);
}

static Fileattr* listdircontent(char *path, Fileattr *fileattr)
{
    struct dirent *dp;
    struct tm* clock;
    struct tm filedate;
    struct stat attrib;

    DIR *dirp = finddir(path);

    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            char filenamepath[200] = "";
            strcat(filenamepath, path);
            strcat(filenamepath, dp->d_name);

            // check if the object is a file / if it is a directory => errno != 0
            FILE *f = fopen(dp->d_name, "r");
            if(errno == 0)
            {
                fclose(f);
                stat(dp->d_name, &attrib);
                clock = gmtime(&(attrib.st_ctime));

                // save filename in char[]
                char filename[200];
                strcpy(filename, dp->d_name);

                if (findinlist(fileattr, filename) == 0)
                {
                    // add desc of the file
                    Fileattr *file = (Fileattr *) malloc (sizeof(struct fileattributs));
                    strcpy(file->name, filename);
                    filedate.tm_mday = clock->tm_mday;
                    filedate.tm_mon = clock->tm_mon;
                    filedate.tm_year = clock->tm_year;
                    filedate.tm_hour = (clock->tm_hour+3+24)%24;
                    filedate.tm_min = clock->tm_min;
                    filedate.tm_sec = clock->tm_sec;
                    file->tmdate = filedate;
                    //file->cdate = ctime(&(attrib.st_ctime));
                    file->cdate = mktime(&filedate);

                    //fileattr = insertplace(fileattr, file);

                    fileattr = insertplace(fileattr, file);

                    printf("File : '%s' ==> added\n", file->name);

                    printf("\n\n");
                    printf("**********************************************************************************************************************\n");
                    showfilelist(fileattr);


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
