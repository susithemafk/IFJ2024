// bad function paramater, string (should not be allowed)
const ifj = @import("ifj24.zig");
// Hlavni funkce
pub fn main() void {
    string_input_test("hello"); // should not be allowed
}
// pomocna funkce, která bere jako argument string
pub fn string_input_test(a: []u8) void {}
