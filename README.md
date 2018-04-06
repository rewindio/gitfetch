# gitfetch

gitfetch is a minimal wrapper around libgit2's git_remote_fetch().
It is used for testing libgit2 without rugged.

## Installation

place

```
gem 'gitfetch', git: 'https://github.com/backhub/gitfetch.git', branch: 'master', submodules: true 
```

in your `Gemfile`

## Usage

```
require 'gitfetch'

# Git.fetch(repository, access_token)
# fetches remote/origin of repository using access_token as username for authentication
# example:
Git.fetch('repository.git', '6a61e7dddd28c...')

# Git.libgti2_version
# Returns the version String of libgit2
# example:
Git.libgit2_version

# Git.local_checkout(src_path, dst_path)
# Clones a bare repository into a local repository, checks out all branches and
# removes remote/origin
# example:
Git.local_checkout('/data/git/src_repository.git', '/data/downloads/dst_repository')

# Git.mirror(remote_url, path, access_token)
# Set up a mirror of the remote repository.
# example:
Git.mirror('https://github.com/octokit/Spoon-Knife.git', '/data/git/Spoon-Knife.git', '6a61e7dddd28c...')

# Git.push(repository_path, push_url, access_token)
# Pushes local repository to remote push_url
# example:
Git.push('/data/git/repository.git', 'https://github.com/backhub/new_repository.git', '6a61e7dddd28c...')
```
