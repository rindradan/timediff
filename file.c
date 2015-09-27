typedef struct fileattributs
{
    char name[25];
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
    struct tm tmdate;
    time_t cdate;
    struct fileattributs *next;

} Fileattr;
