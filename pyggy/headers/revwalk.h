#define GIT_SORT_NONE ...
#define GIT_SORT_TOPOLOGICAL ...
#define GIT_SORT_TIME ...
#define GIT_SORT_REVERSE ...

int git_revwalk_new(git_revwalk **out, git_repository *repo);
void git_revwalk_reset(git_revwalk *walker);
int git_revwalk_push(git_revwalk *walk, const git_oid *id);
int git_revwalk_push_glob(git_revwalk *walk, const char *glob);
int git_revwalk_push_head(git_revwalk *walk);
int git_revwalk_hide(git_revwalk *walk, const git_oid *commit_id);
int git_revwalk_hide_glob(git_revwalk *walk, const char *glob);
int git_revwalk_hide_head(git_revwalk *walk);
int git_revwalk_push_ref(git_revwalk *walk, const char *refname);
int git_revwalk_hide_ref(git_revwalk *walk, const char *refname);
int git_revwalk_next(git_oid *out, git_revwalk *walk);
void git_revwalk_sorting(git_revwalk *walk, unsigned int sort_mode);
int git_revwalk_push_range(git_revwalk *walk, const char *range);
void git_revwalk_free(git_revwalk *walk);
git_repository * git_revwalk_repository(git_revwalk *walk);
