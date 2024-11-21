const ifj = @import("ifj24.zig");

pub fn reverse_string(s: []u8) []u8 {
    var c = ifj.reverse(s);
    return c;
}

pub fn main() void {
    const str = ifj.string("Zig language");
    const reversed = reverse_string(str);
    ifj.write("Reversed: ");
    ifj.write(reversed);
}
