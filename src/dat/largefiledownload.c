#include <unistd.h> 
#include <libgen.h> 
#include <curl/curl.h> 
#include <string> 
#include <vector> 
#include <map> 
#include "thread_pool.h" 

using namespace std; 
#define HTTP_RANGE 0 
//http range? 
const long seg_size = 20 * 1024 * 1024; 

//100MB, 
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER; 
map<string, int> download_map; 

//, key url, val, 

	//CloseInfo(FILE* f, string& s): fp(f), url(s) 
{

} 
~CloseInfo()
{
} 

public: 
	FILE* fp; 
	string url; 
}; 
vector<CloseInfo*> closed_vec; 
//, /,
//struct
class struct JobInfo
{ 
	JobInfo(): fp(NULL), startPos(0), stopPos(0), ranged(false) 
	{
	} 
	FILE* fp; 



//
long startPos; 
long stopPos; 
string url; 
bool ranged; 
//http range }; 
static void set_JobInfo(JobInfo* ji, FILE* f, long s, long e, string u, bool r)
{ 
	ji->fp = f; 
	ji->startPos = s; 
	ji->stopPos = e; 
	ji->url = u; 
	ji->ranged = r; 
} 
long get_download_file_length (const char *url) 
{ 
	double file_len = 0; 
	CURL *handle = curl_easy_init (); 
	curl_easy_setopt (handle, CURLOPT_URL, url); 
	curl_easy_setopt (handle, CURLOPT_FOLLOWLOCATION, 1L); 
	curl_easy_setopt (handle, CURLOPT_MAXREDIRS, 3L); 
	curl_easy_setopt (handle, CURLOPT_AUTOREFERER, 1L); 
	curl_easy_setopt (handle, CURLOPT_HEADER, 0L); 

	//header
	curl_easy_setopt (handle, CURLOPT_NOBODY, 1L); 

	//body 
	curl_easy_setopt (handle, CURLOPT_FORBID_REUSE, 1); 
	curl_easy_setopt (handle, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36"); 
	//user-agent 
	if (curl_easy_perform (handle) == CURLE_OK) 
	{ 
		curl_easy_getinfo (handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_len); 
	} 
	else 
	{ 
		file_len = -1; 
	} 
	curl_easy_cleanup(handle); 
	return file_len; 
	} 
	//,
	static size_t write_data(void* ptr, size_t size, size_t nmemb, void* userdata)
	{ 
		JobInfo* ji = (JobInfo*) userdata; 
		bool ranged = ji->ranged; 
		size_t written; 
		//, http range
		if(ranged)			
		{ 
			//, 
			pthread_mutex_lock (&g_mutex); 
			if(ji->startPos + size * nmemb <= ji->stopPos)
			{ 
				fseek(ji->fp, ji->startPos, SEEK_SET); 
				written = fwrite(ptr, size, nmemb, ji->fp); 
				ji->startPos += size * nmemb; 
			} 
			else
			{ 
				fseek(ji->fp, ji->startPos, SEEK_SET); 
				written = fwrite(ptr, 1, ji->stopPos - ji->startPos + 1, ji->fp); 
				ji->startPos = ji->stopPos; 
			} 
			pthread_mutex_unlock (&g_mutex); 
		}
		else
		{ 
			written = fwrite(ptr, size, nmemb, ji->fp); 
		} 
		return written; 
	} 

	void* job_process(void* arg)
		{ 
			CURL* curl; 
				CURLcode res; 
			JobInfo* ji = (JobInfo*)arg; 
			char range[64] = 
				{ 0 }; 
				if(ji->ranged) 
				{ 
					snprintf (range, sizeof(range), "%ld-%ld", ji->startPos, ji->stopPos); 
				printf("range: [%s]\n", range); 
				} 
				curl = curl_easy_init(); 
				curl_easy_setopt(curl, CURLOPT_URL, ji->url.c_str()); 
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)ji); 
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); 
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L); 
			curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L); 
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L); 
			curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L); 
		curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 5L); 
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36"); 

	//user-agent curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L); 
	//curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L); 
	if(ji->ranged) curl_easy_setopt(curl, CURLOPT_RANGE, range); 
	res = curl_easy_perform(curl); 
		if(CURLE_OK != res)
		{ 
			printf("[%d] %s\n", res, ji->url.c_str()); 
		} 
		curl_easy_cleanup(curl); 

		//TODO: .... 
		string url = ji->url; 
		pthread_mutex_lock(&g_mutex);
		 download_map[url]--; 
		printf("[-1]%s\n", url.c_str()); 
		pthread_mutex_unlock(&g_mutex); 
		return NULL; 
	} 

	//url
	int get_job_queue (vector<string>& urls, vector<JobInfo*>& jobs)
	{ 
		FILE* fp; 
		long file_len = 0L, start, stop, seg_num; 
		bool ranged = false; 
		JobInfo* ji; 
		vector<string>::iterator it; 
		for(it = urls.begin(); it != urls.end(); ++it)
		{ 
			string url = *it; 
			file_len = get_download_file_length (url.c_str()); 
		if(file_len <= 0) continue; 
			printf("[%ld] %s\n", file_len, url.c_str()); 
		//url, 
		const char* fn = basename((char*)url.c_str()); 
		string full_path(fn); 
		full_path = "./" + full_path; 
		//
	fp = fopen(full_path.c_str(), "wb"); 
	//
	if(NULL == fp) continue; 
		//
	CloseInfo* ci = new CloseInfo(fp, url); 
		closed_vec.push_back(ci); 
		//
		int additional = (file_len % seg_size == 0) ? 0 : 1; 
	int seg_total = ranged ? (file_len < seg_size ? 1 : (file_len/seg_size + additional)) : 1; 
	download_map[url] = seg_total; 
	printf("[+1]%s, seg total %d\n", url.c_str(), seg_total); 

#if HTTP_RANGE 
//, ? 
if(file_len < seg_size)
{ 
	start = 0; stop = file_len - 1; 
	ji = new JobInfo(); 
	set_JobInfo(ji, fp, start, stop, url, ranged); 
	jobs.push_back(ji); 
	} 
	else
	{ 
	//,
	ranged = true; 
	seg_num = (long)file_len / seg_size; 
	printf("filesize[%ld], segsize[%ld], seg num: %ld\n", file_len, seg_size, seg_num); 
	for(int i = 0; i <= seg_num; i++){ 
		if(i < seg_num)
		{ 
			start = i * seg_size; 
			stop = (i + 1) * seg_size - 1; 
		} 
		else
		{ 
			if(file_len % seg_size != 0)
			{ 
				start = i * seg_size; stop = file_len - 1; 
			}
			else 
				break; 
		} 
		ji = new JobInfo(); 
		set_JobInfo(ji, fp, start, stop, url, ranged); 
		jobs.push_back(ji); 
		} 
	} 
	#else 
		start = 0; 
		stop = file_len - 1; 
		ji = new JobInfo(); 
		set_JobInfo(ji, fp, start, stop, url, ranged); 
		jobs.push_back(ji); 
	#endif 
	} 
	return 0; 
} 

