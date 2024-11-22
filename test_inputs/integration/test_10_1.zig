const ifj = @import("ifj24.zig");

pub fn sum_array(arr: []i32) i32 {
    var total: i32 = 0;
    for (arr) |value| {
        total += value;
    }
    return total;
}

pub fn main() void {
    const values: [5]i32 = [_]i32{1, 2, 3, 4, 5};
    const result = sum_array(values);
    ifj.write("Sum: ");
    ifj.write(result);
}
