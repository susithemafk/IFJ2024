const ifj = @import("ifj24.zig");

pub fn main() void {
    const str = "\xAa";
    ifj.write(str);
}