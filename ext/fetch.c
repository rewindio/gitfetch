#include "gitfetch.h"

// git-fetch remote "origin" for repository at "repository_path"
VALUE method_fetch(int argc, VALUE *argv, VALUE self) {
  VALUE repository_path, access_token;
  rb_scan_args(argc, argv, "11", &repository_path, &access_token);

  int error;

  git_repository *repository;

  // open repository "repository_path"
  error = git_repository_open(&repository, StringValuePtr(repository_path));
  if (error == 0) {
    error = fetch_repository(repository, access_token);
  }

  // check for errors and raise exception accordingly
  // raise Git::Error if error is not 0
  if (error < 0) {
    raise_exception(error);
  } else {
    git_repository_free(repository);
  }

  return Qnil;
}

int fetch_repository(git_repository *repository, VALUE access_token) {
  int error;

  git_remote *remote;

  // look up remote "origin"
  error = git_remote_lookup(&remote, repository, "origin");

  if (error == 0) {
    struct credentials_s credentials = { NULL, 0 };
    // fetch remote
    git_fetch_options fetch_options = GIT_FETCH_OPTIONS_INIT;
    fetch_options.download_tags = GIT_REMOTE_DOWNLOAD_TAGS_ALL;
    fetch_options.prune = GIT_FETCH_PRUNE;

    if (access_token != Qnil) {
      credentials.access_token = StringValuePtr(access_token);
      fetch_options.callbacks.credentials = cb_cred_access_token;
      fetch_options.callbacks.payload     = &credentials;
    }

    error = git_remote_fetch(remote, NULL, &fetch_options, NULL);
    git_remote_free(remote);
  }

  return error;
}