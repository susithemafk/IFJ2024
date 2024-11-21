const ifj = @import("ifj24.zig");

pub fn foo() i32 {
    if (a) |na| {
        return 10;
    } else  {
        return 2;
    }
    var a = ifj.string("not an integer"); // Returning string from a function expecting i32
    return a;
}

pub fn main() void {
    _ = foo();
}
