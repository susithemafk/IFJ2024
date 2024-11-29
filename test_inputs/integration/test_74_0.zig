const ifj = @import("ifj24.zig");

pub fn main() void {
    const x: i32 = 42; // Variable
    const y: i32 = 11; // Constant i32

    // Example 1: Valid comparisons
    if (x == y) {
        ifj.write("x == y is valid.\n");
    } else {
        ifj.write("x == y failed.\n");
    }

    // Example 2: Mixed arithmetic
    if (x + 1 == y) {
        ifj.write("x + 1 == y is valid.\n");
    } else {
        ifj.write("x + 1 == y failed.\n");
    }

    if (2 * x - (10 + y) == 36 - x) {
        ifj.write("Complex arithmetic comparison is valid.\n");
    } else {
        ifj.write("Complex arithmetic comparison failed.\n");
    }
}
