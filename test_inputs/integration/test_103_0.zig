const ifj = @import("ifj24.zig");

pub fn func(a: []u8) void {
    ifj.write(a);
    ifj.write("hello there\n");
}

pub fn main() void {
    const string = ifj.string("hello\n");
    func(string);
}
