#include <git2.h>
#include <ruby.h>
#include "gitfetch.h"

// store error class Git::Error
VALUE rb_eGitError = Qnil;

// the initialization method for this module
void Init_gitfetch() {
  // initialize libgit2
  git_libgit2_init();

  // define a space for information and references about the module to be stored internally
  VALUE Git = Qnil;

  Git = rb_define_module("Git");
  rb_define_module_function(Git, "fetch", method_fetch, 2);
  rb_eGitError = rb_define_class_under(Git, "Error", rb_eStandardError);
}

// callback: tries the access_token passed in through the payload ONCE
//           and returns GIT_EAUTH if called for a second time
int cb_cred_access_token(git_cred **out, const char *url, const char *username_from_url,
  unsigned int allowed_types, void *payload) {

  GIT_UNUSED(url);
  GIT_UNUSED(username_from_url);

  if(allowed_types & GIT_CREDTYPE_USERPASS_PLAINTEXT) {
    struct credentials_s *credentials = payload;

    if(credentials->count > 0) {
      giterr_set_str(GITERR_CALLBACK, "Authentication failed.");
      return GIT_EAUTH;
    } else {
      credentials->count++;
      return git_cred_userpass_plaintext_new(out, credentials->access_token, "");
    }
  }

  return GIT_PASSTHROUGH;
}

// git-fetch remote "origin" for repository at "repository_path"
VALUE method_fetch(VALUE self, VALUE repository_path, VALUE access_token) {
  int error;

  git_repository *repository;
  git_remote *remote;

  // open repository "repository_path"
  error = git_repository_open(&repository, StringValuePtr(repository_path));
  if(! error) {
    // look up remote "origin"
    error = git_remote_lookup(&remote, repository, "origin");

    if(! error) {
      // fetch remote
      struct credentials_s credentials = { StringValuePtr(access_token), 0 };
      git_fetch_options fetch_options = GIT_FETCH_OPTIONS_INIT;

      fetch_options.callbacks.credentials = cb_cred_access_token;
      fetch_options.callbacks.payload     = &credentials;

      error = git_remote_fetch(remote, NULL, &fetch_options, "fetch");
      git_remote_free(remote);
    }
    git_repository_free(repository);
  }

  // check for errors and raise exception accordingly
  // raise Git::Error if error is not 0
  if (error) {
    const git_error *g_error = giterr_last();
    rb_raise(rb_eGitError, "%s (%d)\n", (g_error && g_error->message) ? g_error->message : "Unkown Error",
      error);
  }

  return Qnil;
}