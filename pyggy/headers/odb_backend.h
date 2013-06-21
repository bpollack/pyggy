int git_odb_backend_pack(git_odb_backend **out, const char *objects_dir);
int git_odb_backend_loose(git_odb_backend **out, const char *objects_dir, int compression_level, int do_fsync);
int git_odb_backend_one_pack(git_odb_backend **out, const char *index_file);
typedef enum {
	GIT_STREAM_RDONLY = (1 << 1),
	GIT_STREAM_WRONLY = (1 << 2),
	GIT_STREAM_RW = (GIT_STREAM_RDONLY | GIT_STREAM_WRONLY),
} git_odb_stream_t;
struct git_odb_stream {
	git_odb_backend *backend;
	unsigned int mode;

	int (*read)(git_odb_stream *stream, char *buffer, size_t len);
	int (*write)(git_odb_stream *stream, const char *buffer, size_t len);
	int (*finalize_write)(git_oid *oid_p, git_odb_stream *stream);
	void (*free)(git_odb_stream *stream);
};
struct git_odb_writepack {
	git_odb_backend *backend;

	int (*add)(git_odb_writepack *writepack, const void *data, size_t size, git_transfer_progress *stats);
	int (*commit)(git_odb_writepack *writepack, git_transfer_progress *stats);
	void (*free)(git_odb_writepack *writepack);
};
