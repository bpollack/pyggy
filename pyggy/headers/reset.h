typedef enum {
	GIT_RESET_SOFT  = ...,
	GIT_RESET_MIXED = ...,
	GIT_RESET_HARD  = ...,
} git_reset_t;

int git_reset(
	git_repository *repo, git_object *target, git_reset_t reset_type);

int git_reset_default(
    git_repository *repo,
    git_object *target,
    git_strarray* pathspecs);
