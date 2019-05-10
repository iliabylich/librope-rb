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

  %w[abc абв].each do |sample|
    context "for #{sample}123" do
      let(:str) { sample + '123' }
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

      context '#insert' do
        it 'inserts at given position' do
          expect {
            rope.insert(3, '<inserted>')
          }.to change {
            rope.to_s
          }.to(sample + '<inserted>' + '123')
        end
      end

      shared_examples 'it creates a full copy' do |method_name|
        let!(:copy) { rope.public_send(method_name) }

        it 'copies the content' do
          expect(copy.to_s).to eq(rope.to_s)
        end

        it 'creates a full copy' do
          expect {
            rope.insert(0, 'x')
          }.to change {
            rope.to_s
          }.to('x' + str).and not_change {
            copy.to_s
          }
        end
      end

      context '#dup' do
        include_examples 'it creates a full copy', :dup
      end

      context '#clone' do
        include_examples 'it creates a full copy', :clone
      end
    end
  end
end
