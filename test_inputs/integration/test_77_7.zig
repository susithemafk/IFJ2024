const ifj = @import("ifj24.zig");

pub fn main() void {
    var x: i32 = 42; // Variable
    const y: i32 = 11; // Constant i32
    const z: f64 = 10.0; // Constant f64
    var w: f64 = 5.0; // Variable f64

    // Mixed valid and invalid comparisons

    // Valid
    if (x == y) {
        ifj.write("x == y passed.\n");
    } else {
        ifj.write("x == y failed.\n");
    }
    if (10 == 10.0) {
        ifj.write("10 == 10.0 passed.\n");
    } else {
        ifj.write("10 == 10.0 failed.\n");
    }
    if (y == w + 6.0) {
        ifj.write("y == w + 6.0 passed.\n");
    } else {
        ifj.write("y == w + 6.0 failed.\n");
    }

    // Invalid
    if (x == 9.2) {
        ifj.write("x == 9.2 should have failed but passed.\n");
    } else {
        ifj.write("x == 9.2 correctly failed.\n");
    }

    if (z == x) {
        ifj.write("z == x should have failed but passed.\n");
    } else {
        ifj.write("z == x correctly failed.\n");
    }

    w = 10.0; // to not get unused variable warning
}
