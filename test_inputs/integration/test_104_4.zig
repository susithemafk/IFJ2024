const ifj = @import("ifj24.zig");

pub fn func(a: []u8) void {
    ifj.write(a);
    ifj.write("hello there\n");
}

pub fn main() void {
    func("hello\n");
}
