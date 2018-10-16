#include "gitfetch.h"

void raise_exception(int error) {
  const git_error *g_error = giterr_last();

  switch (error) {
    case GIT_EAUTH:
      rb_raise(rb_eGitAuthenticationError, "%s", (g_error && g_error->message) ? g_error->message : "Authentication Error");
      break;

    case GIT_EEOF:
      rb_raise(rb_eGitEofError, "%s", (g_error && g_error->message) ? g_error->message : "Unexpected EOF");
      break;

    case GIT_EEXISTS:
      rb_raise(rb_eGitExistsError, "%s", (g_error && g_error->message) ? g_error->message : "Object exists");
      break;

    case GIT_ENOTFOUND:
      rb_raise(rb_eGitNotFoundError, "%s", (g_error && g_error->message) ? g_error->message : "Object not found");
      break;

    case GIT_EUSER:
      rb_raise(rb_eGitUserError, "%s", (g_error && g_error->message) ? g_error->message : "User defined error");

    default:
      if (g_error) {
        if (g_error->klass == GITERR_NONE) {
          return;
        } else if (g_error->klass == GITERR_NET) {
          rb_raise(rb_eGitNetworkError, "%s", g_error->message ? g_error->message : "Network Error");
        }
      } else {
        rb_raise(rb_eGitError, "%s (%d)", (g_error && g_error->message) ? g_error->message : "Unkown Error",
          (g_error && g_error->klass) ? g_error->klass : error);
      }
  }
}