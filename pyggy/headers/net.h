static char *const GIT_DEFAULT_PORT;

typedef enum {
	GIT_DIRECTION_FETCH = ...,
	GIT_DIRECTION_PUSH  = ...,
} git_direction;


struct git_remote_head {
	int local;
	git_oid oid;
	git_oid loid;
	char *name;
};

typedef int (*git_headlist_cb)(git_remote_head *rhead, void *payload);
