static const char GIT_PATH_LIST_SEPARATOR;
#define GIT_PATH_MAX ...
static char *const GIT_OID_HEX_ZERO;

void git_libgit2_version(int *major, int *minor, int *rev);

enum {
    GIT_CAP_THREADS = ...,
    GIT_CAP_HTTPS = ...,
};

int git_libgit2_capabilities(void);

enum {
    GIT_OPT_GET_MWINDOW_SIZE,
    GIT_OPT_SET_MWINDOW_SIZE,
    GIT_OPT_GET_MWINDOW_MAPPED_LIMIT,
    GIT_OPT_SET_MWINDOW_MAPPED_LIMIT,
    GIT_OPT_GET_SEARCH_PATH,
    GIT_OPT_SET_SEARCH_PATH,
    GIT_OPT_GET_ODB_CACHE_SIZE,
    GIT_OPT_SET_ODB_CACHE_SIZE,
};

int git_libgit2_opts(int option, ...);
