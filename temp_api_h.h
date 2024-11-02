#pragma once
# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
//#include <windows.h>

typedef struct file
{
    uint16_t year;
    uint8_t mounth;
    uint32_t dayCount;
    uint32_t ERRCount;
    int8_t maxtemp;
    int8_t mintemp;
    int32_t Awgtemp;
    uint32_t str_err[10];
}file;



int menu(int ar,char **arr);
uint8_t checking_file_cmd(const char* arr);
void help();
uint8_t file_open_r(char* arr);
uint8_t file_open_w(char* arr);
bool err_data_temp(char* p);
void get_data_file(uint8_t d, file* p);
void print_month(file* p);
void fprint_month(file* p);