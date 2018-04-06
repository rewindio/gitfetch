Gem::Specification.new do |s|
  s.name                  = "gitfetch"
  s.version               = "0.0.7"
  s.date                  = Time.now.strftime('%Y-%m-%d')
  s.summary               = "Minimal wrapper around libgit2's git_remote_fetch()"
  s.homepage              = "https://github.com/backhub/gitfetch"
  s.email                 = "christian@backhub.co"
  s.authors               = ["Christian Schlack"]
  s.license               = "MIT"
  s.files                 = %w( README.md )
  s.files                 += Dir.glob("lib/**/*.rb")
  s.files                 += Dir.glob("ext/**/*.[ch]")
  s.files                 += Dir.glob("vendor/libgit2/cmake/**/*")
  s.files                 += Dir.glob("vendor/libgit2/{include,src,deps}/**/*")
  s.files                 += Dir.glob("vendor/libgit2/{CMakeLists.txt,Makefile.embed,AUTHORS,COPYING,libgit2.pc.in}")
  s.extensions            = ['ext/extconf.rb']
  s.required_ruby_version = '>= 1.9.3'
  s.description           = <<desc
gitfetch is a minimal wrapper around libgit2's git_remote_fetch().
It is used for testing libgit2 without rugged.
desc
end
