const ifj = @import("ifj24.zig");

pub fn divide(a: i32, b: i32) i32 {
    return a / b;
}

pub fn main() void {
    const numerator = 20;
    const denominator = 5;
    const quotient = divide(numerator, denominator);
    ifj.write("Quotient: ");
    ifj.write(quotient);
}
