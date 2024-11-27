const ifj = @import("ifj24.zig");

pub fn bar(param: i32) void {
    ifj.write(param); // to not get an anused param
}

pub fn main() void {
    bar("wrong type"); // Passing a string instead of an integer
}
