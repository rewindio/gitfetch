# gitfetch

Clone/fetch (or push to) git repositories.
All method calls are non-blocking, so cloning/fetching/pushing multiple
repositories concurrently is possible using Threads.

Supports GitHub authentication tokens for remote repositories using HTTP basic auth.

## Documentation

API docs, see here: https://rubydoc.info/gems/gitfetch

## Installation

place

```ruby
gem 'gitfetch'
```

in your `Gemfile` and execute

```
bundle install
```

## Usage

```ruby
require 'gitfetch'

# Git.fetch(repository, access_token=nil, update_head=false)
# connects to remote "origin" of +repository_path+ using +access_token+ as
# username for authentication:
#
# - prunes tracking refs that are no longer present on remote
# - downloads new data and update tips
# - optionally make the repository HEAD point to the remote's default branch
#
# example:
Git.fetch('repository.git', '6a61e7dddd28c...', true)

# Git.mirror(remote_url, path, access_token)
# Set up a (bare) mirror of the remote repository.
# example:
Git.mirror('https://github.com/octokit/Spoon-Knife.git', '/data/git/Spoon-Knife.git', '6a61e7dddd28c...')

# Git.push(repository_path, push_url, access_token)
# Pushes local repository to remote push_url
# example:
Git.push('/data/git/repository.git', 'https://github.com/backhub/new_repository.git', '6a61e7dddd28c...')
```

# Git.libgit2_version
# Returns the version String of libgit2
# example:
Git.libgit2_version

## Why

*gitfetch* was built @[BackHub](https://backhub.co) because we needed a tool to backup GitHub
repositories. We use [Sidekiq](https://sidekiq.org) for running jobs in multiple threads.
[rugged](https://github.com/libgit2/rugged) is not thread-safe/blocking background threads.

*gitfetch* features a simple API (clone/fetch/push) and can run in multiple threads concurrently.

---
made with :heart: by Christian Schlack@[BackHub](https://backhub.co)
