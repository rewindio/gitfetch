#ifndef GIT_UNUSED
  #define GIT_UNUSED(x) ((void)(x))
#endif

struct credentials_s {
  char *access_token;
  unsigned int count;
};

// initialization method - Ruby calls this, not you
void Init_git();

// method 'fetch' - methods are prefixed by 'method_' here
VALUE method_fetch(VALUE self, VALUE repository_name, VALUE access_token);