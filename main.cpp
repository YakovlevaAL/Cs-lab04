#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <windows.h>

#include "histogram.h"
#include "svg.h"

using namespace std;

vector<double>
input_numbers(istream& in, size_t count) {
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        in >> result[i];
    }
    return result;
}

Input
read_input(istream& in, bool promt) {
    Input data;

    if(promt)
    cerr << "Enter number count: ";

    size_t number_count;
    in >> number_count;

    if(promt)
    cerr << "Enter numbers: ";

    data.numbers = input_numbers(in, number_count);

    if(promt)
    cerr << "Enter column count: ";


    in >> data.bin_count;

    return data;
}

size_t
write_data(void* items, size_t item_size, size_t item_count, void* ctx) {

    size_t data_size = item_size * item_count;

    stringstream* buffer = reinterpret_cast<stringstream*>(ctx);
    buffer->write(reinterpret_cast<const char*>(items), data_size);

    return data_size;
}

int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow){
double result;
    result=100*dlnow/dltotal;
    if (dltotal=0) {
       result=0;
    }
    cerr << "progress: " << result << "%" << endl;
    return CURL_PROGRESSFUNC_CONTINUE;
}

Input
download(const string& address)
{
    stringstream buffer;

    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curl = curl_easy_init();
    if(curl)
    {

        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
          curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);



        /* Perform the request */
        res = curl_easy_perform(curl);

        if(res)
        {
            cout <<  curl_easy_strerror(res);
            exit(1);
        }

         curl_easy_cleanup(curl);
    }


    return read_input(buffer, false);
}


int main(int argc, char* argv[])
{
    Input input;

    if (argc > 1)
    {
        input = download(argv[1]);
    }

    else
    {
         input = read_input(cin, true);
    }
    const auto bins = make_histogram(input);
    show_histogram_svg(bins);

    return 0;
}
