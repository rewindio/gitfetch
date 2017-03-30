#include "gitfetch.h"

VALUE method_push(VALUE self, VALUE repository_path, VALUE push_url, VALUE access_token) {
  int error;

  git_repository *repository;
  git_remote *remote;
  git_strarray refs;

  // open repository "repository_path"
  error = git_repository_open(&repository, StringValuePtr(repository_path));
  if (error == 0) {
    error = git_remote_create_anonymous(&remote, repository, StringValuePtr(push_url));
    if (error == 0) {
      struct credentials_s credentials = { NULL, 0 };
      git_push_options push_options = GIT_PUSH_OPTIONS_INIT;

      if (access_token != Qnil) {
        credentials.access_token = StringValuePtr(access_token);
        push_options.callbacks.credentials = cb_cred_access_token;
        push_options.callbacks.payload     = &credentials;
      }

      error = git_reference_list(&refs, repository);
      if (error == 0) {
        error = git_remote_push(remote, &refs, &push_options);
        git_strarray_free(&refs);
      }
      git_remote_free(remote);
    }
    git_repository_free(repository);
  }

  // check for errors and raise exception accordingly
  // raise Git::Error if error is not 0
  if (error < 0) {
    raise_exception(error);
  }

  return Qnil;
}