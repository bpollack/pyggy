enum {
	GIT_CONFIG_LEVEL_SYSTEM = ...,
	GIT_CONFIG_LEVEL_XDG = ...,
	GIT_CONFIG_LEVEL_GLOBAL = ...,
	GIT_CONFIG_LEVEL_LOCAL = ...,
	GIT_CONFIG_HIGHEST_LEVEL = ...,
};

typedef struct {
	const char *name;
	const char *value;
	unsigned int level;
} git_config_entry;

typedef int  (*git_config_foreach_cb)(const git_config_entry *, void *);


struct git_config_backend {
	unsigned int version;
	struct git_config *cfg;
	int (*open)(struct git_config_backend *, unsigned int level);
	int (*get)(const struct git_config_backend *, const char *key, const git_config_entry **entry);
	int (*get_multivar)(struct git_config_backend *, const char *key, const char *regexp, git_config_foreach_cb callback, void *payload);
	int (*set)(struct git_config_backend *, const char *key, const char *value);
	int (*set_multivar)(git_config_backend *cfg, const char *name, const char *regexp, const char *value);
	int (*del)(struct git_config_backend *, const char *key);
	int (*foreach)(struct git_config_backend *, const char *, git_config_foreach_cb callback, void *payload);
	int (*refresh)(struct git_config_backend *);
	void (*free)(struct git_config_backend *);
};

#define GIT_CONFIG_BACKEND_VERSION ...

typedef enum {
	GIT_CVAR_FALSE = ...,
	GIT_CVAR_TRUE = ...,
	GIT_CVAR_INT32,
	GIT_CVAR_STRING,
} git_cvar_t;

typedef struct {
	git_cvar_t cvar_type;
	const char *str_match;
	int map_value;
} git_cvar_map;

int git_config_find_global(char *out, size_t length);
int git_config_find_xdg(char *out, size_t length);
int git_config_find_system(char *out, size_t length);
int git_config_open_default(git_config **out);
int git_config_new(git_config **out);

int git_config_add_backend(
	git_config *cfg,
	git_config_backend *file,
	unsigned int level,
	int force);

int git_config_add_file_ondisk(
	git_config *cfg,
	const char *path,
	unsigned int level,
	int force);

int git_config_open_ondisk(git_config **out, const char *path);

int git_config_open_level(
    git_config **out,
    const git_config *parent,
    unsigned int level);

int git_config_refresh(git_config *cfg);
void git_config_free(git_config *cfg);

int git_config_get_entry(
   const git_config_entry **out,
	const git_config *cfg,
	const char *name);

int git_config_get_int32(int32_t *out, const git_config *cfg, const char *name);
int git_config_get_int64(int64_t *out, const git_config *cfg, const char *name);
int git_config_get_bool(int *out, const git_config *cfg, const char *name);
int git_config_get_string(const char **out, const git_config *cfg, const char *name);
int git_config_get_multivar(const git_config *cfg, const char *name, const char *regexp, git_config_foreach_cb callback, void *payload);
int git_config_set_int32(git_config *cfg, const char *name, int32_t value);
int git_config_set_int64(git_config *cfg, const char *name, int64_t value);
int git_config_set_bool(git_config *cfg, const char *name, int value);
int git_config_set_string(git_config *cfg, const char *name, const char *value);
int git_config_set_multivar(git_config *cfg, const char *name, const char *regexp, const char *value);
int git_config_delete_entry(git_config *cfg, const char *name);

int git_config_foreach(
	const git_config *cfg,
	git_config_foreach_cb callback,
	void *payload);

int git_config_foreach_match(
	const git_config *cfg,
	const char *regexp,
	git_config_foreach_cb callback,
	void *payload);

int git_config_get_mapped(
      int *out,
      const git_config *cfg,
      const char *name,
      const git_cvar_map *maps,
      size_t map_n);

int git_config_lookup_map_value(
	int *out,
	const git_cvar_map *maps,
	size_t map_n,
	const char *value);

int git_config_parse_bool(int *out, const char *value);
int git_config_parse_int32(int32_t *out, const char *value);
int git_config_parse_int64(int64_t *out, const char *value);
