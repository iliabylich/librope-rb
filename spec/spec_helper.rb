# frozen_string_literal: true

$LOAD_PATH.unshift File.expand_path('..', __dir__)

require 'bundler/setup'
require 'librope'

RSpec.configure do |config|
  config.disable_monkey_patching!

  config.expect_with :rspec do |c|
    c.syntax = :expect
  end
end

RSpec::Matchers.define_negated_matcher :not_change, :change
