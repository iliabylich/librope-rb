# frozen_string_literal: true

require 'spec_helper'

RSpec.describe Librope do
  it 'has a version number' do
    expect(Librope::VERSION).not_to be nil
  end
end

RSpec.describe Librope::Rope do
  let(:max_node_size) { Librope::Rope::MAX_NODE_SIZE }

  it 'can be instantiated without arguments' do
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

      context '#delete' do
        before { rope.delete(2, 3) }
        it { expect(rope.to_s).to eq(str[0..1] + str[5..-1]) }
      end

      context '#print' do
        subject(:rope) { Librope::Rope.new(str + 'a' * max_node_size * 3) }
        let(:dummy_stdout) { StringIO.new }

        it 'prints a tree-like representation of the rope' do
          begin
            original, $stdout = $stdout, dummy_stdout
            rope.print
            dummy_stdout.rewind
          ensure
            $stdout = original
          end

          output = dummy_stdout.read

          # That's the title
          expect(output).to include('HEAD')

          # At least one node must be full
          expect(output).to include(('a' * max_node_size).inspect)
        end
      end

      context '#each_node' do
        # Has 3 nodes
        #   + empty root
        #   + full left child (MAX_NODE_SIZE bytes)
        #   + 4 bytes in the right child
        subject(:rope) { Librope::Rope.new('a' * (max_node_size + 4)) }

        it 'yields all nodes; top-to-bottom, left-to-right' do
          expect { |b| rope.each_node(&b) }.to yield_successive_args(
            { string: '',                  length: 0,             bytesize: 0             },
            { string: 'a' * max_node_size, length: max_node_size, bytesize: max_node_size },
            { string: 'a' * 4,             length: 4,             bytesize: 4             }
          )
        end
      end
    end
  end
end
