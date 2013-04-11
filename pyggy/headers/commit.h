int git_commit_lookup(git_commit **commit, git_repository *repo, const git_oid *id);
int git_commit_lookup_prefix(git_commit **commit, git_repository *repo, const git_oid *id, size_t len);
void git_commit_free(git_commit *commit);
const git_oid * git_commit_id(const git_commit *commit);
const char * git_commit_message_encoding(const git_commit *commit);
const char * git_commit_message(const git_commit *commit);
git_time_t git_commit_time(const git_commit *commit);
int git_commit_time_offset(const git_commit *commit);
const git_signature * git_commit_committer(const git_commit *commit);
const git_signature * git_commit_author(const git_commit *commit);
int git_commit_tree(git_tree **tree_out, const git_commit *commit);
const git_oid * git_commit_tree_id(const git_commit *commit);
unsigned int git_commit_parentcount(const git_commit *commit);
int git_commit_parent(git_commit **out, git_commit *commit, unsigned int n);
const git_oid * git_commit_parent_id(git_commit *commit, unsigned int n);

int git_commit_nth_gen_ancestor(
	git_commit **ancestor,
	const git_commit *commit,
	unsigned int n);

int git_commit_create(
		git_oid *id,
		git_repository *repo,
		const char *update_ref,
		const git_signature *author,
		const git_signature *committer,
		const char *message_encoding,
		const char *message,
		const git_tree *tree,
		int parent_count,
		const git_commit *parents[]);

int git_commit_create_v(
		git_oid *id,
		git_repository *repo,
		const char *update_ref,
		const git_signature *author,
		const git_signature *committer,
		const char *message_encoding,
		const char *message,
		const git_tree *tree,
		int parent_count,
		...);
