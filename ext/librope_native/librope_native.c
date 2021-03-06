#include "rope.h"
#include <ruby.h>

VALUE rb_mLibrope;
VALUE rb_cRope;

VALUE symString;
VALUE symLength;
VALUE symBytesize;

static void debug(const char* str) {
  rb_p(rb_str_new2(str));
}

static void rope_dealloc(rope *r) {
  rope_free(r);
}

static VALUE rope_alloc_with(VALUE rope_klass, rope *r) {
  return Data_Wrap_Struct(rope_klass, NULL, rope_dealloc, r);
}

static VALUE rope_alloc(VALUE rope_klass) {
  return rope_alloc_with(rope_klass, rope_new());
}

// Does something like "rope* r; Data_Get_Struct(obj, rope, r); return r;""
#define ExtractRope(obj) \
  ((rope*)rb_data_object_get(obj))
#define CurrentRope ExtractRope(self)

static VALUE rb_rope_initialize(int argc, VALUE* argv, VALUE self) {
  if (argc == 0) {
    // noop, empty rope has been created by allocator
  } else if (argc == 1) {
    VALUE str = argv[0];
    Check_Type(str, T_STRING);

    rope_insert(CurrentRope, 0, (uint8_t*) StringValueCStr(str));
  } else {
    rb_raise(rb_eArgError, "wrong number of arguments, expected 0 or 1");
  }

  return Qnil;
}

static VALUE rb_rope_copy(VALUE self) {
  return rope_alloc_with(CLASS_OF(self), rope_copy(CurrentRope));
}

static VALUE rb_rope_length(VALUE self) {
  return LONG2FIX(rope_char_count(CurrentRope));
}

static VALUE rb_rope_bytesize(VALUE self) {
  return LONG2FIX(rope_byte_count(CurrentRope));
}

static VALUE rb_rope_to_s(VALUE self) {
  rope *r = CurrentRope;
  uint8_t *str = rope_create_cstr(r);
  size_t bytesize = rope_byte_count(r);
  return rb_utf8_str_new((char*) str, bytesize);
}

static VALUE rb_rope_insert(VALUE self, VALUE pos, VALUE str) {
  Check_Type(pos, T_FIXNUM);
  Check_Type(str, T_STRING);

  rope_insert(CurrentRope, FIX2LONG(pos), (uint8_t*) (StringValueCStr(str)));

  return Qtrue;
}

static VALUE rb_rope_delete(VALUE self, VALUE pos, VALUE num) {
  Check_Type(pos, T_FIXNUM);
  Check_Type(num, T_FIXNUM);

  rope_del(CurrentRope, FIX2LONG(pos), FIX2LONG(num));

  return Qtrue;
}

#define print_formatted(fmt, ...)\
  rb_io_write(rb_stdout, rb_sprintf(fmt, ##__VA_ARGS__))

#define rope_node_to_ruby_string(node) rb_utf8_str_new((char*) node->str, node->num_bytes)
#define rope_node_length(node) LONG2FIX(rope_node_chars(node))
#define rope_node_bytesize(node) LONG2FIX(rope_node_num_bytes(node))

static VALUE rb_rope_print(VALUE self) {
  rope *r = CurrentRope;

  print_formatted("chars: %zd\tbytes: %zd\theight: %d\n", r->num_chars, r->num_bytes, r->head.height);

  print_formatted("HEAD");
  for (int i = 0; i < r->head.height; i++) {
    print_formatted(" |%3zd ", r->head.nexts[i].skip_size);
  }
  print_formatted("\n");

  int num = 0;
  ROPE_FOREACH(r, n) {
    print_formatted("%3d:", num++);
    for (int i = 0; i < n->height; i++) {
      print_formatted(" |%3zd ", n->nexts[i].skip_size);
    }
    print_formatted("        : \"");

    VALUE string = rope_node_to_ruby_string(n);
    print_formatted("%s", StringValueCStr(string));

    print_formatted("\"\n");
  }

  return Qnil;
}

static VALUE rb_rope_print_native(VALUE self) {
  _rope_print(CurrentRope);

  return Qtrue;
}

static VALUE rb_rope_each_node(VALUE self) {
  ROPE_FOREACH(CurrentRope, node) {
    VALUE rope_node_hash = rb_hash_new();

    rb_hash_aset(rope_node_hash, symString,   rope_node_to_ruby_string(node));
    rb_hash_aset(rope_node_hash, symLength,   rope_node_length(node));
    rb_hash_aset(rope_node_hash, symBytesize, rope_node_bytesize(node));

    rb_yield(rope_node_hash);
  }

  return Qnil;
}

void Init_librope_native() {
  debug("Loading librope_native");

  symString   = ID2SYM(rb_intern("string"));
  symLength   = ID2SYM(rb_intern("length"));
  symBytesize = ID2SYM(rb_intern("bytesize"));

  rb_mLibrope = rb_define_module("Librope");

  rb_cRope = rb_define_class_under(rb_mLibrope, "Rope", rb_cObject);
  rb_const_set(rb_cRope, rb_intern("MAX_NODE_SIZE"), LONG2FIX(ROPE_NODE_STR_SIZE));

  rb_define_alloc_func(rb_cRope, rope_alloc);

  rb_define_method(rb_cRope, "initialize", rb_rope_initialize, -1);
  rb_define_method(rb_cRope, "dup", rb_rope_copy, 0);
  rb_define_method(rb_cRope, "clone", rb_rope_copy, 0);
  rb_define_method(rb_cRope, "length", rb_rope_length, 0);
  rb_define_method(rb_cRope, "bytesize", rb_rope_bytesize, 0);
  rb_define_method(rb_cRope, "insert", rb_rope_insert, 2);
  rb_define_method(rb_cRope, "delete", rb_rope_delete, 2);
  rb_define_method(rb_cRope, "to_s", rb_rope_to_s, 0);
  rb_define_method(rb_cRope, "print", rb_rope_print, 0);
  rb_define_method(rb_cRope, "print_native", rb_rope_print_native, 0);
  rb_define_method(rb_cRope, "each_node", rb_rope_each_node, 0);
}
