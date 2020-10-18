#include "gitfetch.h"

void *git_update_head_cb(void *data) {
  struct cb_args *args = data;
  struct credentials_s credentials = { args->access_token, 0 };

  git_repository *repository = NULL;
  git_fetch_options fetch_options = GIT_FETCH_OPTIONS_INIT;

  git_buf remote_head = {0};
  git_remote *remote = NULL;
  int error;

  if (args->access_token) {
    fetch_options.callbacks.credentials = cb_cred_access_token;
    fetch_options.callbacks.payload     = &credentials;
  }

  error = git_repository_open(&repository, args->src);
  if(error == GIT_OK) {
    // look up remote "origin"
    error = git_remote_lookup(&remote, repository, "origin");
    if(error == GIT_OK) {
      error = git_remote_connect(remote,
                                 GIT_DIRECTION_FETCH,
                                 &fetch_options.callbacks,
                                 &fetch_options.proxy_opts,
                                 &fetch_options.custom_headers);

      if(error == GIT_OK) {
        error = git_remote_default_branch(&remote_head, remote);

        if(error == GIT_ENOTFOUND) {
          error = GIT_OK; // Ignore GIT_ENOTFOUND on empty repositories
        } else if(error == GIT_OK) {
          error = git_repository_set_head(repository, remote_head.ptr);
          git_buf_dispose(&remote_head);
        }
      }
      git_remote_free(remote);
    }
  }

  args->error = error;

  git_repository_free(repository);
  return &args->error;
}

/*
 * @overload update_head(path, access_token=nil)
 *   Set HEAD to the remote 'origin' HEAD
 *   @param path [String]
 *   @param access_token [String]
 *   @return [nil]
 */
VALUE rb_git_update_head(int argc, VALUE *argv, VALUE self) {
  int *error;
  VALUE path, access_token;
  rb_scan_args(argc, argv, "11", &path, &access_token);

  struct cb_args args = {
    .src = StringValueCStr(path),
    .access_token = NULL
  };

  if (access_token != Qnil) {
    args.access_token = StringValueCStr(access_token);
  }

  error = rb_thread_call_without_gvl(git_update_head_cb, &args, RUBY_UBF_IO, NULL);

  if (*error < 0) {
    raise_exception(*error);
  }

  return Qnil;
}

void Init_gitfetch_update_head() {
  rb_define_module_function(rb_mGit, "update_head", rb_git_update_head, -1);
}
