#include "rope.h"
#include <ruby.h>

void Init_librope_native() {
  rb_p(rb_str_new2("Loading librop_native"));
}
