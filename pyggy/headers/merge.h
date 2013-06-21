typedef enum {
	/** Detect renames */
	GIT_MERGE_TREE_FIND_RENAMES = ...,
} git_merge_tree_flag_t;

typedef enum {
	GIT_MERGE_AUTOMERGE_NORMAL = ...,
	GIT_MERGE_AUTOMERGE_NONE = ...,
	GIT_MERGE_AUTOMERGE_FAVOR_OURS = ...,
	GIT_MERGE_AUTOMERGE_FAVOR_THEIRS = ...,
} git_merge_automerge_flags;

typedef struct {
	unsigned int version;
	git_merge_tree_flag_t flags;

	/** Similarity to consider a file renamed (default 50) */
	unsigned int rename_threshold;

	/** Maximum similarity sources to examine (overrides the
	 * `merge.renameLimit` config) (default 200)
	 */
	unsigned int target_limit;

	/** Pluggable similarity metric; pass NULL to use internal metric */
	git_diff_similarity_metric *metric;

	/** Flags for automerging content. */
	git_merge_automerge_flags automerge_flags;
} git_merge_tree_opts;

#define GIT_MERGE_TREE_OPTS_VERSION ...


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

int git_merge_head_from_ref(
	git_merge_head **out,
	git_repository *repo,
	git_reference *ref);

int git_merge_head_from_fetchhead(
	git_merge_head **out,
	git_repository *repo,
	const char *branch_name,
	const char *remote_url,
	const git_oid *oid);

int git_merge_head_from_oid(
	git_merge_head **out,
	git_repository *repo,
	const git_oid *oid);

void git_merge_head_free(
	git_merge_head *head);

int git_merge_trees(
	git_index **out,
	git_repository *repo,
	const git_tree *ancestor_tree,
	const git_tree *our_tree,
	const git_tree *their_tree,
	const git_merge_tree_opts *opts);
