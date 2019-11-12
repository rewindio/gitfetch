#include "gitfetch.h"

/*
 * @overload remote_url(repository_path)
 *
 *   gets the URL of remote "origin" for +repository_path+
 *
 *   @param repository_path [String] the path to the repository
 *   @return [String]
 *
 *   @example
 *     Git.remote_url('repository.git')
 *     => "https://github.com/libgit2/libgit2"
 */
VALUE rb_git_remote_url(VALUE self, VALUE repository_path) {
  int error;
  git_repository *repository = NULL;
  git_remote *remote = NULL;
  VALUE remote_url = Qnil;

  error = git_repository_open(&repository, StringValueCStr(repository_path));

  if (error == GIT_OK) {
    error = git_remote_lookup(&remote, repository, "origin");

    if (error == GIT_OK) {
      remote_url = rb_str_new_cstr(git_remote_url(remote));
    }
    git_remote_free(remote);
  }

  git_repository_free(repository);

  if (error < 0) {
    raise_exception(error);
  }

  return remote_url;
}

/*
 * @overload set_remote_url(repository_path, remote_url)
 *
 *   sets the URL of remote "origin" for +repository_path+ to +remote_url+
 *
 *   @param repository_path [String] the path to the repository
 *   @param remote_url [String] remote url
 *   @return [nil]
 *
 *   @example
 *     Git.set_remote_url('repository.git', 'https://github.com/backhub/backhub.git')
 */
VALUE rb_git_set_remote_url(VALUE self, VALUE repository_path, VALUE remote_url) {
  int error;
  git_repository *repository = NULL;

  error = git_repository_open(&repository, StringValueCStr(repository_path));
  if (error == GIT_OK) {
    error = git_remote_set_url(repository, "origin", StringValueCStr(remote_url));
  }

  git_repository_free(repository);

  if (error < 0) {
    raise_exception(error);
  }

  return Qnil;
}

void Init_gitfetch_remote_url() {
  rb_define_module_function(rb_mGit, "remote_url", rb_git_remote_url, 1);
  rb_define_module_function(rb_mGit, "set_remote_url", rb_git_set_remote_url, 2);
}
