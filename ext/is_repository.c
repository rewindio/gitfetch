#include "gitfetch.h"

/*
 * @overload is_repository?(repository_path)
 *
 * Checks whether +repository_path+ is a valid git repository
 *
 * @param repository_path [String] the path to the repository
 * @return [Bool] true or false
 */
VALUE rb_git_is_repository(VALUE self, VALUE repository_path) {
  git_repository *repository;
  int error;

  error = git_repository_open(&repository, StringValueCStr(repository_path));

  if(error == GIT_OK) {
    git_repository_free(repository);
    return Qtrue;
  }

  return Qfalse;
}

void Init_gitfetch_is_repository() {
  rb_define_module_function(rb_mGit, "is_repository?", rb_git_is_repository, 1);
}
