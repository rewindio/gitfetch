#include <ruby.h>
#include <ruby/thread.h>
#include "gitfetch.h"

struct clone_args {
  git_repository **repository;
  char *src_path;
  char *dst_path;
  git_clone_options *clone_options;
  int error;
};

void *clone_without_gvl_cb(void *data) {
  struct clone_args* args = data;
  args->error = git_clone(args->repository, args->src_path, args->dst_path, args->clone_options);
  return NULL;
}

int git_clone_without_gvl(git_repository **repository, char *src_path, char *dst_path, git_clone_options *clone_options) {
  struct clone_args args = { repository, src_path, dst_path, clone_options, GIT_OK };

  rb_thread_call_without_gvl(&clone_without_gvl_cb, (void *) &args, NULL, NULL);
  return args.error;
}
