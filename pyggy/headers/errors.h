enum {
    GIT_OK = ...,
    GIT_ERROR = ...,
    GIT_ENOTFOUND = ...,
    GIT_EEXISTS = ...,
    GIT_EAMBIGUOUS = ...,
    GIT_EBUFS = ...,
    GIT_EUSER = ...,
    GIT_EBAREREPO = ...,
    GIT_EORPHANEDHEAD = ...,
    GIT_EUNMERGED = ...,
    GIT_ENONFASTFORWARD = ...,
    GIT_EINVALIDSPEC = ...,
    GIT_EMERGECONFLICT = ...,

    GIT_PASSTHROUGH = ...,
    GIT_ITEROVER = ...,
};

typedef struct {
    char *message;
    int klass;
} git_error;

typedef enum {
    GITERR_NOMEMORY,
    GITERR_OS,
    GITERR_INVALID,
    GITERR_REFERENCE,
    GITERR_ZLIB,
    GITERR_REPOSITORY,
    GITERR_CONFIG,
    GITERR_REGEX,
    GITERR_ODB,
    GITERR_INDEX,
    GITERR_OBJECT,
    GITERR_NET,
    GITERR_TAG,
    GITERR_TREE,
    GITERR_INDEXER,
    GITERR_SSL,
    GITERR_SUBMODULE,
    GITERR_THREAD,
    GITERR_STASH,
    GITERR_CHECKOUT,
    GITERR_FETCHHEAD,
    GITERR_MERGE,
} git_error_t;

const git_error * giterr_last(void);
void giterr_clear(void);
void giterr_set_str(int error_class, const char *string);
void giterr_set_oom(void);
