const ifj = @import("ifj24.zig");

pub fn main() void {
    const str = ifj.string("\xAa");
    ifj.write(str);
}
