#ifndef GITFETCH_H
#define GITFETCH_H

#include <stdbool.h>
#include <git2.h>
#include <ruby.h>
#include <ruby/thread.h>

#ifndef GIT_UNUSED
  #define GIT_UNUSED(x) ((void)(x))
#endif

struct cb_args {
  char *src;
  char *dst;
  char *access_token;
  int update_head;
  int error;
};

struct credentials_s {
  char *access_token;
  unsigned int count;
};

// initialization methods - Ruby calls these, not you
void Init_gitfetch();
void Init_gitfetch_clone_bare();
void Init_gitfetch_is_repository();
void Init_gitfetch_fetch();
void Init_gitfetch_libgit2_version();
void Init_gitfetch_local_checkout();
void Init_gitfetch_push();
void Init_gitfetch_register_dummy_transport();
void Init_gitfetch_remote_url();

int cb_cred_access_token(git_cred**, const char*, const char*, unsigned int, void*);
void raise_exception(int error);

// methods under module Git are prefixed by 'rb_git_'
VALUE rb_git_clone_bare(int argc, VALUE *argv, VALUE self);
VALUE rb_git_fetch(int argc, VALUE *argv, VALUE self);
VALUE rb_git_libgit2_version(VALUE self);
VALUE rb_git_local_checkout(VALUE self, VALUE src, VALUE dst);
VALUE rb_git_push(int argc, VALUE *argv, VALUE self);
VALUE rb_git_remote_url(VALUE self, VALUE repository_path);
VALUE rb_git_set_remote_url(VALUE self, VALUE repository_path, VALUE remote_url);

extern VALUE rb_mGit;

extern VALUE rb_eGitError;
extern VALUE rb_eGitAuthenticationError;
extern VALUE rb_eGitEofError;
extern VALUE rb_eGitExistsError;
extern VALUE rb_eGitHTTPError;
extern VALUE rb_eGitNetworkError;
extern VALUE rb_eGitNotFoundError;
extern VALUE rb_eGitRepositoryNotExportedError;
extern VALUE rb_eGitUserError;

#endif
