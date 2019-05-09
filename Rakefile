# frozen_string_literal: true

require 'bundler/gem_tasks'
require 'rspec/core/rake_task'
require 'rake/extensiontask'

RSpec::Core::RakeTask.new(:spec)

Rake::ExtensionTask.new do |ext|
  ext.name    = 'librope_native'
  ext.ext_dir = 'ext/librope_native'
  ext.lib_dir = 'lib/librope'
end

task default: %w[compile spec]
