const ifj = @import("ifj24.zig");

pub fn calculate_difference(a: i32, b: i32) i32 {
    return a - b;
}

pub fn main() void {
    const first = 50;
    const second = 30;
    const difference = calculate_difference(first, second);
    ifj.write("Difference: ");
    ifj.write(difference);
}
