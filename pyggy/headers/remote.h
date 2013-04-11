typedef int (*git_remote_rename_problem_cb)(const char *problematic_refspec, void *payload);

int git_remote_create(
		git_remote **out,
		git_repository *repo,
		const char *name,
		const char *url);

int git_remote_create_inmemory(
		git_remote **out,
		git_repository *repo,
		const char *fetch,
		const char *url);

int git_remote_load(git_remote **out, git_repository *repo, const char *name);
int git_remote_save(const git_remote *remote);
const char * git_remote_name(const git_remote *remote);
const char * git_remote_url(const git_remote *remote);
const char * git_remote_pushurl(const git_remote *remote);
int git_remote_set_url(git_remote *remote, const char* url);
int git_remote_set_pushurl(git_remote *remote, const char* url);
int git_remote_set_fetchspec(git_remote *remote, const char *spec);
const git_refspec * git_remote_fetchspec(const git_remote *remote);
int git_remote_set_pushspec(git_remote *remote, const char *spec);
const git_refspec * git_remote_pushspec(const git_remote *remote);
int git_remote_connect(git_remote *remote, git_direction direction);
int git_remote_ls(git_remote *remote, git_headlist_cb list_cb, void *payload);

int git_remote_download(
		git_remote *remote,
		git_transfer_progress_callback progress_cb,
		void *payload);

int git_remote_connected(git_remote *remote);
void git_remote_stop(git_remote *remote);
void git_remote_disconnect(git_remote *remote);
void git_remote_free(git_remote *remote);
int git_remote_update_tips(git_remote *remote);
int git_remote_valid_url(const char *url);
int git_remote_supported_url(const char* url);
int git_remote_list(git_strarray *out, git_repository *repo);
void git_remote_check_cert(git_remote *remote, int check);

void git_remote_set_cred_acquire_cb(
	git_remote *remote,
	git_cred_acquire_cb cred_acquire_cb,
	void *payload);

int git_remote_set_transport(
	git_remote *remote,
	git_transport *transport);

typedef enum git_remote_completion_type {
	GIT_REMOTE_COMPLETION_DOWNLOAD,
	GIT_REMOTE_COMPLETION_INDEXING,
	GIT_REMOTE_COMPLETION_ERROR,
} git_remote_completion_type;

struct git_remote_callbacks {
	unsigned int version;
	void (*progress)(const char *str, int len, void *data);
	int (*completion)(git_remote_completion_type type, void *data);
	int (*update_tips)(const char *refname, const git_oid *a, const git_oid *b, void *data);
	void *payload;
};

#define GIT_REMOTE_CALLBACKS_VERSION ...

int git_remote_set_callbacks(git_remote *remote, git_remote_callbacks *callbacks);
const git_transfer_progress * git_remote_stats(git_remote *remote);

typedef enum {
	GIT_REMOTE_DOWNLOAD_TAGS_UNSET,
	GIT_REMOTE_DOWNLOAD_TAGS_NONE,
	GIT_REMOTE_DOWNLOAD_TAGS_AUTO,
	GIT_REMOTE_DOWNLOAD_TAGS_ALL
} git_remote_autotag_option_t;

git_remote_autotag_option_t git_remote_autotag(git_remote *remote);

void git_remote_set_autotag(
	git_remote *remote,
	git_remote_autotag_option_t value);

int git_remote_rename(
	git_remote *remote,
	const char *new_name,
	git_remote_rename_problem_cb callback,
	void *payload);

int git_remote_update_fetchhead(git_remote *remote);
void git_remote_set_update_fetchhead(git_remote *remote, int value);
int git_remote_is_valid_name(const char *remote_name);
