#include "gitfetch.h"

#define check_error(f) do { int error; if ((error = f) < 0) { git_remote_free(remote); return error; }} while(0)

int fetch_origin(git_repository *repository, char *access_token, int update_head) {
  git_remote *remote = NULL;
  git_buf default_branch = {0};

  // look up remote "origin"
  check_error(git_remote_lookup(&remote, repository, "origin"));

  struct credentials_s credentials = { access_token, 0 };
  // fetch remote
  git_fetch_options fetch_options = GIT_FETCH_OPTIONS_INIT;
  fetch_options.download_tags = GIT_REMOTE_DOWNLOAD_TAGS_ALL;
  fetch_options.prune = GIT_FETCH_NO_PRUNE;

  if (access_token) {
    fetch_options.callbacks.credentials = cb_cred_access_token;
    fetch_options.callbacks.payload     = &credentials;
  }

  check_error(git_remote_connect(remote, GIT_DIRECTION_FETCH, &fetch_options.callbacks,
                                 &fetch_options.proxy_opts, &fetch_options.custom_headers));

  credentials.count = 0;
  check_error(git_remote_prune(remote, &fetch_options.callbacks));

  credentials.count = 0;
  check_error(git_remote_fetch(remote, NULL, &fetch_options, NULL));

  // update HEAD for bare repositories
  if(update_head) {
    credentials.count = 0;
    if(git_remote_default_branch(&default_branch, remote) == GIT_OK) {
      check_error(git_repository_set_head(repository, default_branch.ptr));
      git_buf_dispose(&default_branch);
    }
  }
  git_remote_free(remote);

  return GIT_OK;
}

void *git_fetch_cb(void* data) {
  struct cb_args *args = data;

  git_repository *repository = NULL;

  // open repository "repository_path"
  args->error = git_repository_open(&repository, args->src);
  if (args->error == GIT_OK) {
    args->error = fetch_origin(repository, args->access_token, args->update_head);
  }

  git_repository_free(repository);

  return &(args->error);
}

/*
 * @overload fetch(repository_path, access_token=nil, update_head=false)
 *
 *   connects to remote "origin" of +repository_path+ using +access_token+ as
 *   username for authentication:
 *
 *   - prunes tracking refs that are no longer present on remote
 *   - downloads new data and update tips
 *   - make the repository HEAD point to the remote's default branch (optional)
 *
 *   @param repository_path [String] the path to the repository
 *   @param access_token [String] (optional) access token used for authentication
 *   @param update_head [Bool] (optional) set HEAD to remote HEAD
 *   @return [nil]
 *
 *   @example
 *     Git.fetch('repository.git', '6a61e7dddd28c...')
 */
VALUE rb_git_fetch(int argc, VALUE *argv, VALUE self) {
  int *error;
  VALUE repository_path, access_token, update_head;
  rb_scan_args(argc, argv, "12", &repository_path, &access_token, &update_head);

  struct cb_args args = {
    .src = StringValueCStr(repository_path),
    .update_head = ((update_head == Qtrue) ? 1 : 0)
  };

  if (access_token != Qnil) {
    args.access_token = StringValueCStr(access_token);
  }

  error = rb_thread_call_without_gvl(git_fetch_cb, &args, RUBY_UBF_IO, NULL);

  if (*error < 0) {
    raise_exception(*error);
  }

  return Qnil;
}

void Init_gitfetch_fetch() {
  rb_define_module_function(rb_mGit, "fetch", rb_git_fetch, -1);
}
