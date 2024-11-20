const ifj = @import("ifj24.zig");

pub fn main() void {
    const a = ifj.readi32();
    const b = ifj.readi32();
    if (a and b) |x, y| { // not supported in ifj zig thing
        if (x > y) {
            ifj.write(x);
        } else {
            ifj.write(y);
        }
    } else {
        ifj.write("Invalid inputs.\n");
    }
}
