#define _LITE_EXIT

/*
 *  Common definitions for atexit-like routines
 */

enum __atexit_types
{
  __et_atexit, //
  __et_onexit, //
  __et_cxa
};

extern void
__call_exitprocs (int, void*);

typedef void
(*exit_func_t) (void);

extern int
__register_exitproc (int, exit_func_t fn, void*, void*);

extern void
__run_fini_array (void);
