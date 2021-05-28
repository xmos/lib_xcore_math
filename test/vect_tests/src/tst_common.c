// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include "tst_common.h"

#include <stdio.h>
#include <assert.h>


void print_warns(
    int start_case, 
    unsigned test_c, 
    unsigned test_asm)
{
    if(start_case != -1 && start_case != 0){
        printf("\t\t\t\t\t\t************************************************\n");
        printf("\t\t\t\t\t\t***** WARNING: Test not started on case 0 ******\n");
        printf("\t\t\t\t\t\t************************************************\n");
    }
    if(!test_c){
        printf("\t\t\t\t\t\t************************************************\n");
        printf("\t\t\t\t\t\t***** WARNING: Not testing C implementation ****\n");
        printf("\t\t\t\t\t\t************************************************\n");
    }
    if(!test_asm){
        printf("\t\t\t\t\t\t************************************************\n");
        printf("\t\t\t\t\t\t***** WARNING: Not testing ASM implementation **\n");
        printf("\t\t\t\t\t\t************************************************\n");
    }
}