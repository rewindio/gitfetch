### 0.0.7

- enable parallel execution (releasing GIL before each method call)
- improve docs (using YARD)
- add license (MIT)
- update libgit2 to version 0.27.0

### 0.0.6

- http-authentication now sends
  `username: "x-access-token", password: access_token`

### 0.0.5

- update libgit2 to commit 35cb7b8 (fix git submodule init)

### 0.0.4

- fix error check
- fix fetch remote with access_token
- update libgit2 to version 0.26.0

### 0.0.3

- prune _before_ fetch
- local_checkout.c:
  Instead of a fetch, libgit2 will copy the object
  database directory, which is faster. If possible,
  it will hardlink the files to save space.
- release GIL before calling git_clone
