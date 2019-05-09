# frozen_string_literal: true

require 'spec_helper'

RSpec.describe Librope do
  it 'has a version number' do
    expect(Librope::VERSION).not_to be nil
  end
end

RSpec.describe Librope::Rope do
  it 'can be instantiated' do
    expect(Librope::Rope.new).to be_a(Librope::Rope)
  end
end
