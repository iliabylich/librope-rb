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

  %w[asd123 456абв].each do |sample|
    context "for #{sample}" do
      let(:str) { sample }
      subject(:rope) { Librope::Rope.new(str) }

      context '#length' do
        it { expect(rope.length).to eq(str.length) }
      end

      context '#bytesize' do
        it { expect(rope.bytesize).to eq(str.bytesize) }
      end

      context '#to_s' do
        it { expect(rope.to_s).to eq(str) }
      end
    end
  end
end
