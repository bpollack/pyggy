int git_ignore_add_rule(
	git_repository *repo,
	const char *rules);

int git_ignore_clear_internal_rules(
	git_repository *repo);

int git_ignore_path_is_ignored(
	int *ignored,
	git_repository *repo,
	const char *path);
