//------------------- CMCC tasks implementation --------------------//
//#include "stdafx.h"
#include "ChinaMobileDlg.h"

#include <curl/curl.h>
#include <curl/curloptions.h>
//#include ".\JsonLib\json_cpp\json.h"
//#include ".\JsonLib\json_lib\json.h"
//#include <string> 
#include <vector>

//-------------------------- CurlOptions ---------------------------//

size_t curl_write_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
	curlData *realData = (curlData *)data;

	if (size *nmemb <=0){
		return 0;
	}

	realData->data = (char *)realloc(realData->data, realData->size + size * nmemb);
	memcpy(realData->data + realData->size, (char *)ptr, size * nmemb);
	realData->size += size * nmemb;

	return size * nmemb;
}

curlData *curl_data_init()
{
	curlData *data;
	data = (curlData *)malloc(sizeof(curlData));
	if (data == NULL){
		//printf("[%s]:Malloc Failed\n", __func__);
		return NULL;
	}
	memset(data, 0, sizeof (curlData));

	data->size = 0;
	data->data = NULL;
	
	return data;
}

curlData *curl_http_post(char *url, char *form)
{
	CURL *curl;
	curlData *data;
	
	data = curl_data_init();
	if (data == NULL){
		//printf("[%s]:curl data init failed !\n", __func__);
		goto error00;
	}
	
	curl_global_init(CURL_GLOBAL_ALL);
	if ((curl = curl_easy_init()) == NULL){
		//printf("[%s]:curl easy init failed\n", __func__);
		goto error01;
	}
	
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, form);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(form));

	if (curl_easy_perform(curl)){
		//printf("[%s]:curl easy perform failed\n", __func__);
		goto error02;
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return data;
	
error02:
	if (data->data != NULL){
		free(data->data);
	}
error01:
	free(data);
error00:
	return NULL;

}

curlData *curl_http_get(char *url)
{
	CURL *curl;
	curlData *data;
	
	data = curl_data_init();
	if (data == NULL){
		//printf("[%s]:curl data init failed !\n", __func__);
		goto error00;
	}
	
	curl_global_init(CURL_GLOBAL_ALL);
	if ((curl = curl_easy_init()) == NULL){
		//printf("[%s]:curl easy init failed\n", __func__);
		goto error01;
	}
	
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)data);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	if (curl_easy_perform(curl)){
		//printf("[%s]:curl easy perform failed\n", __func__);
		goto error02;
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return data;
	
error02:
	if (data->data != NULL){
		free(data->data);
	}
error01:
	free(data);
error00:
	return NULL;
}