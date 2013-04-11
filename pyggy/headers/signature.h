int git_signature_new(git_signature **out, const char *name, const char *email, git_time_t time, int offset);
int git_signature_now(git_signature **out, const char *name, const char *email);
git_signature * git_signature_dup(const git_signature *sig);
void git_signature_free(git_signature *sig);
