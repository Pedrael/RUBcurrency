#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "curl/curl.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::find;

size_t size = 0;

size_t writeToString(void *ptr, size_t size, size_t count, void *stream) {
  ((string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}

int main(int argc, char **argv) {
    //cout << system("curl -v --silent https://www.cbr-xml-daily.ru/daily_json.js --stderr - | grep -A 5 -e USD -e EUR") << endl;
    
    CURL *curl;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        //curl_easy_setopt(curl, CURLOPT_URL, "https://api.ratesapi.io/api/latest");
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily_json.js");
        string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */ 
        curl_easy_cleanup(curl);
        
        if(argc > 1) {
            string delimiter = " ";
        
            size_t pos = 0;
            string token;
            while ((pos = response.find(delimiter)) != string::npos) 
            {
                token += response.substr(0, pos);
                response.erase(0, pos + delimiter.length());
            }
            
            size_t curPos = token.find(argv[1]);
            if (curPos == string::npos) {
                cout << "Wrong parameter!" << endl;
                return 0;
            }
            string area = token.substr(curPos + 5, 128);
            size_t valPos = area.find("Value");
            string currency = area.substr(valPos + 7, 7);
            cout << currency << endl;
        }
    }
    else {
        std::cerr << "Curl init failed!" << endl;
        return -1;
    }
    curl_global_cleanup();
    return 0;
}
