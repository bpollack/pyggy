typedef struct {
	unsigned int version;
	unsigned int pb_parallelism;
} git_push_options;

#define GIT_PUSH_OPTIONS_VERSION ...
int git_push_new(git_push **out, git_remote *remote);
int git_push_set_options(
	git_push *push,
	const git_push_options *opts);
int git_push_add_refspec(git_push *push, const char *refspec);
int git_push_update_tips(git_push *push);
int git_push_finish(git_push *push);
int git_push_unpack_ok(git_push *push);
int git_push_status_foreach(git_push *push,
			int (*cb)(const char *ref, const char *msg, void *data),
			void *data);
void git_push_free(git_push *push);
