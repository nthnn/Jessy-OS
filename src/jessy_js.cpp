/*
 * This file is part of the Jessy OS.
 * Copyright (c) 2023 Nathanne Isip
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <duktape.h>

#include "jessy_agent.h"
#include "jessy_io.h"
#include "jessy_js.h"
#include "jessy_util.h"

static duk_context *ctx;

static duk_ret_t io_print(duk_context *context) {
    JessyIO::print(String(duk_to_string(context, -1)));
    return 0;
}

static duk_ret_t io_println(duk_context *context) {
    JessyIO::println(String(duk_to_string(context, -1)));
    return 0;
}

static duk_ret_t io_scan(duk_context *context) {
    String str = JessyIO::scan();
    duk_push_string(context, str.c_str());

    return 1;
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
    yield();
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

    duk_push_c_function(ctx, io_scan, 0);
    duk_put_prop_string(ctx, -2, "scan");

    duk_put_global_string(ctx, "IO");
}