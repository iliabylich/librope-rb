# frozen_string_literal: true

require 'mkmf'

$CFLAGS << ' -Wall -Werror -Wno-declaration-after-statement '
$CFLAGS << '-O2 -std=c99'
$INCFLAGS << ' -I$(srcdir)/../../vendor/librope'

create_makefile('librope_native')
