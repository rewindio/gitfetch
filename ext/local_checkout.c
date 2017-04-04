#include "gitfetch.h"

int branch_exist(git_repository *repository, const char *name);
void check_exception(int error);
int copy_branch(git_repository *repository, git_reference *src_branch, const char *dst_name);
const char *branch_name(git_reference *branch);

VALUE rb_git_local_checkout(VALUE self, VALUE src_path, VALUE dst_path) {
  int error;

  git_repository *repository;
  git_branch_iterator *iterator;
  git_reference *branch;
  git_branch_t branch_type;
  git_clone_options clone_options = GIT_CLONE_OPTIONS_INIT;

  VALUE file_url = rb_sprintf("file://%"PRIsVALUE, src_path);

  error = git_clone(&repository, StringValueCStr(file_url), StringValueCStr(dst_path), &clone_options);

  if (error == GIT_OK) {
    error = git_branch_iterator_new(&iterator, repository, GIT_BRANCH_REMOTE);

    while (error == GIT_OK) {
      error = git_branch_next(&branch, &branch_type, iterator);
      if (error == GIT_OK) {
        const char *name = branch_name(branch);

        if ((error = branch_exist(repository, name)) == true) {
          error = GIT_OK;
        } else if (error == false) {
          error = copy_branch(repository, branch, name);
        }
      }
    }

    if (error == GIT_ITEROVER) {
      error = git_remote_delete(repository, "origin");
    }

    git_branch_iterator_free(iterator);
    git_repository_free(repository);
  }

  check_exception(error);

  return Qnil;
}

int branch_exist(git_repository *repository, const char *name) {
  int error;
  git_reference *branch;

  error = git_branch_lookup(&branch, repository, name, GIT_BRANCH_LOCAL);
  git_reference_free(branch);

  if (error == GIT_ENOTFOUND) {
    return false;
  } else if (error < 0) {
    return error;
  }

  return true;
}

inline void check_exception(int error) {
  if (error < 0) {
    raise_exception(error);
  }
}

int copy_branch(git_repository *repository, git_reference *src_branch, const char *dst_name) {
  int error;
  git_annotated_commit *commit;
  git_reference *new_branch;

  if((error = git_annotated_commit_from_ref(&commit, repository, src_branch)) == GIT_OK) {
    if((error = git_branch_create_from_annotated(&new_branch, repository, dst_name, commit, false)) == GIT_OK) {
      git_reference_free(new_branch);
    }
    git_annotated_commit_free(commit);
  }

  return error;
}

const char *branch_name(git_reference *branch) {
  const char *name = git_reference_shorthand(branch);

  if(strncmp("origin/", name, 7) == 0) {
    name += 7;
  }

  return name;
}
