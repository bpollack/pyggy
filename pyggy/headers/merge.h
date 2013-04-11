int git_merge_base(
	git_oid *out,
	git_repository *repo,
	const git_oid *one,
	const git_oid *two);
int git_merge_base_many(
	git_oid *out,
	git_repository *repo,
	const git_oid input_array[],
	size_t length);
