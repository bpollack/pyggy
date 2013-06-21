int git_revparse_single(git_object **out, git_repository *repo, const char *spec);
int git_revparse_ext(
	git_object **object_out,
	git_reference **reference_out,
	git_repository *repo,
	const char *spec);
typedef enum {
	GIT_REVPARSE_SINGLE         = ...,
	GIT_REVPARSE_RANGE          = ...,
	GIT_REVPARSE_MERGE_BASE     = ...,
} git_revparse_mode_t;
typedef struct {
	git_object *from;
	git_object *to;
	unsigned int flags;
} git_revspec;
int git_revparse(
		git_revspec *revspec,
		git_repository *repo,
		const char *spec);
