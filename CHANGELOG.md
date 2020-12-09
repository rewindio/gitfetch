### 0.2.2 (2020-12-09)
- New exception Git::SSLError
- Improve docs & code quality

### 0.2.1 (2020-10-18)
- Git.fetch: fix memory leak
- Git.update_head: fix memory leak
- Git.update_head: fix 'Object not found' error on empty repositories

### 0.2.0 (2020-10-14)
- breaking change: Replace Git.mirror by Git.clone_bare
- breaking change: Remove optional parameter 'update_head' from method Git.fetch
- Add method Git.update_head
- Update libgit2 to version 1.1.0
- New exception Git::RepositoryNotExportedError
- Don't update HEAD after Git.clone_bare

### 0.1.9 (2020-08-25)
- bugfix: raise Exception for unknown errors
- New class Git::HTTPError
- Update README

### 0.1.8 (2020-05-04)
- Update libgit2 to version 1.0.1

### 0.1.7 (2020-04-16)
- Update libgit2 to version 1.0.0

### 0.1.6 (2020-03-20)
- Update libgit2 to version 0.99.0
- Git.is_repository?: check whether a given path is a git repository
- Improve docs

### 0.1.5 (2020-01-15)
- update libgit2 to version 0.28.4
- Git.mirror: set HEAD to remote HEAD
- Git.fetch: optionally update HEAD to remote HEAD

### 0.1.4 (2019-11-12)
- add new method Git.remote_url
- add new method Git.set_remote_url

### 0.1.3 (2019-09-17)
- update libgit2 to version 0.28.3

### 0.1.2 (2019-03-22)
- update libgit2 to version 0.28.1

### 0.1.1 (2019-01-16)
- update libgit2 to version 0.27.7

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
