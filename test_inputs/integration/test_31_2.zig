const ifj = @import("ifj24.zig");

pub fn foo() i32 {
    return 10;
}

pub fn main() void {
    const : []u8 res = foo();
}
