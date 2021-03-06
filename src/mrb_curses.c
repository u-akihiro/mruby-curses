/*
** mrb_curses.c - Curses class
**
** Copyright (c) Kotaro Hibi 2014
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"
#include "mrb_curses.h"
#include <ncurses.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

// static mrb_value rb_stdscr;

typedef struct {
  char *str;
  int len;
} mrb_curses_data;

static const struct mrb_data_type mrb_curses_data_type = {
  "mrb_curses_data", mrb_free,
};

static mrb_value mrb_curses_init(mrb_state *mrb, mrb_value self)
{
  mrb_curses_data *data;

  data = (mrb_curses_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_curses_data_type;
  DATA_PTR(self) = NULL;

  data = (mrb_curses_data *)mrb_malloc(mrb, sizeof(mrb_curses_data));
  data->str = "dummy";
  data->len = 5;
  DATA_PTR(self) = data;

  return self;
}

static mrb_value
mrb_curses_initscr(mrb_state *mrb, mrb_value self)
{
  // if (rb_stdscr) return rb_stdscr;
  initscr();
  /*
  if (stdscr == 0) {
    rb_raise(rb_eRuntimeError, "can't initialize curses");
  }
  */
  clear();
  // rb_stdscr = prep_window(cWindow, stdscr);
  // return rb_stdscr;
  return mrb_bool_value(true);
}

static mrb_value
mrb_curses_endwin(mrb_state *mrb, mrb_value self)
{
  endwin();
  return mrb_bool_value(true);
}

static mrb_value
mrb_curses_cbreak(mrb_state *mrb, mrb_value self)
{
  cbreak();
  return mrb_bool_value(true);
}

static mrb_value
mrb_curses_noecho(mrb_state *mrb, mrb_value self)
{
  noecho();
  return mrb_bool_value(true);
}

static mrb_value
mrb_curses_clear(mrb_state *mrb, mrb_value self)
{
  clear();
  return mrb_bool_value(true);
}

static mrb_value
mrb_curses_addstr(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;

  mrb_get_args(mrb, "S", &obj);
  const char *body = mrb_string_value_ptr(mrb, obj);
  addstr(body);
  return mrb_bool_value(true);
}

static mrb_value
mrb_curses_keypad(mrb_state *mrb, mrb_value self)
{
  mrb_bool obj = TRUE;
  mrb_get_args(mrb, "|b", &obj);
  keypad(stdscr, obj);
  return mrb_bool_value(true);
}

static mrb_value
mrb_curses_move(mrb_state *mrb, mrb_value self)
{
  mrb_value v1, v2;
  mrb_get_args(mrb, "ii", &v1, &v2);
  move(mrb_fixnum(v1), mrb_fixnum(v2));
  return mrb_bool_value(true);
}

void mrb_mruby_curses_gem_init(mrb_state *mrb)
{
    struct RClass *curses;
    curses = mrb_define_class(mrb, "Curses", mrb->object_class);
    MRB_SET_INSTANCE_TT(curses, MRB_TT_DATA);

    mrb_define_method(mrb, curses, "initialize", mrb_curses_init, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, curses, "initscr", mrb_curses_initscr, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, curses, "cbreak", mrb_curses_cbreak, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, curses, "noecho", mrb_curses_noecho, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, curses, "keypad", mrb_curses_keypad, MRB_ARGS_ANY());
    mrb_define_class_method(mrb, curses, "clear", mrb_curses_clear, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, curses, "addstr", mrb_curses_addstr, MRB_ARGS_REQ(1));
    mrb_define_class_method(mrb, curses, "move", mrb_curses_move, MRB_ARGS_ANY());
    mrb_define_class_method(mrb, curses, "endwin", mrb_curses_endwin, MRB_ARGS_NONE());
    DONE;
}

void mrb_mruby_curses_gem_final(mrb_state *mrb)
{
}

