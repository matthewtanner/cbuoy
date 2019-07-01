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
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <ndbcc.h>

#define NDBC_HTTP_HEADER_SIZE     128
#define NDBC_RESPONSE_BUFFER_SIZE 16384

const uint32_t	HTTP_RCVBUF_MIN_SIZE  = (120 * 5);
const float		BUOYDATA_NOT_AVAIL	  = -998;
const float		BUOYDATA_ERROR		  = -999;
const uint16_t	SINGLE_QUERY_STR_SIZE = 1024;

const uint8_t  URL_SIZE		        = 255;
const char	  *URL_STR_TXT	        = "txt";
const char	  *URL_STR_SPEC         = "spec";
const char	  *URL_STR_CWIND	    = "cwind";
const char	  *NDBC_REALDATA_URL    = "/data/realtime2/";
const char    *NDBC_HOST            = "www.ndbc.noaa.gov";
const char    *NDBC_REALDATA_PATH   = "/data/realtime2/";

typedef struct
{
    char *ptr;
    size_t len;
} dyn_string_t;


int32_t parse_ndbc_data(ndbc_data_t *ndbc_data, char *response,
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

    /* Discard http response header */
    line = strtok_r(response, "\n", &temp1);
    while (line[0] != '\r')
    {
        line = strtok_r(NULL, "\n", &temp1);
    }

    /* Discard first two line headers in message*/
    line = strtok_r(NULL, "\n", &temp1);
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

int32_t open_connection(void)
{
    struct hostent *hp = NULL;
	struct sockaddr_in addr;
	int on = 1, sockfd;    
    const int PORT = 443;
	
    if((hp = gethostbyname(NDBC_HOST)) == NULL)
	if(hp == NULL)
    {
		perror("gethostbyname");
		exit(1);
	}
	
    memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockfd == -1)
    {
		perror("setsockopt");
        return -1;
	}
	
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
	if(sockfd == -1)
    {
		perror("setsockopt");
		close(sockfd);
        return -1;
	}
	
	if(connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
    {
		perror("connect");
		close(sockfd);
        return -1;
	}
    return sockfd;
}


int32_t https_request_buoydata(char *request_url, ndbc_data_t *ndbc_data,
        char *response)
{
    const int32_t SSL_READ_SIZE = 1024;
    char http_header[NDBC_HTTP_HEADER_SIZE];
    int32_t sockfd;
    uint32_t idx = 0;
    int32_t bytes_read;
    
    sockfd = open_connection();
    if (sockfd < 0)
    {
        printf("Error opening connection\n");
        return -1;
    }

    SSL_load_error_strings ();
    SSL_library_init ();
    
    SSL_CTX *ssl_ctx = SSL_CTX_new (TLSv1_2_client_method ());
    SSL_CTX_set_options(ssl_ctx, SSL_OP_ALL);
    
    SSL *ssl = SSL_new(ssl_ctx);
    SSL_set_tlsext_host_name(ssl, NDBC_HOST);
    SSL_set_fd(ssl, sockfd);
    
    int err = SSL_connect(ssl);
    if (err != 1)
    {
        SSL_CTX_free(ssl_ctx);
        SSL_free(ssl);
        close(sockfd);
        ERR_print_errors_fp(stderr); 
        return -1;
    }

    memset(http_header, 0, NDBC_HTTP_HEADER_SIZE);
    sprintf(http_header, "GET %s HTTP/1.1\r\nHost: %s\r\n"
            "Connection: close\r\n\r\n", request_url, NDBC_HOST);
    
    SSL_write(ssl, http_header, strlen(http_header));  

    memset(response, 0, NDBC_RESPONSE_BUFFER_SIZE);

   	bytes_read = 1;
    while ((bytes_read > 0) && (idx < (NDBC_RESPONSE_BUFFER_SIZE
            - SSL_READ_SIZE -1)))
    {
        bytes_read = SSL_read(ssl, &response[idx], SSL_READ_SIZE - 1);
        idx += bytes_read;
    }

	shutdown(sockfd, SHUT_RDWR); 
	close(sockfd);    
    return sockfd;
}


int32_t build_ndbc_url(char* req_url, uint16_t station_id,
                       ndbc_data_set_t data_set)
{
    switch (data_set)
    {
        case NDBC_DATA_SET_TXT:
            snprintf(req_url, URL_SIZE, "%s%u.%s", NDBC_REALDATA_URL,
                     station_id, URL_STR_TXT);
            break;
        case NDBC_DATA_SET_SPEC:
            snprintf(req_url, URL_SIZE, "%s%u.%s", NDBC_REALDATA_URL,
                     station_id, URL_STR_SPEC);
            break;
        case NDBC_DATA_SET_CWIND:
            snprintf(req_url, URL_SIZE, "%s%u.%s", NDBC_REALDATA_URL,
                     station_id, URL_STR_CWIND);
            break;
        default:
            return -1;
    }
    return 0;
}


int32_t ndbcc_get_data (ndbc_data_t *ndbc_data, ndbc_data_set_t data_set)
{
    char		  request_url[URL_SIZE];
    int32_t		  status;
    char response[NDBC_RESPONSE_BUFFER_SIZE];

    status = build_ndbc_url(request_url, ndbc_data->station_id, data_set);
    if (status < 0)
    {
        printf("Error building request url\n");
        return -1;
    }

    status = https_request_buoydata(request_url, ndbc_data, response);
    if (status < 0)
    {
        printf("Error fetching data\n");
        return -1;
    }
    status = parse_ndbc_data(ndbc_data, response, data_set);
    return status;
}


int32_t ndbcc_get_all_data (ndbc_data_t *data)
{
    int32_t status;

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

