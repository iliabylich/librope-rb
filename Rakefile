# frozen_string_literal: true

require 'bundler/gem_tasks'
require 'rspec/core/rake_task'
require 'rake/extensiontask'

RSpec::Core::RakeTask.new(:spec)
Rake::ExtensionTask.new(:librope_native)

task default: %w[compile spec]
