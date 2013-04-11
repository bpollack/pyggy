struct git_odb_stream;
struct git_odb_writepack;

typedef int (*git_odb_foreach_cb)(const git_oid *id, void *payload);
struct git_odb_backend {
    unsigned int version;
    git_odb *odb;
    int (* read)(
            void **, size_t *, git_otype *,
            struct git_odb_backend *,
            const git_oid *);
    int (* read_prefix)(
            git_oid *,
            void **, size_t *, git_otype *,
            struct git_odb_backend *,
            const git_oid *,
            size_t);
    int (* read_header)(
            size_t *, git_otype *,
            struct git_odb_backend *,
            const git_oid *);
    int (* write)(
            git_oid *,
            struct git_odb_backend *,
            const void *,
            size_t,
            git_otype);
    int (* writestream)(
            struct git_odb_stream **,
            struct git_odb_backend *,
            size_t,
            git_otype);
    int (* readstream)(
            struct git_odb_stream **,
            struct git_odb_backend *,
            const git_oid *);
    int (* exists)(
            struct git_odb_backend *,
            const git_oid *);
    int (* refresh)(struct git_odb_backend *);
    int (* foreach)(
            struct git_odb_backend *,
            git_odb_foreach_cb cb,
            void *payload);
    int (* writepack)(
            struct git_odb_writepack **,
            struct git_odb_backend *,
            git_transfer_progress_callback progress_cb,
            void *progress_payload);
    void (* free)(struct git_odb_backend *);
};

#define GIT_ODB_BACKEND_VERSION ...

enum {
    GIT_STREAM_RDONLY = ...,
    GIT_STREAM_WRONLY = ...,
    GIT_STREAM_RW = ...,
};

struct git_odb_stream {
    struct git_odb_backend *backend;
    unsigned int mode;
    int (*read)(struct git_odb_stream *stream, char *buffer, size_t len);
    int (*write)(struct git_odb_stream *stream, const char *buffer, size_t len);
    int (*finalize_write)(git_oid *oid_p, struct git_odb_stream *stream);
    void (*free)(struct git_odb_stream *stream);
};

struct git_odb_writepack {
    struct git_odb_backend *backend;

    int (*add)(struct git_odb_writepack *writepack, const void *data, size_t size, git_transfer_progress *stats);
    int (*commit)(struct git_odb_writepack *writepack, git_transfer_progress *stats);
    void (*free)(struct git_odb_writepack *writepack);
};

void * git_odb_backend_malloc(git_odb_backend *backend, size_t len);
int git_odb_backend_pack(git_odb_backend **out, const char *objects_dir);
int git_odb_backend_loose(git_odb_backend **out, const char *objects_dir, int compression_level, int do_fsync);
int git_odb_backend_one_pack(git_odb_backend **out, const char *index_file);

int git_odb_new(git_odb **out);
int git_odb_open(git_odb **out, const char *objects_dir);
int git_odb_add_backend(git_odb *odb, git_odb_backend *backend, int priority);
int git_odb_add_alternate(git_odb *odb, git_odb_backend *backend, int priority);
int git_odb_add_disk_alternate(git_odb *odb, const char *path);
void git_odb_free(git_odb *db);
int git_odb_read(git_odb_object **out, git_odb *db, const git_oid *id);
int git_odb_read_prefix(git_odb_object **out, git_odb *db, const git_oid *short_id, size_t len);
int git_odb_read_header(size_t *len_out, git_otype *type_out, git_odb *db, const git_oid *id);
int git_odb_exists(git_odb *db, const git_oid *id);
int git_odb_refresh(struct git_odb *db);
int git_odb_foreach(git_odb *db, git_odb_foreach_cb cb, void *payload);
int git_odb_write(git_oid *out, git_odb *odb, const void *data, size_t len, git_otype type);
int git_odb_open_wstream(git_odb_stream **out, git_odb *db, size_t size, git_otype type);
int git_odb_open_rstream(git_odb_stream **out, git_odb *db, const git_oid *oid);
int git_odb_write_pack(
    git_odb_writepack **out,
    git_odb *db,
    git_transfer_progress_callback progress_cb,
    void *progress_payload);
int git_odb_hash(git_oid *out, const void *data, size_t len, git_otype type);
int git_odb_hashfile(git_oid *out, const char *path, git_otype type);
void git_odb_object_free(git_odb_object *object);
const git_oid * git_odb_object_id(git_odb_object *object);
void * git_odb_object_data(git_odb_object *object);
size_t git_odb_object_size(git_odb_object *object);
git_otype git_odb_object_type(git_odb_object *object);
