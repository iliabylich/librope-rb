#include "rope.h"
#include <ruby.h>

VALUE rb_mLibrope;
VALUE rb_cRope;

static void debug(const char* str) {
  rb_p(rb_str_new2(str));
}

static void rope_dealloc(rope *r) {
  rope_free(r);
}

static VALUE rope_alloc(VALUE rope_klass) {
  rope *r;
  r = rope_new();
  return Data_Wrap_Struct(rope_klass, NULL, rope_dealloc, r);
}

static VALUE rope_alloc_with(VALUE rope_klass, rope *r) {
  return Data_Wrap_Struct(rope_klass, NULL, rope_dealloc, r);
}

#define INIT_ROPE(ruby_obj, r) \
  rope *r; \
  Data_Get_Struct(ruby_obj, rope, r);

static VALUE rb_rope_initialize(int argc, VALUE* argv, VALUE self) {
  if (argc == 0) {
    // noop, empty rope has been created by allocator
  } else if (argc == 1) {
    VALUE str = argv[0];
    Check_Type(str, T_STRING);
    char *raw_str = StringValueCStr(str);

    rope *r;
    Data_Get_Struct(self, rope, r);

    rope_insert(r, 0, (uint8_t*) raw_str);
  } else {
    rb_raise(rb_eArgError, "wrong number of arguments, expected 0 or 1");
  }

  return Qnil;
}

static VALUE rb_rope_copy(VALUE self) {
  INIT_ROPE(self, r);
  rope *copy = rope_copy(r);
  return rope_alloc_with(rb_cRope, copy);
}

static VALUE rb_rope_length(VALUE self) {
  INIT_ROPE(self, r);
  size_t length = rope_char_count(r);
  return LONG2FIX(length);
}

static VALUE rb_rope_bytesize(VALUE self) {
  INIT_ROPE(self, r);
  size_t bytesize = rope_byte_count(r);
  return LONG2FIX(bytesize);
}

static VALUE rb_rope_to_s(VALUE self) {
  INIT_ROPE(self, r);
  uint8_t *str = rope_create_cstr(r);
  return rb_utf8_str_new_cstr((char*) str);
}

static VALUE rb_rope_insert(VALUE self, VALUE pos, VALUE str) {
  Check_Type(pos, T_FIXNUM);
  Check_Type(str, T_STRING);

  INIT_ROPE(self, r);

  char *raw_str = StringValueCStr(str);

  rope_insert(r, FIX2LONG(pos), (uint8_t*) raw_str);

  return Qtrue;
}

static VALUE rb_rope_delete(VALUE self, VALUE pos, VALUE num) {
  Check_Type(pos, T_FIXNUM);
  Check_Type(num, T_FIXNUM);

  INIT_ROPE(self, r);

  rope_del(r, FIX2LONG(pos), FIX2LONG(num));

  return Qtrue;
}

void Init_librope_native() {
  debug("Loading librope_native");

  rb_mLibrope = rb_define_module("Librope");

  rb_cRope = rb_define_class_under(rb_mLibrope, "Rope", rb_cObject);
  rb_define_alloc_func(rb_cRope, rope_alloc);

  rb_define_method(rb_cRope, "initialize", rb_rope_initialize, -1);
  rb_define_method(rb_cRope, "dup", rb_rope_copy, 0);
  rb_define_method(rb_cRope, "clone", rb_rope_copy, 0);
  rb_define_method(rb_cRope, "length", rb_rope_length, 0);
  rb_define_method(rb_cRope, "bytesize", rb_rope_bytesize, 0);
  rb_define_method(rb_cRope, "insert", rb_rope_insert, 2);
  rb_define_method(rb_cRope, "delete", rb_rope_delete, 2);
  rb_define_method(rb_cRope, "to_s", rb_rope_to_s, 0);
}
