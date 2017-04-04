#include "gitfetch.h"

// store error classes
VALUE rb_eGitError = Qnil;
VALUE rb_eGitAuthenticationError = Qnil;
VALUE rb_eGitEofError = Qnil;
VALUE rb_eGitExistsError = Qnil;
VALUE rb_eGitNetworkError = Qnil;
VALUE rb_eGitNotFoundError = Qnil;

static VALUE rb_mShutdownHook;
static void cleanup_cb(void*);

// the initialization method for this module
void Init_gitfetch() {
  // define a space for information and references about the module to be stored internally
  VALUE rb_mGit = Qnil;

  rb_mGit = rb_define_module("Git");

  rb_define_module_function(rb_mGit, "fetch", rb_git_fetch, -1);
  rb_define_module_function(rb_mGit, "libgit2_version", rb_git_libgit2_version, 0);
  rb_define_module_function(rb_mGit, "local_checkout", rb_git_local_checkout, 2);
  rb_define_module_function(rb_mGit, "mirror", rb_git_mirror, -1);
  rb_define_module_function(rb_mGit, "push", rb_git_push, -1);

  rb_eGitError = rb_define_class_under(rb_mGit, "Error", rb_eStandardError);
  rb_eGitAuthenticationError = rb_define_class_under(rb_mGit, "AuthenticationError", rb_eGitError);
  rb_eGitEofError = rb_define_class_under(rb_mGit, "EofError", rb_eGitError);
  rb_eGitExistsError = rb_define_class_under(rb_mGit, "ExistsError", rb_eGitError);
  rb_eGitNetworkError = rb_define_class_under(rb_mGit, "NetworkError", rb_eGitError);
  rb_eGitNotFoundError = rb_define_class_under(rb_mGit, "NotFoundError", rb_eGitError);

  // initialize libgit2
  git_libgit2_init();

  /* Hook a global object to cleanup the library
   * on shutdown */
  rb_mShutdownHook = Data_Wrap_Struct(rb_cObject, NULL, &cleanup_cb, NULL);
  rb_global_variable(&rb_mShutdownHook);
}

static void cleanup_cb(void *unused) {
  GIT_UNUSED(unused);
  git_libgit2_shutdown();
}
