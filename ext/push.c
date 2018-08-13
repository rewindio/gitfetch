#include "gitfetch.h"

void *git_push_cb(void *data) {
  struct cb_args *args = data;
  int error;

  git_repository *repository = NULL;
  git_remote *remote = NULL;
  git_strarray refs;

  // open repository "repository_path"
  error = git_repository_open(&repository, args->src);
  if (error == GIT_OK) {
    error = git_remote_create_anonymous(&remote, repository, args->dst);
    if (error == GIT_OK) {
      struct credentials_s credentials = { args->access_token, 0 };
      git_push_options push_options = GIT_PUSH_OPTIONS_INIT;
      push_options.pb_parallelism = 0; // auto-detect the number of threads used by the packbuilder

      if (args->access_token) {
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

  args->error = error;

  return &(args->error);
}

/*
 * @overload push(repository_path, push_url, access_token=nil)
 *   Push local repository in +repository_path+ to remote repository identified by +push_url+.
 *   @param repository_path [String]
 *   @param push_url [String]
 *   @param access_token [String]
 *   @return [nil]
 *
 *   @example
 *     Git.push('/data/git/repository.git', 'https://github.com/backhub/new_repository.git', '6a61e7dddd28c...')
 */
VALUE rb_git_push(int argc, VALUE *argv, VALUE self) {
  int *error;
  VALUE repository_path, push_url, access_token;
  rb_scan_args(argc, argv, "21", &repository_path, &push_url, &access_token);

  struct cb_args args = {
    .src = StringValueCStr(repository_path),
    .dst = StringValueCStr(push_url)
  };

  if (access_token != Qnil) {
    args.access_token = StringValueCStr(access_token);
  }

  error = rb_thread_call_without_gvl(git_push_cb, &args, RUBY_UBF_IO, NULL);

  // check for errors and raise exception accordingly
  // raise Git::Error if error is not 0
  if (*error < 0) {
    raise_exception(*error);
  }

  return Qnil;
}

void Init_gitfetch_push() {
  rb_define_module_function(rb_mGit, "push", rb_git_push, -1);
}