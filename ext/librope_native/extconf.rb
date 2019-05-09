# frozen_string_literal: true

require 'mkmf'

$CFLAGS << ' -O2 -Wall -I. -std=c99'

extension_name = 'librope_native'
dir_config(extension_name)

$srcs = ['librope_native.c', 'vendor/rope.c']

$INCFLAGS << ' -I$(srcdir)/vendor'
$VPATH << '$(srcdir)/vendor'

create_makefile(extension_name)
