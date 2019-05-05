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
* @brief Implementation of cbuoy library
*
* @author Matthew Tanner
******************************************************************************/
#include <ndbcc.h>

const float BUOYDATA_NOT_AVAIL  = -998;
const float BUOYDATA_ERROR      = -999;
const char *NDBC_REALDATA_URL   = "https://www.ndbc.noaa.gov/data/realtime2/";

int32_t parse_ndbcdata_string ()
{
    return 0;
}


int32_t fetch_ndbcdata_string ()
{
    return 0;
}


int32_t cbuoy_get_data (ndbc_data_t *data)
{
    printf("fetch\n");

    return 0;
}


