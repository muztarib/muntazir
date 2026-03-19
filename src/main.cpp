#include <iostream>
#include <fstream>
#include <cstring>
#include <curl/curl.h>

using namespace std;

size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    ofstream* file = (ofstream*)stream;
    size_t total = size * nmemb;

    file->write((char*)ptr, total);

    return total;
}

int progress_callback(void* clientp, double total, double now, double, double)
{
    if (total <= 0.0) return 0;

    int percent = (int)((now / total) * 100.0);

    cout << "\rDownloading: " << percent << "%";
    cout.flush();

    return 0;
}

int main()
{
    char url[500];
    char filename[200];

    cout << "Enter URL: ";
    cin.getline(url, 500);

    cout << "Save as: ";
    cin.getline(filename, 200);

    ofstream file(filename, ios::binary);
    if (!file)
    {
        cout << "Error opening file\n";
        return 1;
    }

    CURL* curl = curl_easy_init();
    if (!curl)
    {
        cout << "Curl init failed\n";
        return 1;
    }

    // Core setup
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    // Progress
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);

    cout << "\nStarting download...\n";

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        cout << "\nDownload failed: " << curl_easy_strerror(res) << endl;
    }
    else
    {
        double downloaded_size = 0;
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &downloaded_size);

        cout << "\nDownload completed successfully!\n";
        cout << "Downloaded bytes: " << downloaded_size << endl;
    }

    curl_easy_cleanup(curl);
    file.close();

    return 0;
}