#include <strdio.h>

void getplugins(char *fullpath)
{
	// Make sure the path to dlopen has a slash, for it to consider it
	// an actual filesystem path and not just a lookup name.
	char * slashedpath;
	
	sprintf(slashedpath("./%s", fullpath));

	// Attempt to open the plugin DSO
	void* libhandle = dlopen(dstring_cstr(slashedpath), RTLD_NOW);
	dstring_free(slashedpath);
	if (!libhandle) {
    		printf("Error loading DSO: %s\n", dlerror());
    		return NULL;
	}
}
