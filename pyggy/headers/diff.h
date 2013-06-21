typedef enum {
	GIT_DIFF_NORMAL = ...,
	GIT_DIFF_REVERSE = ...,
	GIT_DIFF_FORCE_TEXT = ...,
	GIT_DIFF_IGNORE_WHITESPACE = ...,
	GIT_DIFF_IGNORE_WHITESPACE_CHANGE = ...,
	GIT_DIFF_IGNORE_WHITESPACE_EOL = ...,
	GIT_DIFF_IGNORE_SUBMODULES = ...,
	GIT_DIFF_PATIENCE = ...,
	GIT_DIFF_INCLUDE_IGNORED = ...,
	GIT_DIFF_INCLUDE_UNTRACKED = ...,
	GIT_DIFF_INCLUDE_UNMODIFIED = ...,
	GIT_DIFF_RECURSE_UNTRACKED_DIRS = ...,
	GIT_DIFF_DISABLE_PATHSPEC_MATCH = ...,
	GIT_DIFF_DELTAS_ARE_ICASE = ...,
	GIT_DIFF_INCLUDE_UNTRACKED_CONTENT = ...,
	GIT_DIFF_SKIP_BINARY_CHECK = ...,
	GIT_DIFF_INCLUDE_TYPECHANGE = ...,
	GIT_DIFF_INCLUDE_TYPECHANGE_TREES  = ...,
	GIT_DIFF_IGNORE_FILEMODE = ...,
	GIT_DIFF_RECURSE_IGNORED_DIRS = ...,
	GIT_DIFF_FAST_UNTRACKED_DIRS = ...,
	GIT_DIFF_FORCE_BINARY = ...,
} git_diff_option_t;

typedef struct git_diff_list git_diff_list;

typedef enum {
	GIT_DIFF_FLAG_BINARY     = ...,
	GIT_DIFF_FLAG_NOT_BINARY = ...,
	GIT_DIFF_FLAG_VALID_OID  = ...,
} git_diff_flag_t;

typedef enum {
	GIT_DELTA_UNMODIFIED = ...,
	GIT_DELTA_ADDED = ...,
	GIT_DELTA_DELETED = ...,
	GIT_DELTA_MODIFIED = ...,
	GIT_DELTA_RENAMED = ...,
	GIT_DELTA_COPIED = ...,
	GIT_DELTA_IGNORED = ...,
	GIT_DELTA_UNTRACKED = ...,
	GIT_DELTA_TYPECHANGE = ...,
} git_delta_t;

typedef struct {
	git_oid     oid;
	const char *path;
	git_off_t   size;
	uint32_t    flags;
	uint16_t    mode;
} git_diff_file;

typedef struct {
	git_diff_file old_file;
	git_diff_file new_file;
	git_delta_t   status;
	uint32_t      similarity;
	uint32_t      flags;
} git_diff_delta;

typedef int (*git_diff_notify_cb)(
	const git_diff_list *diff_so_far,
	const git_diff_delta *delta_to_add,
	const char *matched_pathspec,
	void *payload);

typedef struct {
	unsigned int version;
	uint32_t flags;
	uint16_t context_lines;
	uint16_t interhunk_lines;
	const char *old_prefix;
	const char *new_prefix;
	git_strarray pathspec;
	git_off_t max_size;
	git_diff_notify_cb notify_cb;
	void *notify_payload;
} git_diff_options;

#define GIT_DIFF_OPTIONS_VERSION ...

typedef int (*git_diff_file_cb)(
	const git_diff_delta *delta,
	float progress,
	void *payload);

typedef struct {
	int old_start;
	int old_lines;
	int new_start;
	int new_lines;
} git_diff_range;

typedef int (*git_diff_hunk_cb)(
	const git_diff_delta *delta,
	const git_diff_range *range,
	const char *header,
	size_t header_len,
	void *payload);

typedef enum {
	GIT_DIFF_LINE_CONTEXT   = ...,
	GIT_DIFF_LINE_ADDITION  = ...,
	GIT_DIFF_LINE_DELETION  = ...,
	GIT_DIFF_LINE_CONTEXT_EOFNL = ...,
	GIT_DIFF_LINE_ADD_EOFNL = ...,
	GIT_DIFF_LINE_DEL_EOFNL = ...,
	GIT_DIFF_LINE_FILE_HDR  = ...,
	GIT_DIFF_LINE_HUNK_HDR  = ...,
	GIT_DIFF_LINE_BINARY    = ...,
} git_diff_line_t;

typedef int (*git_diff_data_cb)(
	const git_diff_delta *delta,
	const git_diff_range *range,
	char line_origin,
	const char *content,
	size_t content_len,
	void *payload);

typedef struct git_diff_patch git_diff_patch;

typedef enum {
	GIT_DIFF_FIND_RENAMES = ...,
	GIT_DIFF_FIND_RENAMES_FROM_REWRITES = ...,
	GIT_DIFF_FIND_COPIES = ...,
	GIT_DIFF_FIND_COPIES_FROM_UNMODIFIED = ...,
	GIT_DIFF_FIND_REWRITES = ...,
	GIT_DIFF_BREAK_REWRITES = ...,
	GIT_DIFF_FIND_AND_BREAK_REWRITES = ...,
	GIT_DIFF_FIND_FOR_UNTRACKED = ...,
	GIT_DIFF_FIND_ALL = ...,
	GIT_DIFF_FIND_IGNORE_LEADING_WHITESPACE = ...,
	GIT_DIFF_FIND_IGNORE_WHITESPACE = ...,
	GIT_DIFF_FIND_DONT_IGNORE_WHITESPACE = ...,
	GIT_DIFF_FIND_EXACT_MATCH_ONLY = ...,
} git_diff_find_t;

