#include <duktape.h>
#include <jessy_agent.h>
#include <jessy_io.h>
#include <jessy_js.h>
#include <jessy_util.h>

static duk_context *ctx;

static duk_ret_t io_print(duk_context *context) {
    duk_push_string(context, " ");
    duk_insert(context, 0);
    duk_join(context, duk_get_top(context) - 1);

    JessyIO::print(String(duk_to_string(context, -1)));
    return 0;
}

void JessyDuktape::init() {
    ctx = duk_create_heap_default();
    duk_push_object(ctx);

    JessyDuktape::initIOObject();
}

void JessyDuktape::free() {
    duk_pop(ctx);
    duk_destroy_heap(ctx);
}

void JessyDuktape::eval(String fileName) {
    duk_int_t rc = duk_peval_string(
        ctx,
        JessyIO::readFile(fileName).c_str()
    );

    if(rc != 0)
        duk_safe_to_stacktrace(ctx, -1);
    else duk_safe_to_string(ctx, -1);

    (void) duk_get_string(ctx, -1);
}

void JessyDuktape::initIOObject() {
    duk_push_c_function(ctx, io_print, DUK_VARARGS);
    duk_put_prop_string(ctx, -2, "print");
    duk_put_global_string(ctx, "IO");
}