#include "gitfetch.h"

// callback: tries the access_token passed in through the payload ONCE
//           and returns GIT_EAUTH if called for a second time
int cb_cred_access_token(git_cred **out, const char *url, const char *username_from_url,
  unsigned int allowed_types, void *payload) {

  GIT_UNUSED(url);
  GIT_UNUSED(username_from_url);

  if (allowed_types & GIT_CREDTYPE_USERPASS_PLAINTEXT) {
    struct credentials_s *credentials = payload;

    if (credentials->count > 0) {
      giterr_set_str(GITERR_CALLBACK, "Invalid token");
      return GIT_EAUTH;
    } else {
      credentials->count++;
      return git_cred_userpass_plaintext_new(out, "x-access-token", credentials->access_token);
    }
  }

  return GIT_PASSTHROUGH;
}
