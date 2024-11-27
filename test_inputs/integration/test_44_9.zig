const ifj = @import("ifj24.zig");

pub fn main() void {
    var unusedVariable: i32 = 100;
    ifj.write("Hello World");
    // `unusedVariable` is never used in any operation
}
