const ifj = @import("ifj24.zig");

pub fn main() void {
    var a: i32 = 42;
    a = ifj.string("wrong type"); // Assigning a string to an integer
}
