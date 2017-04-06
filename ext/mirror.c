#include "gitfetch.h"

int create_remote(git_remote **out, git_repository *repository, const char *name, const char *url, void *payload);

VALUE rb_git_mirror(int argc, VALUE *argv, VALUE self) {
  VALUE remote_url, path, access_token;
  rb_scan_args(argc, argv, "21", &remote_url, &path, &access_token);

  int error;
  struct credentials_s credentials = { NULL, 0 };

  git_repository *repository = NULL;

  git_clone_options clone_options = GIT_CLONE_OPTIONS_INIT;
  clone_options.bare = true;
  clone_options.fetch_opts.download_tags = GIT_REMOTE_DOWNLOAD_TAGS_ALL;
  clone_options.remote_cb = create_remote;

  if (access_token != Qnil) {
    credentials.access_token = StringValuePtr(access_token);
    clone_options.fetch_opts.callbacks.credentials = cb_cred_access_token;
    clone_options.fetch_opts.callbacks.payload     = &credentials;
  }

  error = git_clone(&repository, StringValueCStr(remote_url), StringValueCStr(path), &clone_options);

  git_repository_free(repository);

  if (error < 0) {
    raise_exception(error);
  }

  return Qnil;
}

int create_remote(git_remote **out, git_repository *repo, const char *name, const char *url, void *payload) {
  GIT_UNUSED(payload);

  int error;
  git_config *cfg = NULL;
  char *mirror_config = NULL;

  /* Create the repository with a mirror refspec */
  if ((error = git_remote_create_with_fetchspec(out, repo, name, url, "+refs/*:refs/*")) < 0)
    return error;

  /* Set the mirror setting to true on this remote */
  if ((error = git_repository_config(&cfg, repo)) < 0)
    return error;

  if (asprintf(&mirror_config, "remote.%s.mirror", name) == -1) {
    giterr_set_str(GITERR_OS, "asprintf failed");
    git_config_free(cfg);
    return -1;
  }

  error = git_config_set_bool(cfg, mirror_config, true);

  free(mirror_config);
  git_config_free(cfg);

  return error;
}
