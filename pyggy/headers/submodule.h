typedef struct git_submodule git_submodule;
typedef enum {
	GIT_SUBMODULE_UPDATE_DEFAULT = ...,
	GIT_SUBMODULE_UPDATE_CHECKOUT = ...,
	GIT_SUBMODULE_UPDATE_REBASE = ...,
	GIT_SUBMODULE_UPDATE_MERGE = ...,
	GIT_SUBMODULE_UPDATE_NONE = ...,
} git_submodule_update_t;
typedef enum {
	GIT_SUBMODULE_IGNORE_DEFAULT = ...,
	GIT_SUBMODULE_IGNORE_NONE = ...,
	GIT_SUBMODULE_IGNORE_UNTRACKED = ...,
	GIT_SUBMODULE_IGNORE_DIRTY = ...,
	GIT_SUBMODULE_IGNORE_ALL = ...,
} git_submodule_ignore_t;
typedef enum {
	GIT_SUBMODULE_STATUS_IN_HEAD           = ...,
	GIT_SUBMODULE_STATUS_IN_INDEX          = ...,
	GIT_SUBMODULE_STATUS_IN_CONFIG         = ...,
	GIT_SUBMODULE_STATUS_IN_WD             = ...,
	GIT_SUBMODULE_STATUS_INDEX_ADDED       = ...,
	GIT_SUBMODULE_STATUS_INDEX_DELETED     = ...,
	GIT_SUBMODULE_STATUS_INDEX_MODIFIED    = ...,
	GIT_SUBMODULE_STATUS_WD_UNINITIALIZED  = ...,
	GIT_SUBMODULE_STATUS_WD_ADDED          = ...,
	GIT_SUBMODULE_STATUS_WD_DELETED        = ...,
	GIT_SUBMODULE_STATUS_WD_MODIFIED       = ...,
	GIT_SUBMODULE_STATUS_WD_INDEX_MODIFIED = ...,
	GIT_SUBMODULE_STATUS_WD_WD_MODIFIED    = ...,
	GIT_SUBMODULE_STATUS_WD_UNTRACKED      = ...,
} git_submodule_status_t;

#define GIT_SUBMODULE_STATUS__IN_FLAGS ...
#define GIT_SUBMODULE_STATUS__INDEX_FLAGS ...
#define GIT_SUBMODULE_STATUS__WD_FLAGS ...
int git_submodule_lookup(
	git_submodule **submodule,
	git_repository *repo,
	const char *name);
int git_submodule_foreach(
	git_repository *repo,
	int (*callback)(git_submodule *sm, const char *name, void *payload),
	void *payload);
int git_submodule_add_setup(
	git_submodule **submodule,
	git_repository *repo,
	const char *url,
	const char *path,
	int use_gitlink);
int git_submodule_add_finalize(git_submodule *submodule);
int git_submodule_add_to_index(
	git_submodule *submodule,
	int write_index);
int git_submodule_save(git_submodule *submodule);
git_repository * git_submodule_owner(git_submodule *submodule);
const char * git_submodule_name(git_submodule *submodule);
const char * git_submodule_path(git_submodule *submodule);
const char * git_submodule_url(git_submodule *submodule);
int git_submodule_set_url(git_submodule *submodule, const char *url);
const git_oid * git_submodule_index_id(git_submodule *submodule);
const git_oid * git_submodule_head_id(git_submodule *submodule);
const git_oid * git_submodule_wd_id(git_submodule *submodule);
git_submodule_ignore_t git_submodule_ignore(
	git_submodule *submodule);
git_submodule_ignore_t git_submodule_set_ignore(
	git_submodule *submodule,
	git_submodule_ignore_t ignore);
git_submodule_update_t git_submodule_update(
	git_submodule *submodule);
git_submodule_update_t git_submodule_set_update(
	git_submodule *submodule,
	git_submodule_update_t update);
int git_submodule_fetch_recurse_submodules(
	git_submodule *submodule);
int git_submodule_set_fetch_recurse_submodules(
	git_submodule *submodule,
	int fetch_recurse_submodules);
int git_submodule_init(git_submodule *submodule, int overwrite);
int git_submodule_sync(git_submodule *submodule);
int git_submodule_open(
	git_repository **repo,
	git_submodule *submodule);
int git_submodule_reload(git_submodule *submodule);
int git_submodule_reload_all(git_repository *repo);
int git_submodule_status(
	unsigned int *status,
	git_submodule *submodule);
int git_submodule_location(
	unsigned int *location_status,
	git_submodule *submodule);
