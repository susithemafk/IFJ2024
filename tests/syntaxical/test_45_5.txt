const ifj = @import("ifj24.zig");

pub fn main() void {
    const immutableVar = 42;
    var i: i32 = 0;
    while (i < 10) {
        immutableVar = i; // Attempting to modify a constant semantict error redefinition
        i = i + 1;
    }
}
