#include "gitfetch.h"

/*
 * Returns the version String of libgit2
 *
 * @return [String] libgit2 version string
 */
VALUE rb_git_libgit2_version(VALUE self) {
  int major, minor, rev;

  git_libgit2_version(&major, &minor, &rev);

  VALUE version = rb_sprintf("%d.%d.%d", major, minor, rev);
  return version;
}

void Init_gitfetch_libgit2_version() {
  rb_define_module_function(rb_mGit, "libgit2_version", rb_git_libgit2_version, 0);
}
