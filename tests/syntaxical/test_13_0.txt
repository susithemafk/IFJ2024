const ifj = @import("ifj24.zig");

pub fn main() void {
    const num = ifj.readi32();
    if (num) |value| {
        const doubled = value * 2;
        ifj.write("Doubled value: ");
        ifj.write(doubled);
    } else {
        ifj.write("Input was invalid.\n");
    }
}
