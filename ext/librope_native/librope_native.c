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
  rope *r;
  r = rope_new();
  return Data_Wrap_Struct(rope_klass, NULL, rope_dealloc, r);
}

static VALUE rb_rope_initialize(int argc, VALUE* argv, VALUE rope_klass) {
  if (argc == 0) {
    // noop, empty rope has been created by allocator
  } else if (argc == 1) {
    VALUE str = argv[0];
    Check_Type(str, T_STRING);
    char *raw_str = StringValueCStr(str);

    rope *r;
    Data_Get_Struct(rope_klass, rope, r);

    rope_insert(r, 0, (uint8_t*) raw_str);
  } else {
    rb_raise(rb_eArgError, "wrong number of arguments, expected 0 or 1");
  }

  return Qnil;
}

static VALUE rb_rope_length(VALUE self) {
  rope *r;
  Data_Get_Struct(self, rope, r);
  size_t length = rope_char_count(r);
  return LONG2FIX(length);
}

static VALUE rb_rope_bytesize(VALUE self) {
  rope *r;
  Data_Get_Struct(self, rope, r);
  size_t bytesize = rope_byte_count(r);
  return LONG2FIX(bytesize);
}

static VALUE rb_rope_to_s(VALUE self) {
  rope *r;
  Data_Get_Struct(self, rope, r);
  uint8_t *str = rope_create_cstr(r);
  return rb_utf8_str_new_cstr((char*) str);
}

void Init_librope_native() {
  debug("Loading librope_native");

  rb_mLibrope = rb_define_module("Librope");

  rb_cRope = rb_define_class_under(rb_mLibrope, "Rope", rb_cObject);
  rb_define_alloc_func(rb_cRope, rope_alloc);

  rb_define_method(rb_cRope, "initialize", rb_rope_initialize, -1);
  rb_define_method(rb_cRope, "length", rb_rope_length, 0);
  rb_define_method(rb_cRope, "bytesize", rb_rope_bytesize, 0);
  rb_define_method(rb_cRope, "to_s", rb_rope_to_s, 0);
}
