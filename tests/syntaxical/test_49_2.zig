const ifj = @import("ifj24.zig");

pub fn main() void {
    var c = ifj.string("test") + 42; // Mixing string and integer in an operation
    ifj.write(c);
}
