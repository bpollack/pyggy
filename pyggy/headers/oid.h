#define GIT_OID_RAWSZ ...
#define GIT_OID_HEXSZ ...
#define GIT_OID_MINPREFIXLEN ...
typedef struct git_oid {
	unsigned char id[...];
} git_oid;
int git_oid_fromstr(git_oid *out, const char *str);
int git_oid_fromstrp(git_oid *out, const char *str);
int git_oid_fromstrn(git_oid *out, const char *str, size_t length);
void git_oid_fromraw(git_oid *out, const unsigned char *raw);
void git_oid_fmt(char *out, const git_oid *id);
void git_oid_nfmt(char *out, size_t n, const git_oid *id);
void git_oid_pathfmt(char *out, const git_oid *id);
char * git_oid_allocfmt(const git_oid *id);
char * git_oid_tostr(char *out, size_t n, const git_oid *id);
void git_oid_cpy(git_oid *out, const git_oid *src);
int git_oid_cmp(const git_oid *a, const git_oid *b);
int git_oid_equal(const git_oid *a, const git_oid *b);
int git_oid_ncmp(const git_oid *a, const git_oid *b, size_t len);
int git_oid_streq(const git_oid *id, const char *str);
int git_oid_strcmp(const git_oid *id, const char *str);
int git_oid_iszero(const git_oid *id);
typedef struct git_oid_shorten git_oid_shorten;
git_oid_shorten * git_oid_shorten_new(size_t min_length);
int git_oid_shorten_add(git_oid_shorten *os, const char *text_id);
void git_oid_shorten_free(git_oid_shorten *os);
