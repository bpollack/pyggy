typedef enum {
	GIT_CREDTYPE_USERPASS_PLAINTEXT = ...,
} git_credtype_t;

typedef struct git_cred {
	git_credtype_t credtype;
	void (*free)(
		struct git_cred *cred);
} git_cred;

typedef struct git_cred_userpass_plaintext {
	git_cred parent;
	char *username;
	char *password;
} git_cred_userpass_plaintext;

int git_cred_userpass_plaintext_new(
	git_cred **out,
	const char *username,
	const char *password);

typedef int (*git_cred_acquire_cb)(
	git_cred **cred,
	const char *url,
	const char *username_from_url,
	unsigned int allowed_types,
	void *payload);

typedef enum {
	GIT_TRANSPORTFLAGS_NONE = ...,
	GIT_TRANSPORTFLAGS_NO_CHECK_CERT = ...,
} git_transport_flags_t;

typedef void (*git_transport_message_cb)(const char *str, int len, void *data);

typedef struct git_transport {
	unsigned int version;
	int (*set_callbacks)(struct git_transport *transport,
		git_transport_message_cb progress_cb,
		git_transport_message_cb error_cb,
		void *payload);

	int (*connect)(struct git_transport *transport,
		const char *url,
		git_cred_acquire_cb cred_acquire_cb,
		void *cred_acquire_payload,
		int direction,
		int flags);

	int (*ls)(struct git_transport *transport,
		git_headlist_cb list_cb,
		void *payload);

	int (*push)(struct git_transport *transport, git_push *push);

	int (*negotiate_fetch)(struct git_transport *transport,
		git_repository *repo,
		const git_remote_head * const *refs,
		size_t count);

	int (*download_pack)(struct git_transport *transport,
		git_repository *repo,
		git_transfer_progress *stats,
		git_transfer_progress_callback progress_cb,
		void *progress_payload);

	int (*is_connected)(struct git_transport *transport);
	int (*read_flags)(struct git_transport *transport, int *flags);
	void (*cancel)(struct git_transport *transport);
	int (*close)(struct git_transport *transport);
	void (*free)(struct git_transport *transport);
} git_transport;

#define GIT_TRANSPORT_VERSION ...

int git_transport_new(git_transport **out, git_remote *owner, const char *url);

typedef int (*git_transport_cb)(git_transport **out, git_remote *owner, void *param);

int git_transport_dummy(
	git_transport **out,
	git_remote *owner,
	void *payload);

int git_transport_local(
	git_transport **out,
	git_remote *owner,
	void *payload);

int git_transport_smart(
	git_transport **out,
	git_remote *owner,
	void *payload);

typedef enum {
	GIT_SERVICE_UPLOADPACK_LS = 1,
	GIT_SERVICE_UPLOADPACK = 2,
	GIT_SERVICE_RECEIVEPACK_LS = 3,
	GIT_SERVICE_RECEIVEPACK = 4,
} git_smart_service_t;

struct git_smart_subtransport;

typedef struct git_smart_subtransport_stream {
	struct git_smart_subtransport *subtransport;

	int (*read)(
			struct git_smart_subtransport_stream *stream,
			char *buffer,
			size_t buf_size,
			size_t *bytes_read);

	int (*write)(
			struct git_smart_subtransport_stream *stream,
			const char *buffer,
			size_t len);

	void (*free)(
			struct git_smart_subtransport_stream *stream);
} git_smart_subtransport_stream;

typedef struct git_smart_subtransport {
	int (* action)(
			git_smart_subtransport_stream **out,
			struct git_smart_subtransport *transport,
			const char *url,
			git_smart_service_t action);
	int (* close)(struct git_smart_subtransport *transport);
	void (* free)(struct git_smart_subtransport *transport);
} git_smart_subtransport;

typedef int (*git_smart_subtransport_cb)(
	git_smart_subtransport **out,
	git_transport* owner);

typedef struct git_smart_subtransport_definition {
	git_smart_subtransport_cb callback;
	unsigned rpc;
} git_smart_subtransport_definition;


int git_smart_subtransport_http(
	git_smart_subtransport **out,
	git_transport* owner);

int git_smart_subtransport_git(
	git_smart_subtransport **out,
	git_transport* owner);
