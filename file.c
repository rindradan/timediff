typedef struct fileattributs
{
    char name[25];
    struct tm tmdate;
    time_t cdate;
    struct fileattributs *next;

} Fileattr;
