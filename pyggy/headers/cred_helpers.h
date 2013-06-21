typedef struct git_cred_userpass_payload {
	char *username;
	char *password;
} git_cred_userpass_payload;


int git_cred_userpass(
		git_cred **cred,
		const char *url,
		const char *user_from_url,
		unsigned int allowed_types,
		void *payload);
