const ifj = @import("ifj24.zig");

pub fn add_five(n: i32) i32 {
    return n + 5;
}

pub fn main() void {
    const value = ifj.readi32();
    if (value) |num| {
        const new_value = add_five(num);
        ifj.write("New value: ");
        ifj.write(new_value);
    } else {
        ifj.write("Invalid input.\n");
    }
}
