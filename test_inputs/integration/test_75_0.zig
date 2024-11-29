const ifj = @import("ifj24.zig");

pub fn main() void {
    const y: i32 = 11; // Constant i32
    const z: f64 = 10.0; // Constant f64
    var w: f64 = 22.1; // Variable f64

    // Implicit conversion examples
    if (10 == 10.0) {
        ifj.write("10 == 10.0 is valid.\n");
    } else {
        ifj.write("10 == 10.0 failed.\n");
    }

    if (y == 11.0) {
        ifj.write("y == 11.0 is valid.\n");
    } else {
        ifj.write("y == 11.0 failed.\n");
    }

    if (w == 10) {
        ifj.write("w == 10 is valid.\n");
    } else {
        ifj.write("w == 10 failed.\n");
    }
}
