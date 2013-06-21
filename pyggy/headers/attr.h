typedef enum {
	GIT_ATTR_UNSPECIFIED_T = ...,
	GIT_ATTR_TRUE_T = ...,
	GIT_ATTR_FALSE_T = ...,
	GIT_ATTR_VALUE_T = ...,
} git_attr_t;

git_attr_t git_attr_value(const char *attr);

#define GIT_ATTR_CHECK_FILE_THEN_INDEX ...
#define GIT_ATTR_CHECK_INDEX_THEN_FILE ...
#define GIT_ATTR_CHECK_INDEX_ONLY ...
#define GIT_ATTR_CHECK_NO_SYSTEM ...

int git_attr_get(
	const char **value_out,
	git_repository *repo,
	uint32_t flags,
	const char *path,
	const char *name);

int git_attr_get_many(
	const char **values_out,
	git_repository *repo,
	uint32_t flags,
	const char *path,
	size_t num_attr,
	const char **names);

typedef int (*git_attr_foreach_cb)(const char *name, const char *value, void *payload);

int git_attr_foreach(
	git_repository *repo,
	uint32_t flags,
	const char *path,
	git_attr_foreach_cb callback,
	void *payload);

void git_attr_cache_flush(
	git_repository *repo);

int git_attr_add_macro(
	git_repository *repo,
	const char *name,
	const char *values);
