const ifj = @import("ifj24.zig");

pub fn is_positive(n: i32) bool {
    return n > 0;
}

pub fn main() void {
    const input = ifj.readi32();
    if (input) |value| {
        if (is_positive(value)) {
            ifj.write("The number is positive.\n");
        } else {
            ifj.write("The number is not positive.\n");
        }
    } else {
        ifj.write("Invalid input.\n");
    }
}
