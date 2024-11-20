const ifj = @import("ifj24.zig");

pub fn is_even(n: i32) bool {
    return n % 2 == 0; // % is not a valid token
}

pub fn main() void {
    const num = ifj.readi32();
    if (num) |n| {
        if (is_even(n)) {
            ifj.write(n);
            ifj.write(" is even.\n");
        } else {
            ifj.write(n);
            ifj.write(" is odd.\n");
        }
    }
}