int run()
{ 
	vector<string> urls_vec; 
	urls_vec.push_back("http://dlsw.baidu.com/sw-search-sp/soft/da/17519/BaiduYunGuanjia_4.8.3.1409021519.exe");
	//10M // 
	urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04-server-ppc64el.template"); 
	//67M // 
	urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04.1-server-amd64+mac.template"); 
	//88M 
	urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04.1-server-powerpc.template"); 
	//64M // 
urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04.1-server-ppc64el.template"); 
	//67M // 
	urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04-desktop-amd64+mac.iso"); 
	//962M 
	urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04-desktop-amd64+mac.iso.zsync"); 
	//1.9M // 
	urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04-server-amd64+mac.iso"); 
	//566M // 
	urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04-server-amd64+mac.template"); 
	//88M // 
	urls_vec.push_back("http://cdimage.ubuntu.com/releases/14.04/release/ubuntu-14.04-server-powerpc.iso"); 

	//654M 
	//, cpu, 
	CThreadPool* pool = new CThreadPool(30); 
	vector<JobInfo*> ji_vec; 
	get_job_queue(urls_vec, ji_vec); 
	vector<JobInfo*>::iterator it; 
	for(it = ji_vec.begin(); it != ji_vec.end(); ++it)
	{ 
		JobInfo* ji = *it; 
		pool->pool_add_job(job_process, (void*)ji); 
	} 
	//,,worker, master, worker//, 
	//u
	sleep(2000 * 1000 * 1000); 
	//2000 
	map<string, int>::iterator mit; 
	int finished = 0; 
	size_t map_size = download_map.size(); 
	printf("map size %ld\n", map_size); 
	while(1)
	{ 
		finished = 0; 
		pthread_mutex_lock(&g_mutex); 
		for(mit = download_map.begin(); mit != download_map.end(); ++mit) 
			if(!mit->second) finished++; 
			pthread_mutex_unlock(&g_mutex); 
			if(finished == map_size) 
			{ 
				printf("finish all task =====================>\n"); 
				break; 
			} 
			else{ 
				usleep(30 * 1000 * 1000); 
			} 
		} 
		download_map.clear(); 
		delete pool; 
		urls_vec.clear(); 
		
		//jobinfo,.JobInfo,. //,
		ji_vec.clear(); 

		//,
		vector<CloseInfo*>::iterator cit; 
		for(cit = closed_vec.begin(); cit != closed_vec.end(); ++cit)
		{ 
			CloseInfo* ci = *cit; 
			if(ci->fp != NULL)
			{ 
				fclose(ci->fp); 
				ci->fp = NULL; 
			} 
			printf("[closed] %s\n", ci->url.c_str()); 
			delete ci; 
		} 
		closed_vec.clear(); 
		return 0; 
	
	}
} 

