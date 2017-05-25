#include "gitfetch.h"

#define check_error(error) if (error < 0) { git_remote_free(remote); return error; };

int fetch_origin(git_repository *repository, VALUE access_token);

// git-fetch remote "origin" for repository at "repository_path"
VALUE rb_git_fetch(int argc, VALUE *argv, VALUE self) {
  VALUE repository_path, access_token;
  rb_scan_args(argc, argv, "11", &repository_path, &access_token);

  int error;

  git_repository *repository = NULL;

  // open repository "repository_path"
  error = git_repository_open(&repository, StringValueCStr(repository_path));
  if (error == GIT_OK) {
    error = fetch_origin(repository, access_token);
  }

  git_repository_free(repository);

  // check for errors and raise exception accordingly
  // raise Git::Error if error is not 0
  if (error < 0) {
    raise_exception(error);
  }

  return Qnil;
}

int fetch_origin(git_repository *repository, VALUE access_token) {
  git_remote *remote = NULL;

  // look up remote "origin"
  check_error(git_remote_lookup(&remote, repository, "origin"));

  struct credentials_s credentials = { NULL, 0 };
  // fetch remote
  git_fetch_options fetch_options = GIT_FETCH_OPTIONS_INIT;
  fetch_options.download_tags = GIT_REMOTE_DOWNLOAD_TAGS_ALL;
  fetch_options.prune = GIT_FETCH_NO_PRUNE;

  if (access_token != Qnil) {
    credentials.access_token = StringValueCStr(access_token);
    fetch_options.callbacks.credentials = cb_cred_access_token;
    fetch_options.callbacks.payload     = &credentials;
  }

  check_error(git_remote_connect(remote, GIT_DIRECTION_FETCH, &fetch_options.callbacks,
                                 &fetch_options.proxy_opts, &fetch_options.custom_headers));

  check_error(git_remote_prune(remote, &fetch_options.callbacks));

  check_error(git_remote_fetch(remote, NULL, &fetch_options, NULL));

  git_remote_free(remote);

  return GIT_OK;
}
