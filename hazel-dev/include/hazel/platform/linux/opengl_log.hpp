#define gl_call(x)    \
    gl_clear_error(); \
    x;                \
    assert(gl_log(#x, __FILE__, __LINE__));

void gl_clear_error();

bool gl_log(const char *function, const char *file, int line);