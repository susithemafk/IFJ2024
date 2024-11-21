const ifj = @import("ifj24.zig");

pub fn factorial(n: i32) i32 {
    if (n <= 1) {
        return 1;
    } else {
        var d = n - 1;
        var c = factorial(d);
        return n * c;
    }
}

pub fn main() void {
    const input = ifj.readi32();
    if (input) |value| {
        const result = factorial(value);
        ifj.write("Factorial is: ");
        ifj.write(result);
    } else {
        ifj.write("Invalid input.\n");
    }
}
