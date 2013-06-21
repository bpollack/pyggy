typedef struct {
	git_time_t seconds;
	unsigned int nanoseconds;
} git_index_time;

typedef struct git_index_entry {
	git_index_time ctime;
	git_index_time mtime;

	unsigned int dev;
	unsigned int ino;
	unsigned int mode;
	unsigned int uid;
	unsigned int gid;
	git_off_t file_size;

	git_oid oid;

	unsigned short flags;
	unsigned short flags_extended;

	char *path;
} git_index_entry;

#define GIT_IDXENTRY_NAMEMASK ...
#define GIT_IDXENTRY_STAGEMASK ...
#define GIT_IDXENTRY_EXTENDED ...
#define GIT_IDXENTRY_VALID ...
#define GIT_IDXENTRY_STAGESHIFT ...
#define GIT_IDXENTRY_INTENT_TO_ADD ...
#define GIT_IDXENTRY_SKIP_WORKTREE ...
#define GIT_IDXENTRY_EXTENDED2 ...
#define GIT_IDXENTRY_EXTENDED_FLAGS ...
#define GIT_IDXENTRY_UPDATE ...
#define GIT_IDXENTRY_REMOVE ...
#define GIT_IDXENTRY_UPTODATE ...
#define GIT_IDXENTRY_ADDED ...
#define GIT_IDXENTRY_HASHED ...
#define GIT_IDXENTRY_UNHASHED ...
#define GIT_IDXENTRY_WT_REMOVE ...
#define GIT_IDXENTRY_CONFLICTED ...
#define GIT_IDXENTRY_UNPACKED ...
#define GIT_IDXENTRY_NEW_SKIP_WORKTREE ...

typedef enum {
	GIT_INDEXCAP_IGNORE_CASE = ...,
	GIT_INDEXCAP_NO_FILEMODE = ...,
	GIT_INDEXCAP_NO_SYMLINKS = ...,
	GIT_INDEXCAP_FROM_OWNER  = ...,
} git_indexcap_t;

typedef int (*git_index_matched_path_cb)(
	const char *path, const char *matched_pathspec, void *payload);

typedef enum {
	GIT_INDEX_ADD_DEFAULT = ...,
	GIT_INDEX_ADD_FORCE = ...,
	GIT_INDEX_ADD_DISABLE_PATHSPEC_MATCH = ...,
	GIT_INDEX_ADD_CHECK_PATHSPEC = ...,
} git_index_add_option_t;

int git_index_open(git_index **out, const char *index_path);
int git_index_new(git_index **out);
void git_index_free(git_index *index);
git_repository * git_index_owner(const git_index *index);
unsigned int git_index_caps(const git_index *index);
int git_index_set_caps(git_index *index, unsigned int caps);
int git_index_read(git_index *index);
int git_index_write(git_index *index);
int git_index_read_tree(git_index *index, const git_tree *tree);
int git_index_write_tree(git_oid *out, git_index *index);
int git_index_write_tree_to(git_oid *out, git_index *index, git_repository *repo);
size_t git_index_entrycount(const git_index *index);
void git_index_clear(git_index *index);

const git_index_entry * git_index_get_byindex(
	git_index *index, size_t n);

const git_index_entry * git_index_get_bypath(
	git_index *index, const char *path, int stage);

int git_index_remove(git_index *index, const char *path, int stage);

int git_index_remove_directory(
	git_index *index, const char *dir, int stage);

int git_index_add(git_index *index, const git_index_entry *source_entry);
int git_index_entry_stage(const git_index_entry *entry);
int git_index_add_bypath(git_index *index, const char *path);
int git_index_remove_bypath(git_index *index, const char *path);

int git_index_add_all(
	git_index *index,
	const git_strarray *pathspec,
	unsigned int flags,
	git_index_matched_path_cb callback,
	void *payload);

int git_index_remove_all(
	git_index *index,
	const git_strarray *pathspec,
	git_index_matched_path_cb callback,
	void *payload);

int git_index_update_all(
	git_index *index,
	const git_strarray *pathspec,
	git_index_matched_path_cb callback,
	void *payload);

int git_index_find(size_t *at_pos, git_index *index, const char *path);

int git_index_conflict_add(
	git_index *index,
	const git_index_entry *ancestor_entry,
	const git_index_entry *our_entry,
	const git_index_entry *their_entry);

int git_index_conflict_get(
	const git_index_entry **ancestor_out,
	const git_index_entry **our_out,
	const git_index_entry **their_out,
	git_index *index,
	const char *path);

int git_index_conflict_remove(git_index *index, const char *path);
void git_index_conflict_cleanup(git_index *index);
int git_index_has_conflicts(const git_index *index);

int git_index_conflict_iterator_new(
	git_index_conflict_iterator **iterator_out,
	git_index *index);

int git_index_conflict_next(
	const git_index_entry **ancestor_out,
	const git_index_entry **our_out,
	const git_index_entry **their_out,
	git_index_conflict_iterator *iterator);

void git_index_conflict_iterator_free(
	git_index_conflict_iterator *iterator);
