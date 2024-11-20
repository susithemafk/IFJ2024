const ifj = @import("ifj24.zig");

pub fn main() void {
    var a: i32 = 10;
    var b: f64 = 20.5;
    var c: i32 = a + b; // Adding `i32` and `f64` without explicit conversion
    _ = c;
}
