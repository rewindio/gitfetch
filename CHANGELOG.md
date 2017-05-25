### 0.0.3

- prune _before_ fetch
- local_checkout.c:
  Instead of a fetch, libgit2 will copy the object
  database directory, which is faster. If possible,
  it will hardlink the files to save space.
- release GIL before calling git_clone
