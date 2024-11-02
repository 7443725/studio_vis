#include "temp_api_h.h"    
FILE* file_in = 0;
FILE* file_out = 0;
int16_t static_year[3] = { 0 };

int menu(int ar, char** arr)
{
    int i = 1,d = 0;
    if (!memcmp(arr[i], "-h", 2))
    {
       help();
       return 0;
    }
    if (!memcmp(arr[i], "-f", 2))
    {
        i++;
        if (i < ar)
        {
            if (file_open_r(arr[i])) 
            {
                i++;
                if (i < ar)
                {
                    if (!memcmp(arr[i], "-m", 2))
                    {
                        i++;
                        if (i < ar)
                        {
                            sscanf(arr[i],"%d",&d);
                            if (d > 12 || d <= 0)
                                return 0;
                            else
                                return d;
                        }
                        return 0;
                    }
                    if (!memcmp(arr[i], "-o", 2))
                    {
                        i++;
                        if (i < ar)
                        {
                            sscanf(arr[i], "%d", &d);
                            if (d > 12 || d <= 0)
                                return 0;
                            else
                                return ( -1 * d);
                        }
                        return -13;
                    }
                }
                return 13;  
            }
            return 0;
        }

    }
    return 0;
}

uint8_t checking_file_cmd(const char* arr)
{
    uint16_t n = strlen(arr);
    char file_data[100] = {"\0"};
    for (int c = n; c ; c--)
    {
        if (arr[c] == '.')
        {
            int j = 0;
            for (; arr[c] != '\0'; j++)
            {
                c++;
                file_data[j] = arr[c];
            }
            file_data[j] = '\0';
            if (!memcmp(file_data, "csv", 3))
            {
                return 1;
            }
            break;
        }
    }   
    return 0;
}

void help()
{
    printf("Temp statistic application. Pleas enter key: \n");
    printf("-h for help. \n");
    printf("-f file_name(.csv) for load this file.\n");
    printf("-m xx statistic for xx month. \n");
    printf("-o otput data in out_static(.csv). \n");
    printf("Primer: -f file_name(.csv) (statistic YAER) or -m 2 (statistic MOUNTH 2) \n");
    printf("Primer: -f file_name(.csv) -o (statistic YAER) or -o 2 (statistic MOUNTH 2) in out_static(.txt) \n");
}

uint8_t file_open_r(char* arr)
{
    if (checking_file_cmd(arr))
    {
        if ((file_in = fopen(arr, "r")) == NULL)
        {
            perror("Error occured while opening input file! \n");
            return 0;
        }
        return 1;
    }
    printf("Error opening input file! \n");
    return 0;
}

uint8_t file_open_w(char* arr)
{
        if ((file_out = fopen(arr, "w")) == NULL)
        {
            perror("Error occured while opening output file! \n");
            return 0;
        }
        return 1;
}

bool err_data_temp(char* p)
{
    int i = 0;
    while (*(p + i))
    {
        if (*(p + i) == '-' || *(p + i) == ' ' || *(p + i) >= '0' && *(p + i) <= '9' || *(p + i) == '\n')
            i++;
        else
            return true;
    }
    return false;
}

void get_data_file(uint8_t d, file* p)
{
    fseek(file_in, 0, SEEK_SET);
    char row[100];
    char* token;
    bool fl_err_temp = false;
    uint8_t fl_struct = 0, mounth = 0;
    char temp = 0;
    while (feof(file_in) != true)
    {
        fgets(row, sizeof(row), file_in);
        token = strtok(row, ";");
        while (token != NULL)
        {
            switch (fl_struct)
            {
            case 0:
            {
                p->year = atoi(token);
                token = strtok(NULL, ";");
            }
            break;
            case 1:
            {
                mounth = atoi(token);
                token = strtok(NULL, ";");
            }
            break;
            case 2:
            {
                token = strtok(NULL, ";");
                token = strtok(NULL, ";");
                token = strtok(NULL, ";");
                fl_err_temp = err_data_temp(token);
                temp = atoi(token);
                token = strtok(NULL, ";");
            }
            break;
            default:
                break;
            };
            fl_struct++;
            if (mounth != d && fl_struct == 2)
                break;
        }
        fl_struct = 0;
        if (mounth == d && !fl_err_temp)
        {
            if (!p->dayCount)
            {
                p->maxtemp = temp;
                p->mintemp = temp;
            }
            p->mounth = mounth;
            if (p->maxtemp < temp)
                p->maxtemp = temp;
            if (p->mintemp > temp)
                p->mintemp = temp;
            p->Awgtemp += temp;
            p->dayCount++;
        }
        if (mounth == d && fl_err_temp)
        {
            uint8_t i = 0;
            p->str_err[i++] = p->ERRCount + 1;
            if (i > 9)
            {
                printf("Max ERROR file \n");
                return;
            }
            fl_err_temp = false;
        }
        if (mounth > d)
            return;
        p->ERRCount++;
    }
}

void print_month(file* p)
{
    static uint8_t num = 0;
    uint8_t ernum = 0;
    while (p->str_err[ernum] != 0)
    {
        if (p->dayCount == 0)
        {
            printf("  Error temp fail ..... .csv str = %d \n", p->str_err[ernum]);
            return;
        }
        ernum++;
        if (ernum > 9)
        {
            printf("Max ERROR file \n");
            return;
        }
    }
    if (!num)
        printf("  #  Year Month Nuvalue Ervalue MonthAvg MonthMax MonfMin \n");
    int32_t n = (int32_t)p->Awgtemp / (int32_t)p->dayCount;
    printf(" %2d %5d %3d %8d %6d %7d %7d %8d \n", num, p->year, p->mounth, p->dayCount, ernum, n, p->maxtemp, p->mintemp);
    if (p->mounth == 1)
    {
        static_year[0] = n;
        static_year[1] = p->mintemp;
        static_year[2] = p->maxtemp;
    }
    else
    {
        static_year[0] += n;
        if (static_year[1] > p->mintemp)
            static_year[1] = p->mintemp;
        if (static_year[2] < p->maxtemp)
            static_year[2] = p->maxtemp;
    }
    num++;
}

void fprint_month(file* p)
{
    static uint8_t num = 0;
    uint8_t ernum = 0;
    while (p->str_err[ernum] != 0)
    {
        if (p->dayCount == 0)
        {
            fprintf(file_out,"  Error temp fail ..... .csv str = %d \n", p->str_err[ernum]);
            return;
        }
        ernum++;
        if (ernum > 9)
        {
            printf("Max ERROR file \n");
            return;
        }
    }
    if (!num)
        fprintf(file_out,"  #  Year Month Nuvalue Ervalue MonthAvg MonthMax MonfMin \n");
    int32_t n = (int32_t)p->Awgtemp / (int32_t)p->dayCount;
    fprintf(file_out," %2d %5d %3d %8d %6d %7d %7d %8d \n", num, p->year, p->mounth, p->dayCount, ernum, n, p->maxtemp, p->mintemp);
    if (p->mounth == 1)
    {
        static_year[0] = n;
        static_year[1] = p->mintemp;
        static_year[2] = p->maxtemp;
    }
    else
    {
        static_year[0] += n;
        if (static_year[1] > p->mintemp)
            static_year[1] = p->mintemp;
        if (static_year[2] < p->maxtemp)
            static_year[2] = p->maxtemp;
    }
    num++;
}