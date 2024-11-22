const ifj = @import("ifj24.zig");

pub fn add(a: i32, b: i32) i32 {
    return a + b;
}

pub fn main() void {
    const num1 = 42;
    const num2 = 58;
    const sum = add(num1, num2);
    ifj.write("The sum is: ");
    ifj.write(sum);
}
