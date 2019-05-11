#include <stdio.h>
#include <ndbcc.h>

int main(int argc, char *argv[])
{
    ndbc_data_t  bdata;

    bdata.station_id  = 46042;
    bdata.hour_offset = 0;
    ndbcc_get_all_data(&bdata);

    printf("TXT WVHT: %f\n", bdata.txt_data[TXT_DATA_WVHT]);
    printf("SPEC SWH: %f\n", bdata.spec_data[SPEC_DATA_SWH]);
    printf("SPEC STEEPNESS: %f\n", bdata.spec_data[SPEC_DATA_STEEPNESS]);
    printf("CWIND WSPD: %f\n", bdata.cwind_data[CWIND_DATA_WSPD]);
}


