const ifj = @import("ifj24.zig");

pub fn main() void {
    var a: []u8 = ifj.string("MOZEM TU BYT\n");
    a = "NEMAM TU BYT\n";
}
