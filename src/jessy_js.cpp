#include <duktape.h>
#include <jessy_agent.h>
#include <jessy_io.h>
#include <jessy_js.h>
#include <jessy_util.h>

static duk_context *ctx;

static duk_ret_t io_print(duk_context *context) {
    JessyIO::print(String(duk_to_string(context, -1)));
    return 0;
}

static duk_ret_t io_println(duk_context *context) {
    JessyIO::println(String(duk_to_string(context, -1)));
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

void JessyDuktape::evalString(String script) {
    duk_int_t rc = duk_peval_string(ctx, script.c_str());
    if(rc != 0)
        duk_safe_to_stacktrace(ctx, -1);
    else duk_safe_to_string(ctx, -1);

    (void) duk_get_string(ctx, -1);
}

void JessyDuktape::initIOObject() {
    duk_push_c_function(ctx, io_print, 1);
    duk_put_prop_string(ctx, -2, "print");

    duk_push_c_function(ctx, io_println, 1);
    duk_put_prop_string(ctx, -2, "println");

    duk_put_global_string(ctx, "IO");
}