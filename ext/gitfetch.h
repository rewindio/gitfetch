#include <stdbool.h>
#include <git2.h>
#include <ruby.h>

#ifndef GIT_UNUSED
  #define GIT_UNUSED(x) ((void)(x))
#endif

struct credentials_s {
  char *access_token;
  unsigned int count;
};

// initialization method - Ruby calls this, not you
void Init_gitfetch();

int cb_cred_access_token(git_cred**, const char*, const char*, unsigned int, void*);
int fetch_repository(git_repository *repository, VALUE access_token);
void raise_exception(int error);

// method 'fetch' - methods are prefixed by 'method_' here
VALUE method_clone(VALUE self, VALUE remote_url, VALUE path, VALUE access_token);
VALUE method_fetch(VALUE self, VALUE repository_name, VALUE access_token);
VALUE method_libgit2_version(VALUE self);
VALUE method_push(VALUE self, VALUE repository_path, VALUE push_url, VALUE access_token);

extern VALUE rb_eGitError;
extern VALUE rb_eGitAuthenticationError;
extern VALUE rb_eGitEofError;
extern VALUE rb_eGitExistsError;
extern VALUE rb_eGitNetworkError;
extern VALUE rb_eGitNotFoundError;
