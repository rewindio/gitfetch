#include <git2/sys/transport.h>
#include "gitfetch.h"

static int dummy_transport(git_transport **transport, git_remote *owner, void *param) {
  GIT_UNUSED(transport);
  GIT_UNUSED(owner);
  GIT_UNUSED(param);

  giterr_set_str(GITERR_NONE, "dummy transport: remote operation blocked");
  return GIT_ERROR;
}

/*
 * @overload disable_protocol
 *   disables remote operations for *protocol*
 *   @param protocol [String] the transport protocol (e. g. 'https')
 *   @return [true,false]
 *
 *   @example
 *     Git.disable_protocol('https')
 */
VALUE rb_disable_protocol(VALUE self, VALUE protocol) {
  int error = git_transport_register(StringValueCStr(protocol), &dummy_transport, NULL);

  if (error == GIT_EEXISTS) {
    // dummy_transport already registered for this protocol
    return Qfalse;
  }

  if (error < 0) {
    raise_exception(error);
  }

  return Qtrue;
}

/*
 * @overload enable_protocol
 *   re-enables previosly disabled remote operations for *protocol* 
 *   @param protocol [String] the transport protocol (e. g. 'https')
 *   @return [true,false]
 *
 *   @example
 *     Git.enable_protocol('https')
 */
VALUE rb_enable_protocol(VALUE self, VALUE protocol) {
  int error = git_transport_unregister(StringValueCStr(protocol));

  if (error == GIT_ENOTFOUND) {
    // no transport registered for this protocol
    return Qfalse;
  }

  if (error < 0) {
    raise_exception(error);
  }

  return Qtrue;
}

void Init_gitfetch_register_dummy_transport() {
  rb_define_module_function(rb_mGit, "disable_protocol", rb_disable_protocol, 1);
  rb_define_module_function(rb_mGit, "enable_protocol", rb_enable_protocol, 1);
}