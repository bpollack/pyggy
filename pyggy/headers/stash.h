typedef enum {
	GIT_STASH_DEFAULT = ...,

	/* All changes already added to the index
	 * are left intact in the working directory
	 */
	GIT_STASH_KEEP_INDEX = ...,

	/* All untracked files are also stashed and then
	 * cleaned up from the working directory
	 */
	GIT_STASH_INCLUDE_UNTRACKED = ...,

	/* All ignored files are also stashed and then
	 * cleaned up from the working directory
	 */
	GIT_STASH_INCLUDE_IGNORED = ...,
} git_stash_flags;

int git_stash_save(
	git_oid *out,
	git_repository *repo,
	git_signature *stasher,
	const char *message,
	unsigned int flags);

typedef int (*git_stash_cb)(
	size_t index,
	const char* message,
	const git_oid *stash_id,
	void *payload);

int git_stash_foreach(
	git_repository *repo,
	git_stash_cb callback,
	void *payload);

int git_stash_drop(
	git_repository *repo,
	size_t index);
