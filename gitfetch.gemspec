Gem::Specification.new do |s|
  s.name                  = "gitfetch"
  s.version               = "0.1.0"
  s.date                  = Time.now.strftime('%Y-%m-%d')
  s.summary               = "Concurrently clone/fetch/push to (remote) git repositories"
  s.homepage              = "https://backhub.co"
  s.email                 = "christian@backhub.co"
  s.authors               = ["Christian Schlack"]
  s.license               = "MIT"
  s.files                 = %w( README.md CHANGELOG.md LICENSE )
  s.files                 += Dir.glob("ext/**/*.[ch]")
  s.files                 += Dir.glob("vendor/libgit2/{cmake,include,src,deps}/**/*")
  s.files                 += Dir.glob("vendor/libgit2/{CMakeLists.txt,Makefile.embed,AUTHORS,COPYING,libgit2.pc.in}")
  s.extensions            = ['ext/extconf.rb']
  s.metadata              = { "source_code_uri" => "https://github.com/backhub/gitfetch" }
  s.required_ruby_version = '>= 1.9.3'
  s.description           = <<desc
Clone/fetch (or push to) git repositories.
All method calls are non-blocking, so cloning/fetching/pushing multiple
repositories concurrently is possible using Threads.

Supports GitHub authentication tokens for remote repositories using HTTP basic auth.
desc
end
