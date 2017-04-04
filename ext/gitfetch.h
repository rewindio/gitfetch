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
void raise_exception(int error);

// methods under module Git are prefixed by 'rb_git_'
VALUE rb_git_fetch(int argc, VALUE *argv, VALUE self);
VALUE rb_git_libgit2_version(VALUE self);
VALUE rb_git_local_checkout(VALUE self, VALUE src, VALUE dst);
VALUE rb_git_mirror(int argc, VALUE *argv, VALUE self);
VALUE rb_git_push(int argc, VALUE *argv, VALUE self);

extern VALUE rb_eGitError;
extern VALUE rb_eGitAuthenticationError;
extern VALUE rb_eGitEofError;
extern VALUE rb_eGitExistsError;
extern VALUE rb_eGitNetworkError;
extern VALUE rb_eGitNotFoundError;
