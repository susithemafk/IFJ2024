const ifj = @import("ifj24.zig");

pub fn main() void {
    var x: i32 = 42;
    const z: f64 = 9.2;

    // Invalid implicit conversion: z == x (z has non-integer part)
    // Expected to fail.
    if (x == z) {
        ifj.write("x == z is invalid but passed.\n");
    } else {
        ifj.write("x == z is invalid and correctly failed.\n");
    }

    // Invalid comparison: x == 9.2
    // Expected to fail.
    if (x == 9.2) {
        ifj.write("x == 9.2 is invalid but passed.\n");
    } else {
        ifj.write("x == 9.2 is invalid and correctly failed.\n");
    }
}
