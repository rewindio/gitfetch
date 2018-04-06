#include "gitfetch.h"

int branch_exist(git_repository *repository, const char *name) {
  int error;
  git_reference *branch = NULL;

  error = git_branch_lookup(&branch, repository, name, GIT_BRANCH_LOCAL);
  git_reference_free(branch);

  if (error == GIT_ENOTFOUND) {
    return false;
  } else if (error < 0) {
    return error;
  }

  return true;
}

int copy_branch(git_repository *repository, git_reference *src_branch, const char *dst_name) {
  int error;
  git_annotated_commit *commit = NULL;
  git_reference *new_branch = NULL;

  if((error = git_annotated_commit_from_ref(&commit, repository, src_branch)) == GIT_OK) {
    error = git_branch_create_from_annotated(&new_branch, repository, dst_name, commit, false);
    git_reference_free(new_branch);
  }

  git_annotated_commit_free(commit);

  return error;
}

const char *branch_name(git_reference *branch) {
  const char *name = git_reference_shorthand(branch);

  if(strncmp("origin/", name, 7) == 0) {
    name += 7;
  }

  return name;
}

void *git_local_checkout_cb(void* data) {
  struct cb_args *args = data;
  int error;

  git_repository *repository = NULL;
  git_branch_iterator *iterator = NULL;
  git_reference *branch = NULL;
  git_branch_t branch_type;
  git_clone_options clone_options = GIT_CLONE_OPTIONS_INIT;

  error = git_clone(&repository, args->src, args->dst, &clone_options);

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
  }

  git_branch_iterator_free(iterator);
  git_repository_free(repository);

  args->error = error;

  return &args->error;
}

/*
 * Clones a bare repository into a local repository, checks out all branches and
 * removes remote/origin
 *
 * @param src_path [String] path of the repository to be cloned
 * @param dst_path [String] path where the cloned repository will be written into
 * @return [nil]
 */
VALUE rb_git_local_checkout(VALUE self, VALUE src_path, VALUE dst_path) {
  int *error;

  struct cb_args args = {
    .src = StringValueCStr(src_path),
    .dst = StringValueCStr(dst_path)
  };

  error = rb_thread_call_without_gvl(git_local_checkout_cb, &args, RUBY_UBF_IO, NULL);

  if (*error < 0) {
    raise_exception(*error);
  }

  return Qnil;
}

void Init_gitfetch_local_checkout() {
  rb_define_module_function(rb_mGit, "local_checkout", rb_git_local_checkout, 2);
}
