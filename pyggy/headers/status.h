typedef enum {
	GIT_STATUS_CURRENT = ...,
	GIT_STATUS_INDEX_NEW        = ...,
	GIT_STATUS_INDEX_MODIFIED   = ...,
	GIT_STATUS_INDEX_DELETED    = ...,
	GIT_STATUS_INDEX_RENAMED    = ...,
	GIT_STATUS_INDEX_TYPECHANGE = ...,
	GIT_STATUS_WT_NEW           = ...,
	GIT_STATUS_WT_MODIFIED      = ...,
	GIT_STATUS_WT_DELETED       = ...,
	GIT_STATUS_WT_TYPECHANGE    = ...,
	GIT_STATUS_IGNORED          = ...,
} git_status_t;

typedef int (*git_status_cb)(
	const char *path, unsigned int status_flags, void *payload);

int git_status_foreach(
	git_repository *repo,
	git_status_cb callback,
	void *payload);

typedef enum {
	GIT_STATUS_SHOW_INDEX_AND_WORKDIR = ...,
	GIT_STATUS_SHOW_INDEX_ONLY = ...,
	GIT_STATUS_SHOW_WORKDIR_ONLY = ...,
	GIT_STATUS_SHOW_INDEX_THEN_WORKDIR = ...,
} git_status_show_t;

typedef enum {
	GIT_STATUS_OPT_INCLUDE_UNTRACKED      = ...,
	GIT_STATUS_OPT_INCLUDE_IGNORED        = ...,
	GIT_STATUS_OPT_INCLUDE_UNMODIFIED     = ...,
	GIT_STATUS_OPT_EXCLUDE_SUBMODULES     = ...,
	GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS = ...,
	GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH = ...,
	GIT_STATUS_OPT_RECURSE_IGNORED_DIRS   = ...,
} git_status_opt_t;

#define GIT_STATUS_OPT_DEFAULTS ...

typedef struct {
	unsigned int      version;
	git_status_show_t show;
	unsigned int      flags;
	git_strarray      pathspec;
} git_status_options;

#define GIT_STATUS_OPTIONS_VERSION ...

int git_status_foreach_ext(
	git_repository *repo,
	const git_status_options *opts,
	git_status_cb callback,
	void *payload);

int git_status_file(
	unsigned int *status_flags,
	git_repository *repo,
	const char *path);

int git_status_should_ignore(
	int *ignored,
	git_repository *repo,
	const char *path);
