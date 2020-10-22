#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
//#include "../../include/filecache.h"
//#include "../../include/queue.h"
#include "filecache.h"
#include "queue.h"

size_t write_data(void* ptr, size_t size, size_t nmemb, void* filedata)
{
	DATA* fdata = (DATA*) filedata;
        size_t written;
        //written = fwrite(ptr, size, nmemb, ji->fp);

        return written;

//        bool ranged = fdata->ranged;
        //, http range
  /*      if(ranged)
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
        }*/
}

