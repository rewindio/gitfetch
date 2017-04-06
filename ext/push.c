#include "gitfetch.h"

VALUE rb_git_push(int argc, VALUE *argv, VALUE self) {
  VALUE repository_path, push_url, access_token;
  rb_scan_args(argc, argv, "21", &repository_path, &push_url, &access_token);

  int error;

  git_repository *repository = NULL;
  git_remote *remote = NULL;
  git_strarray refs;

  // open repository "repository_path"
  error = git_repository_open(&repository, StringValueCStr(repository_path));
  if (error == GIT_OK) {
    error = git_remote_create_anonymous(&remote, repository, StringValueCStr(push_url));
    if (error == GIT_OK) {
      struct credentials_s credentials = { NULL, 0 };
      git_push_options push_options = GIT_PUSH_OPTIONS_INIT;

      if (access_token != Qnil) {
        credentials.access_token = StringValueCStr(access_token);
        push_options.callbacks.credentials = cb_cred_access_token;
        push_options.callbacks.payload     = &credentials;
      }

      error = git_reference_list(&refs, repository);
      if (error == GIT_OK) {
        error = git_remote_push(remote, &refs, &push_options);
      }
      git_strarray_free(&refs);
    }
    git_remote_free(remote);
  }

  git_repository_free(repository);

  // check for errors and raise exception accordingly
  // raise Git::Error if error is not 0
  if (error < 0) {
    raise_exception(error);
  }

  return Qnil;
}
