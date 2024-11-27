const ifj = @import("ifj24.zig");

pub fn main() void {
    const immutableVar = 10;
    immutableVar = 20; // Attempting to modify an immutable variable
}
