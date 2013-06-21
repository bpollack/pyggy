int git_reference_lookup(git_reference **out, git_repository *repo, const char *name);
int git_reference_name_to_id(
	git_oid *out, git_repository *repo, const char *name);
int git_reference_dwim(git_reference **out, git_repository *repo, const char *shorthand);
int git_reference_symbolic_create(git_reference **out, git_repository *repo, const char *name, const char *target, int force);
int git_reference_create(git_reference **out, git_repository *repo, const char *name, const git_oid *id, int force);
const git_oid * git_reference_target(const git_reference *ref);
const git_oid * git_reference_target_peel(const git_reference *ref);
const char * git_reference_symbolic_target(const git_reference *ref);
git_ref_t git_reference_type(const git_reference *ref);
const char * git_reference_name(const git_reference *ref);
int git_reference_resolve(git_reference **out, const git_reference *ref);
git_repository * git_reference_owner(const git_reference *ref);
int git_reference_symbolic_set_target(
	git_reference **out,
	git_reference *ref,
	const char *target);
int git_reference_set_target(
	git_reference **out,
	git_reference *ref,
	const git_oid *id);
int git_reference_rename(
	git_reference **new_ref,
	git_reference *ref,
	const char *new_name,
	int force);
int git_reference_delete(git_reference *ref);
int git_reference_list(git_strarray *array, git_repository *repo);
typedef int (*git_reference_foreach_cb)(git_reference *reference, void *payload);
typedef int (*git_reference_foreach_name_cb)(const char *name, void *payload);
int git_reference_foreach(
	git_repository *repo,
	git_reference_foreach_cb callback,
	void *payload);
int git_reference_foreach_name(
	git_repository *repo,
	git_reference_foreach_name_cb callback,
	void *payload);
void git_reference_free(git_reference *ref);
int git_reference_cmp(git_reference *ref1, git_reference *ref2);
int git_reference_iterator_new(
	git_reference_iterator **out,
	git_repository *repo);
int git_reference_iterator_glob_new(
	git_reference_iterator **out,
	git_repository *repo,
	const char *glob);
int git_reference_next(git_reference **out, git_reference_iterator *iter);
int git_reference_next_name(const char **out, git_reference_iterator *iter);
void git_reference_iterator_free(git_reference_iterator *iter);
int git_reference_foreach_glob(
	git_repository *repo,
	const char *glob,
	git_reference_foreach_name_cb callback,
	void *payload);
int git_reference_has_log(git_reference *ref);
int git_reference_is_branch(git_reference *ref);
int git_reference_is_remote(git_reference *ref);
typedef enum {
	GIT_REF_FORMAT_NORMAL = ...,
	GIT_REF_FORMAT_ALLOW_ONELEVEL = ...,
	GIT_REF_FORMAT_REFSPEC_PATTERN = ...,
	GIT_REF_FORMAT_REFSPEC_SHORTHAND = ...,
} git_reference_normalize_t;

int git_reference_normalize_name(
	char *buffer_out,
	size_t buffer_size,
	const char *name,
	unsigned int flags);

int git_reference_peel(
	git_object **out,
	git_reference *ref,
	git_otype type);

int git_reference_is_valid_name(const char *refname);
const char * git_reference_shorthand(git_reference *ref);
