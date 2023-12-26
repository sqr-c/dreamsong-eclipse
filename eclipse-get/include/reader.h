#ifdef UNIX
#ifndef EID_SEARCH_DIRS
#define EID_SEARCH_DIRS	".",
#endif

char dir_eid_process(const char* dir);
#else
#ifndef EID_SEARCH_DIRS
#define EID_SEARCH_DIRS	".",
#endif

char dir_eid_process(const char* dir);
#endif

void sruname(char* buf, const char* str);