#include <stdio.h>
#include <cbuoy.h>

int main(int argc, char *argv[])
{
    ndbc_data_t bdata;

    bdata.station_id = 46042;
    printf("hello world\n");
    cbuoy_get_data(&bdata);
}
