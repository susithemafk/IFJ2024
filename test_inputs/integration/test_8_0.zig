const ifj = @import("ifj24.zig");

pub fn concat_and_write(a: []u8, b: []u8) void {
    const res = ifj.concat(a, b);
    ifj.write(res);
}

pub fn main() void {
    const str1 = ifj.string("Hello, ");
    const str2 = ifj.string("World!");
    concat_and_write(str1, str2);
}
