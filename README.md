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
```