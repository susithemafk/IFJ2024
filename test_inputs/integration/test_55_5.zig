const ifj = @import("ifj24.zig");

pub fn add(a: i32, b: i32) i32 {
    return a + b;
}

pub fn foo(a: i32) void {
    a = a + 4;
    return;
}

pub fn main() void {
    const num1 = 42;
    const num2 = 3;
    const sum = add(num1, num2);
    _ = foo(sum);   //chyba - přiřazení do konstanty
    ifj.write("The sum is: ");
    ifj.write(sum);
}