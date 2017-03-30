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
  VALUE Git = Qnil;

  Git = rb_define_module("Git");

  rb_define_module_function(Git, "clone", method_clone, 4);
  rb_define_module_function(Git, "fetch", method_fetch, 2);
  rb_define_module_function(Git, "libgit2_version", method_libgit2_version, 0);
  rb_define_module_function(Git, "push", method_push, 3);

  rb_eGitError = rb_define_class_under(Git, "Error", rb_eStandardError);
  rb_eGitAuthenticationError = rb_define_class_under(Git, "AuthenticationError", rb_eGitError);
  rb_eGitEofError = rb_define_class_under(Git, "EofError", rb_eGitError);
  rb_eGitExistsError = rb_define_class_under(Git, "ExistsError", rb_eGitError);
  rb_eGitNetworkError = rb_define_class_under(Git, "NetworkError", rb_eGitError);
  rb_eGitNotFoundError = rb_define_class_under(Git, "NotFoundError", rb_eGitError);

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
