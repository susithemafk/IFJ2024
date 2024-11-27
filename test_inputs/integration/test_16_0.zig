const ifj = @import("ifj24.zig");

pub fn square(n: i32) i32 {
    return n * n;
}

pub fn main() void {
    const input = ifj.readi32();
    if (input) |value| {
        const squared = square(value);
        ifj.write("Square: ");
        ifj.write(squared);
    } else {
        ifj.write("Error reading input.\n");
    }
}
