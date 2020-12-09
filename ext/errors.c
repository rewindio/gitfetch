#include <string.h>
#include "gitfetch.h"

#define OR(a, b) a ? a : b
#define raise(e, m) rb_raise(e, "%s", OR(g_error->message, m))

void raise_exception(int error) {
  const git_error *g_error;

  if (error == GIT_OK) { return; }
  g_error = git_error_last();

  if (g_error) {
    switch (error) {
      case GIT_EAUTH:
        raise(rb_eGitAuthenticationError, "Authentication Error");
        break;

      case GIT_EEOF:
        raise(rb_eGitEofError, "Unexpected EOF");
        break;

      case GIT_EEXISTS:
        raise(rb_eGitExistsError, "Object exists");
        break;

      case GIT_ENOTFOUND:
        raise(rb_eGitNotFoundError, "Object not found");
        break;

      case GIT_EUSER:
        raise(rb_eGitUserError, "User defined error");
        break;

      default:
        switch (g_error->klass) {
          case GIT_ERROR_HTTP:
            raise(rb_eGitHTTPError, "HTTP Error");
            break;

          case GIT_ERROR_NET:
            if (strstr(g_error->message, "repository not exported")) {
              raise(rb_eGitRepositoryNotExportedError, NULL);
            } else {
              raise(rb_eGitNetworkError, "Network Error");
            }
            break;

          case GIT_ERROR_SSL:
            raise(rb_eGitSSLError, "SSL Error");
            break;

          default:
            rb_raise(rb_eGitError, "%s (%d)", OR(g_error->message, "Unknown Error"), g_error->klass);
            break;
        }
        break;
    }
  } else {
    rb_raise(rb_eGitError, "Unkown Error (%d)", error);
  }
}
