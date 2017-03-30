#include "gitfetch.h"

VALUE method_libgit2_version(VALUE self) {
  int major, minor, rev;

  git_libgit2_version(&major, &minor, &rev);

  VALUE version = rb_sprintf("%d.%d.%d", major, minor, rev);
  return version;
}