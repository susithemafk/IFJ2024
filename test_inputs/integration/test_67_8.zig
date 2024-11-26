const ifj = @import("ifj24.zig");

pub fn multiply(a: i32, b: i32) i32 {
    return a * b;
}

pub fn main() void {
    const float : f64 = 7.2;
    const integer : i32 = 1;
    const x = float + integer; //error 8 - nelze odvodit typ
    const y = 3;
    const result = multiply(x, y);
    ifj.write("The product is: ");
    ifj.write(result);
}
