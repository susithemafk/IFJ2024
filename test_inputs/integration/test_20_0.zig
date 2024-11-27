const ifj = @import("ifj24.zig");

pub fn main() void {
    const message = ifj.string("Enter a number: ");
    ifj.write(message);
    const input = ifj.readi32();
    if (input) |num| {
        if (num == 0) {
            ifj.write("You entered zero.\n");
        } else {
            ifj.write("You entered a non-zero number.\n");
        }
    } else {
        ifj.write("Failed to read input.\n");
    }
}