typedef struct {
	int (*file_signature)(
		void **out, const git_diff_file *file,
		const char *fullpath, void *payload);
	int (*buffer_signature)(
		void **out, const git_diff_file *file,
		const char *buf, size_t buflen, void *payload);
	void (*free_signature)(void *sig, void *payload);
	int (*similarity)(int *score, void *siga, void *sigb, void *payload);
	void *payload;
} git_diff_similarity_metric;

typedef struct {
	unsigned int version;
	uint32_t flags;
	uint16_t rename_threshold;
	uint16_t rename_from_rewrite_threshold;
	uint16_t copy_threshold;
	uint16_t break_rewrite_threshold;
	size_t rename_limit;
	git_diff_similarity_metric *metric;
} git_diff_find_options;

#define GIT_DIFF_FIND_OPTIONS_VERSION ...

void git_diff_list_free(git_diff_list *diff);

int git_diff_tree_to_tree(
	git_diff_list **diff,
	git_repository *repo,
	git_tree *old_tree,
	git_tree *new_tree,
	const git_diff_options *opts);

int git_diff_tree_to_index(
	git_diff_list **diff,
	git_repository *repo,
	git_tree *old_tree,
	git_index *index,
	const git_diff_options *opts);

int git_diff_index_to_workdir(
	git_diff_list **diff,
	git_repository *repo,
	git_index *index,
	const git_diff_options *opts);

int git_diff_tree_to_workdir(
	git_diff_list **diff,
	git_repository *repo,
	git_tree *old_tree,
	const git_diff_options *opts);

int git_diff_merge(
	git_diff_list *onto,
	const git_diff_list *from);

int git_diff_find_similar(
	git_diff_list *diff,
	git_diff_find_options *options);

int git_diff_foreach(
	git_diff_list *diff,
	git_diff_file_cb file_cb,
	git_diff_hunk_cb hunk_cb,
	git_diff_data_cb line_cb,
	void *payload);

int git_diff_print_compact(
	git_diff_list *diff,
	git_diff_data_cb print_cb,
	void *payload);

int git_diff_print_raw(
	git_diff_list *diff,
	git_diff_data_cb print_cb,
	void *payload);

char git_diff_status_char(git_delta_t status);

int git_diff_print_patch(
	git_diff_list *diff,
	git_diff_data_cb print_cb,
	void *payload);

size_t git_diff_num_deltas(git_diff_list *diff);

size_t git_diff_num_deltas_of_type(
	git_diff_list *diff,
	git_delta_t type);

int git_diff_get_patch(
	git_diff_patch **patch_out,
	const git_diff_delta **delta_out,
	git_diff_list *diff,
	size_t idx);

void git_diff_patch_free(
	git_diff_patch *patch);

const git_diff_delta * git_diff_patch_delta(
	git_diff_patch *patch);

size_t git_diff_patch_num_hunks(
	git_diff_patch *patch);

int git_diff_patch_line_stats(
	size_t *total_context,
	size_t *total_additions,
	size_t *total_deletions,
	const git_diff_patch *patch);

int git_diff_patch_get_hunk(
	const git_diff_range **range,
	const char **header,
	size_t *header_len,
	size_t *lines_in_hunk,
	git_diff_patch *patch,
	size_t hunk_idx);

int git_diff_patch_num_lines_in_hunk(
	git_diff_patch *patch,
	size_t hunk_idx);

int git_diff_patch_get_line_in_hunk(
	char *line_origin,
	const char **content,
	size_t *content_len,
	int *old_lineno,
	int *new_lineno,
	git_diff_patch *patch,
	size_t hunk_idx,
	size_t line_of_hunk);

int git_diff_patch_print(
	git_diff_patch *patch,
	git_diff_data_cb print_cb,
	void *payload);

int git_diff_patch_to_str(
	char **string,
	git_diff_patch *patch);

int git_diff_blobs(
	const git_blob *old_blob,
	const char *old_as_path,
	const git_blob *new_blob,
	const char *new_as_path,
	const git_diff_options *options,
	git_diff_file_cb file_cb,
	git_diff_hunk_cb hunk_cb,
	git_diff_data_cb line_cb,
	void *payload);

int git_diff_patch_from_blobs(
	git_diff_patch **out,
	const git_blob *old_blob,
	const char *old_as_path,
	const git_blob *new_blob,
	const char *new_as_path,
	const git_diff_options *opts);

int git_diff_blob_to_buffer(
	const git_blob *old_blob,
	const char *old_as_path,
	const char *buffer,
	size_t buffer_len,
	const char *buffer_as_path,
	const git_diff_options *options,
	git_diff_file_cb file_cb,
	git_diff_hunk_cb hunk_cb,
	git_diff_data_cb data_cb,
	void *payload);

int git_diff_patch_from_blob_and_buffer(
	git_diff_patch **out,
	const git_blob *old_blob,
	const char *old_as_path,
	const char *buffer,
	size_t buffer_len,
	const char *buffer_as_path,
	const git_diff_options *opts);
