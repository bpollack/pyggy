typedef int64_t git_off_t;
typedef int64_t git_time_t;

typedef enum {
     GIT_OBJ_ANY = ...,
     GIT_OBJ_BAD = ...,
     GIT_OBJ__EXT1 = ...,
     GIT_OBJ_COMMIT = ...,
     GIT_OBJ_TREE = ...,
     GIT_OBJ_BLOB = ...,
     GIT_OBJ_TAG = ...,
     GIT_OBJ__EXT2 = ...,
     GIT_OBJ_OFS_DELTA = ...,
     GIT_OBJ_REF_DELTA = ...,
} git_otype;

typedef struct git_odb git_odb;
typedef struct git_odb_backend git_odb_backend;
typedef struct git_odb_object git_odb_object;
typedef struct git_odb_stream git_odb_stream;
typedef struct git_odb_writepack git_odb_writepack;
typedef struct git_refdb git_refdb;
typedef struct git_refdb_backend git_refdb_backend;
typedef struct git_repository git_repository;
typedef struct git_object git_object;
typedef struct git_revwalk git_revwalk;
typedef struct git_tag git_tag;
typedef struct git_blob git_blob;
typedef struct git_commit git_commit;
typedef struct git_tree_entry git_tree_entry;
typedef struct git_tree git_tree;
typedef struct git_treebuilder git_treebuilder;
typedef struct git_index git_index;
typedef struct git_config git_config;
typedef struct git_config_backend git_config_backend;
typedef struct git_reflog_entry git_reflog_entry;
typedef struct git_reflog git_reflog;
typedef struct git_note git_note;
typedef struct git_packbuilder git_packbuilder;

typedef struct git_time {
    git_time_t time;
    int offset;
} git_time;

typedef struct git_signature {
    char *name;
    char *email;
    git_time when;
} git_signature;

typedef struct git_reference git_reference;

typedef enum {
    GIT_REF_INVALID = ...,
    GIT_REF_OID = ...,
    GIT_REF_SYMBOLIC = ...,
    GIT_REF_LISTALL = ...,
} git_ref_t;

typedef enum {
    GIT_BRANCH_LOCAL = ...,
    GIT_BRANCH_REMOTE = ...,
} git_branch_t;

typedef enum {
    GIT_FILEMODE_NEW = ...,
    GIT_FILEMODE_TREE = ...,
    GIT_FILEMODE_BLOB = ...,
    GIT_FILEMODE_BLOB_EXECUTABLE = ...,
    GIT_FILEMODE_LINK = ...,
    GIT_FILEMODE_COMMIT = ...,
} git_filemode_t;

typedef struct git_refspec git_refspec;
typedef struct git_remote git_remote;
typedef struct git_push git_push;

typedef struct git_remote_head git_remote_head;
typedef struct git_remote_callbacks git_remote_callbacks;
