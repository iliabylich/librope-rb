#include "rope.h"
#include <ruby.h>

VALUE rb_mLibrope;
VALUE rb_cRope;

static void debug(const char* str) {
  rb_p(rb_str_new2(str));
}

static void rope_dealloc(rope *ptr) {
  rope_free(ptr);
}

static VALUE rope_alloc(VALUE rope_klass) {
  rope *data;
  data = rope_new();
  return Data_Wrap_Struct(rope_klass, NULL, rope_dealloc, data);
}

void Init_librope_native() {
  debug("Loading librope_native");

  rb_mLibrope = rb_define_module("Librope");

  rb_cRope = rb_define_class_under(rb_mLibrope, "Rope", rb_cObject);
  rb_define_alloc_func(rb_cRope, rope_alloc);
}
