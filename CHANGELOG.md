### 0.1.0 (2018-10-16)
- git-push: auto-detect number of threads used by the packbuilder
- update libgit2 to version 0.27.5
- new methods: Git.disable_protocol & Git.enable_protocol
- new error class: Git::UserError
- update README & .gemspec

### 0.0.9 (2018-08-06)

- update libgit2 to version 0.27.4

### 0.0.8 (2018-08-01)

- update libgit2 to version 0.27.3
- update .gemspec & README

### 0.0.7 (2018-04-06)

- enable parallel execution (releasing GIL before each method call)
- improve docs (using YARD)
- add license (MIT)
- update libgit2 to version 0.27.0

### 0.0.6 (2017-10-24)

- http-authentication now sends
  `username: "x-access-token", password: access_token`

### 0.0.5 (2017-10-10)

- update libgit2 to commit 35cb7b8 (fix git submodule init)

### 0.0.4 (2017-10-10)

- fix error check
- fix fetch remote with access_token
- update libgit2 to version 0.26.0

### 0.0.3 (2017-05-25)

- prune _before_ fetch
- local_checkout.c:
  Instead of a fetch, libgit2 will copy the object
  database directory, which is faster. If possible,
  it will hardlink the files to save space.
- release GIL before calling git_clone
