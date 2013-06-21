#define GIT_PATH_LIST_SEPARATOR ...
#define GIT_PATH_MAX ...
#define GIT_OID_HEX_ZERO ...

void git_libgit2_version(int *major, int *minor, int *rev);

typedef enum {
	GIT_CAP_THREADS = ...,
	GIT_CAP_HTTPS = ...,
} git_cap_t;

int git_libgit2_capabilities(void);

typedef enum {
	GIT_OPT_GET_MWINDOW_SIZE = ...,
	GIT_OPT_SET_MWINDOW_SIZE = ...,
	GIT_OPT_GET_MWINDOW_MAPPED_LIMIT = ...,
	GIT_OPT_SET_MWINDOW_MAPPED_LIMIT = ...,
	GIT_OPT_GET_SEARCH_PATH = ...,
	GIT_OPT_SET_SEARCH_PATH = ...,
	GIT_OPT_SET_CACHE_OBJECT_LIMIT = ...,
	GIT_OPT_SET_CACHE_MAX_SIZE = ...,
	GIT_OPT_ENABLE_CACHING = ...,
	GIT_OPT_GET_CACHED_MEMORY = ...,
} git_libgit2_opt_t;

int git_libgit2_opts(int option, ...);
