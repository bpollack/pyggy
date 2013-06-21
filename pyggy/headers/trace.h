typedef enum {
	/** No tracing will be performed. */
	GIT_TRACE_NONE = ...,

	/** Severe errors that may impact the program's execution */
	GIT_TRACE_FATAL = ...,

	/** Errors that do not impact the program's execution */
	GIT_TRACE_ERROR = ...,

	/** Warnings that suggest abnormal data */
	GIT_TRACE_WARN = ...,

	/** Informational messages about program execution */
	GIT_TRACE_INFO = ...,

	/** Detailed data that allows for debugging */
	GIT_TRACE_DEBUG = ...,

	/** Exceptionally detailed debugging data */
	GIT_TRACE_TRACE = ...,
} git_trace_level_t;
typedef void (*git_trace_callback)(git_trace_level_t level, const char *msg);
int git_trace_set(git_trace_level_t level, git_trace_callback cb);
