int git_repository_open(git_repository **out, const char *path);
int git_repository_wrap_odb(git_repository **out, git_odb *odb);

int git_repository_discover(
		char *path_out,
		size_t path_size,
		const char *start_path,
		int across_fs,
		const char *ceiling_dirs);

typedef enum {
	GIT_REPOSITORY_OPEN_NO_SEARCH = ...,
	GIT_REPOSITORY_OPEN_CROSS_FS  = ...,
} git_repository_open_flag_t;

int git_repository_open_ext(
	git_repository **out,
	const char *path,
	unsigned int flags,
	const char *ceiling_dirs);

void git_repository_free(git_repository *repo);

int git_repository_init(
	git_repository **out,
	const char *path,
	unsigned is_bare);

typedef enum {
	GIT_REPOSITORY_INIT_BARE              = ...,
	GIT_REPOSITORY_INIT_NO_REINIT         = ...,
	GIT_REPOSITORY_INIT_NO_DOTGIT_DIR     = ...,
	GIT_REPOSITORY_INIT_MKDIR             = ...,
	GIT_REPOSITORY_INIT_MKPATH            = ...,
	GIT_REPOSITORY_INIT_EXTERNAL_TEMPLATE = ...,
} git_repository_init_flag_t;

typedef enum {
	GIT_REPOSITORY_INIT_SHARED_UMASK = ...,
	GIT_REPOSITORY_INIT_SHARED_GROUP = ...,
	GIT_REPOSITORY_INIT_SHARED_ALL   = ...,
} git_repository_init_mode_t;

typedef struct {
	unsigned int version;
	uint32_t    flags;
	uint32_t    mode;
	const char *workdir_path;
	const char *description;
	const char *template_path;
	const char *initial_head;
	const char *origin_url;
} git_repository_init_options;

int git_repository_init_ext(
	git_repository **out,
	const char *repo_path,
	git_repository_init_options *opts);

int git_repository_head(git_reference **out, git_repository *repo);
int git_repository_head_detached(git_repository *repo);
int git_repository_head_orphan(git_repository *repo);
int git_repository_is_empty(git_repository *repo);
const char * git_repository_path(git_repository *repo);
const char * git_repository_workdir(git_repository *repo);
int git_repository_set_workdir(
	git_repository *repo, const char *workdir, int update_gitlink);
int git_repository_is_bare(git_repository *repo);
int git_repository_config(git_config **out, git_repository *repo);
void git_repository_set_config(git_repository *repo, git_config *config);
int git_repository_odb(git_odb **out, git_repository *repo);
void git_repository_set_odb(git_repository *repo, git_odb *odb);
int git_repository_refdb(git_refdb **out, git_repository *repo);
void git_repository_set_refdb(
	git_repository *repo,
	git_refdb *refdb);
int git_repository_index(git_index **out, git_repository *repo);
void git_repository_set_index(git_repository *repo, git_index *index);
int git_repository_message(char *out, size_t len, git_repository *repo);
int git_repository_message_remove(git_repository *repo);
int git_repository_merge_cleanup(git_repository *repo);

typedef int (*git_repository_fetchhead_foreach_cb)(const char *ref_name,
	const char *remote_url,
	const git_oid *oid,
	unsigned int is_merge,
	void *payload);

int git_repository_fetchhead_foreach(git_repository *repo,
	git_repository_fetchhead_foreach_cb callback,
	void *payload);

typedef int (*git_repository_mergehead_foreach_cb)(const git_oid *oid,
	void *payload);

int git_repository_mergehead_foreach(git_repository *repo,
	git_repository_mergehead_foreach_cb callback,
	void *payload);

int git_repository_hashfile(
    git_oid *out,
    git_repository *repo,
    const char *path,
    git_otype type,
    const char *as_path);

int git_repository_set_head(
	git_repository* repo,
	const char* refname);

int git_repository_set_head_detached(
	git_repository* repo,
	const git_oid* commitish);

int git_repository_detach_head(
	git_repository* repo);

typedef enum {
	GIT_REPOSITORY_STATE_NONE,
	GIT_REPOSITORY_STATE_MERGE,
	GIT_REPOSITORY_STATE_REVERT,
	GIT_REPOSITORY_STATE_CHERRY_PICK,
	GIT_REPOSITORY_STATE_BISECT,
	GIT_REPOSITORY_STATE_REBASE,
	GIT_REPOSITORY_STATE_REBASE_INTERACTIVE,
	GIT_REPOSITORY_STATE_REBASE_MERGE,
	GIT_REPOSITORY_STATE_APPLY_MAILBOX,
	GIT_REPOSITORY_STATE_APPLY_MAILBOX_OR_REBASE,
} git_repository_state_t;

int git_repository_state(git_repository *repo);
