const ifj = @import("ifj24.zig");

pub fn bar(a: i32, b: i32) i32 {
    return a + b;
}

pub fn main() void {
    var str: ?[]u8 = ifj.string("stringValue");
    const result = bar(10, str); // Passing a string instead of an integer
}
