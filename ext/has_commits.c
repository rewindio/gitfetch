#include "gitfetch.h"

/*
 * @overload has_commits?(repository_path)
 *
 *   returns true if the given repository comntains commits
 *
 *   @param repository_path [String] the path to the repository
 *   @return [Boolean]
 *
 *   @example
 *     Git.has_commits?('repository.git')
 *       => true
 */
VALUE rb_git_has_commits(VALUE self, VALUE repository_path) {
  git_repository *repository = NULL;
  git_reference *reference;
  int error;

  // open repository "repository_path"
  error = git_repository_open(&repository, StringValueCStr(repository_path));
  if (error == GIT_OK) {
    error = git_repository_head(&reference, repository);

    if (error == GIT_OK) {
      git_reference_free(reference);
    }

    git_repository_free(repository);

    switch(error) {
      case GIT_OK:
        return Qtrue;

      case GIT_ENOTFOUND:
      case GIT_EUNBORNBRANCH:
        return Qfalse;
    }
  }

  raise_exception(error);
  return Qfalse;
}

void Init_gitfetch_has_commits() {
  rb_define_module_function(rb_mGit, "has_commits?", rb_git_has_commits, 1);
}
