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
* @brief Implementation of ndbc-c library
*
* @author Matthew Tanner
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include <ndbcc.h>

const uint32_t	HTTP_RCVBUF_MIN_SIZE  = (120 * 5);
const float		BUOYDATA_NOT_AVAIL	  = -998;
const float		BUOYDATA_ERROR		  = -999;
const uint16_t	SINGLE_QUERY_STR_SIZE = 1024;

const uint8_t  URL_SIZE		= 255;
const char	  *URL_STR_TXT	= "txt";
const char	  *URL_STR_SPEC = "spec";
const char	  *URL_STR_CWIND	 = "cwind";
const char	  *NDBC_REALDATA_URL =
    "https://www.ndbc.noaa.gov/data/realtime2/";

typedef struct
{
    char *ptr;
    size_t len;
} dyn_string_t;


int32_t parse_ndbc_data(ndbc_data_t *ndbc_data, dyn_string_t response_str,
                        ndbc_data_set_t data_set)
{
    float		  *data;
    const uint8_t  MAX_COLUMN_STR_SIZE = 16;
    uint8_t		   num_elements, column, i;
    char		  *str_token, *line;
    char		   tmp_str[MAX_COLUMN_STR_SIZE];
    float		   temp_val;
    char		  *temp1, *temp2, *nptr;

    switch (data_set)
    {
        case NDBC_DATA_SET_TXT:
            num_elements = NUM_TXT_ELEMENTS;
            data = ndbc_data->txt_data;
            break;
        case NDBC_DATA_SET_SPEC:
            num_elements = NUM_SPEC_ELEMENTS;
            data = ndbc_data->spec_data;
            break;
        case NDBC_DATA_SET_CWIND:
            num_elements = NUM_CWIND_ELEMENTS;
            data = ndbc_data->cwind_data;
            break;
        default:
            return -1;
    }

    /* Discard first two line headers */
    line = strtok_r(response_str.ptr, "\n", &temp1);
    line = strtok_r(NULL, "\n", &temp1);

    /* Seek to hour offset */
    for (i= 0; i <= ndbc_data->hour_offset; i++)
    {
        line = strtok_r(NULL, "\n", &temp1);
    }

    /* Parse Columns */
    str_token = strtok_r(line, " ", &temp2);
    column	  = 0;
    while ((str_token != NULL) && (column <= num_elements))
    {
        strncpy(tmp_str, str_token, 16);
        tmp_str[16] = 0;
        temp_val	= strtof(str_token, &nptr);

        /* String values are not handled yet */
        if (nptr == str_token)
        {
            temp_val = BUOYDATA_ERROR;
        }
        data[column] = temp_val;
        str_token	 = strtok_r(NULL, " ", &temp2);
        column++;
    }

    return 0;
}


size_t write_callback(void *data, size_t size, size_t nmemb, void *userptr)
{
    char		 *temp_ptr;
    dyn_string_t *string	= (dyn_string_t*)userptr;
    size_t		  data_size = size * nmemb;

    temp_ptr = realloc(string->ptr, string->len + data_size + 1);
    if (temp_ptr == NULL)
    {
        return 0;
    }

    string->ptr = temp_ptr;
    memcpy(&(string->ptr[string->len]), data, data_size);
    string->len += data_size;
    string->ptr[string->len] = 0;

    return data_size;
}


int32_t build_ndbc_url(char* strbuf, uint16_t station_id,
                       ndbc_data_set_t data_set)
{
    switch (data_set)
    {
        case NDBC_DATA_SET_TXT:
            snprintf(strbuf, URL_SIZE, "%s%u.%s", NDBC_REALDATA_URL,
                     station_id, URL_STR_TXT);
            break;
        case NDBC_DATA_SET_SPEC:
            snprintf(strbuf, URL_SIZE, "%s%u.%s", NDBC_REALDATA_URL,
                     station_id, URL_STR_SPEC);
            break;
        case NDBC_DATA_SET_CWIND:
            snprintf(strbuf, URL_SIZE, "%s%u.%s", NDBC_REALDATA_URL,
                     station_id, URL_STR_CWIND);
            break;
        default:
            return -1;
    }
    return 0;
}


int32_t ndbcc_get_data (ndbc_data_t *ndbc_data, ndbc_data_set_t data_set)
{
    CURL		 *p_curl;
    CURLcode	  res;
    char		  request_url[URL_SIZE];
    dyn_string_t  response_str;
    int32_t		  status;

    response_str.ptr = NULL;
    response_str.len = 0;
    response_str.ptr = malloc(1);
    if (response_str.ptr == NULL)
    {
        return -1;
    }

    status = build_ndbc_url(request_url, ndbc_data->station_id, data_set);
    if (status < 0)
    {
        free(response_str.ptr);
        return -1;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    p_curl = curl_easy_init();
    curl_easy_setopt(p_curl, CURLOPT_URL, request_url);
    curl_easy_setopt(p_curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(p_curl, CURLOPT_WRITEDATA, (void *)&response_str);
    res = curl_easy_perform(p_curl);
    if (res != CURLE_OK)
    {
        printf("res: %d\n", res);
        free(response_str.ptr);
        return -1;
    }
    
    status = parse_ndbc_data(ndbc_data, response_str, data_set);
    free(response_str.ptr);
    return status;
}


int32_t ndbcc_get_all_data (ndbc_data_t *data)
{
    int32_t  status;

    /* Fetch txt data */
    status = ndbcc_get_data(data, NDBC_DATA_SET_TXT);
    if (status < 0)
    {
        printf("Error fetching txt data\n");
    }

    /* Fetch spec data */
    status = ndbcc_get_data(data, NDBC_DATA_SET_SPEC);
    if (status < 0)
    {
        printf("Error fetching txt data\n");
    }

    /* Fetch cwind data */
    status = ndbcc_get_data(data, NDBC_DATA_SET_CWIND);
    if (status < 0)
    {
        printf("Error fetching txt data\n");
    }

    return 0;
}

