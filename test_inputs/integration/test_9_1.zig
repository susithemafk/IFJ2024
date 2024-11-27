const ifj = @import("ifj24.zig");

pub fn main() void {
    const arr: [3]i32 = [_]i32{1, 2, 3};
    for (arr) |val| {
        ifj.write(val);
        ifj.write("\n");
    }
}
