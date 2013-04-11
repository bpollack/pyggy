int git_revparse_single(git_object **out, git_repository *repo, const char *spec);
int git_revparse_rangelike(git_object **left, git_object **right, int *threedots, git_repository *repo, const char *rangelike);
