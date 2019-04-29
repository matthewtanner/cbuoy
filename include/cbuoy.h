/******************************************************************************
* MIT License
*
* Copyright (c) 2019 Matthew Tanner
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
****************************************************************************//*
* @file
*
* @brief Interface to cbuoy library
*
* @author Matthew Tanner
******************************************************************************/
#ifndef __CBUOY_H__
#define __CBUOY_H__

#include <stdint.h>
#include <stdio.h>

extern const float BUOYDATA_NOT_AVAILABLE;
extern const float BUOYDATA_ERROR;

typedef enum
{
    TXT_DATA_YY,
    TXT_DATA_MM,
    TXT_DATA_DD,
    TXT_DATA_HR,
    TXT_DATA_MN,
    TXT_DATA_WDIR,
    TXT_DATA_WSPD,
    TXT_DATA_GST,
    TXT_DATA_WVHT,
    TXT_DATA_DPD,
    TXT_DATA_APD,
    TXT_DATA_MWD,
    TXT_DATA_PRES,
    TXT_DATA_ATMP,
    TXT_DATA_WTMP,
    TXT_DATA_DEWP,
    TXT_DATA_VIS,
    TXT_DATA_PTDY,
    TXT_DATA_TIDE,
    NUM_TXT_ELEMENTS
} txt_data_index_t;

typedef enum
{
    SPEC_DATA_YY,
    SPEC_DATA_MM,
    SPEC_DATA_DD,
    SPEC_DATA_HR,
    SPEC_DATA_MN,
    SPEC_DATA_WVHT,
    SPEC_DATA_SWH,
    SPEC_DATA_SWP,
    SPEC_DATA_WWH,
    SPEC_DATA_WWP,
    SPEC_DATA_SWD,
    SPEC_DATA_WWD,
    SPEC_DATA_STEEPNESS,
    SPEC_DATA_APD,
    SPEC_DATA_MWD,
    NUM_SPEC_ELEMENTS
} spec_data_index_t;

typedef enum
{
    CWIND_DATA_YY,
    CWIND_DATA_MM,
    CWIND_DATA_DD,
    CWIND_DATA_HR,
    CWIND_DATA_MN,
    CWIND_DATA_WDIR,
    CWIND_DATA_WSPD,
    CWIND_DATA_GDR,
    CWIND_DATA_GST,
    CWIND_DATA_GTIME,
    NUM_CWIND_ELEMENTS
} cwind_data_index_t;
    
typedef struct
{
    uint16_t station_id; 
    float    txt_data[NUM_TXT_ELEMENTS];
    float    spec_data[NUM_SPEC_ELEMENTS];
    float    cwind_data[NUM_CWIND_ELEMENTS];
        
} ndbc_data_t;

int32_t cbuoy_get_data(ndbc_data_t *data);

#endif /* __CBUOY_H__ */
