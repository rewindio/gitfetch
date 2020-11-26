#include "gitfetch.h"

int delete_reference(git_repository *repository, const char *name) {
  git_reference *reference;
  int error;

  error = git_reference_lookup(&reference, repository, name);
  if (error == GIT_OK) {
    error = git_reference_delete(reference);
    git_reference_free(reference);
  }

  return error;
}

void *git_clone_bare_cb(void *data) {
  struct cb_args *args = data;
  struct credentials_s credentials = { args->access_token, 0 };

  git_repository *repository = NULL;

  git_clone_options clone_options = GIT_CLONE_OPTIONS_INIT;
  clone_options.bare = true;
  clone_options.fetch_opts.download_tags = GIT_REMOTE_DOWNLOAD_TAGS_ALL;
  clone_options.remote_cb = (git_remote_create_cb) git_remote_create_with_fetchspec;
  clone_options.remote_cb_payload = (void *) "+refs/heads/*:refs/heads/*";

  if (args->access_token) {
    clone_options.fetch_opts.callbacks.credentials = cb_cred_access_token;
    clone_options.fetch_opts.callbacks.payload     = &credentials;
  }

  args->error = git_clone(&repository, args->src, args->dst, &clone_options);
  if (args->error == GIT_OK) {
    args->error = delete_reference(repository, "refs/remotes/origin/HEAD");
    git_repository_free(repository);
  }

  return &(args->error);
}

/*
 * @overload clone_bare(remote_url, path, access_token=nil)
 *   Set up a bare clone of the remote repository.
 *   @param remote_url [String]
 *   @param path [String]
 *   @param access_token [String]
 *   @return [nil]
 */
VALUE rb_git_clone_bare(int argc, VALUE *argv, VALUE self) {
  int *error;
  VALUE remote_url, path, access_token;
  rb_scan_args(argc, argv, "21", &remote_url, &path, &access_token);

  struct cb_args args = {
    .src = StringValueCStr(remote_url),
    .dst = StringValueCStr(path)
  };

  if (access_token != Qnil) {
    args.access_token = StringValueCStr(access_token);
  }

  error = rb_thread_call_without_gvl(git_clone_bare_cb, &args, RUBY_UBF_IO, NULL);

  if (*error < 0) {
    raise_exception(*error);
  }

  return Qnil;
}

void Init_gitfetch_clone_bare() {
  rb_define_module_function(rb_mGit, "clone_bare", rb_git_clone_bare, -1);
}
