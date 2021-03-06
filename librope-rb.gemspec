# frozen_string_literal: true

lib = File.expand_path('lib', __dir__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'librope/version'

Gem::Specification.new do |spec|
  spec.name          = 'librope-rb'
  spec.version       = Librope::VERSION
  spec.authors       = ['Ilya Bylich']
  spec.email         = ['ibylich@gmail.com']

  spec.summary       = 'Ruby bindings for librope.'
  spec.description   = 'librope is a C implementation of Rope strings.'
  spec.homepage      = 'https://github.com/iliabylich/librope-rb'

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files         = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end
  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']

  spec.add_development_dependency 'bundler', '~> 2.2'
  spec.add_development_dependency 'rake', '~> 13.0'
  spec.add_development_dependency 'rake-compiler', '~> 1.0.7'
  spec.add_development_dependency 'rspec', '~> 3.0'
end
