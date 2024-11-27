const ifj = @import("ifj24.zig");

pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}

pub fn factorial(n: i32) i32 {
    var result: i32 = 0 - 1;
    if (n < 2) {
        result = 1;
    } else {
        const decremented_n = decrement(n, 1);
        const temp_result = factorial(decremented_n);
        result = n * temp_result;
    }
    return result;
}

pub fn main () void {
    _ = factorial(3);
}
