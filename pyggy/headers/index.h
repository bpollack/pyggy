typedef struct git_transfer_progress {
    unsigned int total_objects;
    unsigned int indexed_objects;
    unsigned int received_objects;
    size_t received_bytes;
} git_transfer_progress;

typedef int (*git_transfer_progress_callback)(const git_transfer_progress *stats, void *payload);

typedef struct git_indexer_stream git_indexer_stream;

int git_indexer_stream_new(
    git_indexer_stream **out,
    const char *path,
    git_transfer_progress_callback progress_cb,
    void *progress_cb_payload);

int git_indexer_stream_add(git_indexer_stream *idx, const void *data, size_t size, git_transfer_progress *stats);
int git_indexer_stream_finalize(git_indexer_stream *idx, git_transfer_progress *stats);
const git_oid * git_indexer_stream_hash(const git_indexer_stream *idx);
void git_indexer_stream_free(git_indexer_stream *idx);
