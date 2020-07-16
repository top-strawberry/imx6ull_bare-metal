//
// Created by Administrator on 2020/7/12 0012.
//

#ifndef __MAIN_H
#define __MAIN_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "core_ca7.h"
#include "usr_common.h"


#define DEBUG_PRINTF 1

#if DEBUG_PRINTF
#define PRINTF(_f, ...) printf(_f, ## __VA_ARGS__)
#else
#define PRINTF(_f, ...)
#endif


typedef struct Usr_app Usr_app;
#define TOP_INFO Usr_app

#endif //IMAX6ULL_MAIN_H





