// bad function paramater, string (should not be allowed)
const ifj = @import("ifj24.zig");
// Hlavni funkce
pub fn main() void {
    var a: []u8 = "hello"; // should not be allowed
    a = ifj.string("hello"); // should be allowed
}
