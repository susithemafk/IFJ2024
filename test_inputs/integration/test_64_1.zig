const ifj = @import("ifj24.zig");

pub fn bar(param : []u8) []u8 {
    const r = foo(param);
    return r;
}

pub fn foo(par : []u8) []u8 {
    const ret = bar(par);
    return ret;
}

pub fn main() void {
    const par ~ ifj.string("ahoj"); //error 1 - nepovolene znaky
    _ = bar(par);
}