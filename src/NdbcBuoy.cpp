#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>

#include "NdbcBuoy.h"

#define FEET_PER_METER 3.28084

using namespace std;

const string NDBC_URL = "https://www.ndbc.noaa.gov/data/realtime2/";


static size_t WriteCallback(void *data, size_t size, size_t nmemb,
        void *userp)
{
    ((string*)userp)->append((char*)data, size * nmemb);
    
    return size * nmemb;
}


int NdbcBuoy::parseBuoyData(string rawdata, vector<float> &data)
{
    string      line;
    string      colstr;
    uint8_t     column = 0;

    istringstream ss(rawdata); 
    
    /* Data starts on 3rd line */
    getline (ss, line);
    getline (ss, line);
    getline (ss, line);
   
    istringstream ssline(line);
    while ((getline(ssline, colstr, ' ')) && (column < data.size()))
    {
        if (colstr != "")
        {
            try
            {
                data[column] = stof(colstr, NULL);
            }
            catch (std::out_of_range& e)
            {
                data[column] = BUOYDATA_ERROR;
            }
            catch (...) {}
            column ++;
        }
    }

    return 0; 
}

int NdbcBuoy::fetchBuoyData(string *buf, string dataset)
{
    CURL *bdata;
    CURLcode bdata_result;

    string url = NDBC_URL + to_string(this->station) + "." + dataset;
    bdata = curl_easy_init();
    if(bdata) {
        
        curl_easy_setopt(bdata, CURLOPT_URL, url.c_str());
        curl_easy_setopt(bdata, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(bdata, CURLOPT_WRITEDATA, buf);
        
        bdata_result = curl_easy_perform(bdata);
        if(bdata_result) 
        {
            cout << "Error downloading buoy data" << endl;
            return -1;
        }
    
    }
    curl_easy_cleanup(bdata);
   
    return 0;
}


int NdbcBuoy::updateData(int station)
{
    this->station = station;

    return this->updateData();
}

int NdbcBuoy::updateData()
{
    string txtdata_str;
    string specdata_str;
    string cwinddata_str;
 
    /* Update txt */
    if (fetchBuoyData(&txtdata_str, "txt") < 0)
    {
        cout << "Error fetching txt data" << endl;
    }
    parseBuoyData(txtdata_str, this->txt.data);
    
    /* Update spec */
    if (fetchBuoyData(&specdata_str, "spec") < 0)
    {
        cout << "Error fetching txt data" << endl;
    }
    parseBuoyData(specdata_str, this->spec.data);

    /* Update cwind */
    if (fetchBuoyData(&cwinddata_str, "cwind") < 0)
    {
        cout << "Error fetching txt data" << endl;
    }
    parseBuoyData(cwinddata_str, this->cwind.data);

    return 0;
}

NdbcBuoy::NdbcBuoy(int station)
{
    this->station = station;

    fill(this->txt.data.begin(), this->txt.data.end(),
            BUOYDATA_NOT_AVAILABLE);
    fill(this->spec.data.begin(), this->spec.data.end(), 
            BUOYDATA_NOT_AVAILABLE);
    fill(this->cwind.data.begin(), this->cwind.data.end(), 
            BUOYDATA_NOT_AVAILABLE);
}


