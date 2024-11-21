const ifj = @import("ifj24.zig");

pub fn main() void {
    const prompt = ifj.string("Enter your name: ");
    ifj.write(prompt);
    const name = ifj.readstr();
    if (name) |validName| {
        var c = ifj.string("Hello, ");
        const message = ifj.concat(c, validName);
        ifj.write(message);
        ifj.write("\n");
    } else {
        ifj.write("Error reading name.\n");
    }
}
