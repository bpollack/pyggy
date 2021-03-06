typedef enum {
	GIT_CHECKOUT_NONE = ...,
	GIT_CHECKOUT_SAFE = ...,
	GIT_CHECKOUT_SAFE_CREATE = ...,
	GIT_CHECKOUT_FORCE = ...,
	GIT_CHECKOUT_ALLOW_CONFLICTS = ...,
	GIT_CHECKOUT_REMOVE_UNTRACKED = ...,
	GIT_CHECKOUT_REMOVE_IGNORED = ...,
	GIT_CHECKOUT_UPDATE_ONLY = ...,
	GIT_CHECKOUT_DONT_UPDATE_INDEX = ...,
	GIT_CHECKOUT_NO_REFRESH = ...,
	GIT_CHECKOUT_DISABLE_PATHSPEC_MATCH = ...,
	GIT_CHECKOUT_SKIP_LOCKED_DIRECTORIES = ...,
	GIT_CHECKOUT_SKIP_UNMERGED = ...,
	GIT_CHECKOUT_USE_OURS = ...,
	GIT_CHECKOUT_USE_THEIRS = ...,
	GIT_CHECKOUT_UPDATE_SUBMODULES = ...,
	GIT_CHECKOUT_UPDATE_SUBMODULES_IF_CHANGED = ...,
} git_checkout_strategy_t;

typedef enum {
	GIT_CHECKOUT_NOTIFY_NONE      = ...,
	GIT_CHECKOUT_NOTIFY_CONFLICT  = ...,
	GIT_CHECKOUT_NOTIFY_DIRTY     = ...,
	GIT_CHECKOUT_NOTIFY_UPDATED   = ...,
	GIT_CHECKOUT_NOTIFY_UNTRACKED = ...,
	GIT_CHECKOUT_NOTIFY_IGNORED   = ...,
	GIT_CHECKOUT_NOTIFY_ALL       = ...,
} git_checkout_notify_t;

typedef int (*git_checkout_notify_cb)(
	git_checkout_notify_t why,
	const char *path,
	const git_diff_file *baseline,
	const git_diff_file *target,
	const git_diff_file *workdir,
	void *payload);

typedef void (*git_checkout_progress_cb)(
	const char *path,
	size_t completed_steps,
	size_t total_steps,
	void *payload);

typedef struct git_checkout_opts {
	unsigned int version;
	unsigned int checkout_strategy;
	int disable_filters;
	unsigned int dir_mode;
	unsigned int file_mode;
	int file_open_flags;
	unsigned int notify_flags;
	git_checkout_notify_cb notify_cb;
	void *notify_payload;
	git_checkout_progress_cb progress_cb;
	void *progress_payload;
	git_strarray paths;
	git_tree *baseline;
	const char *target_directory;
} git_checkout_opts;

#define GIT_CHECKOUT_OPTS_VERSION ...

int git_checkout_head(
	git_repository *repo,
	git_checkout_opts *opts);

int git_checkout_index(
	git_repository *repo,
	git_index *index,
	git_checkout_opts *opts);

int git_checkout_tree(
	git_repository *repo,
	const git_object *treeish,
	git_checkout_opts *opts);
