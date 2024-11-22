const ifj = @import("ifj24.zig");

pub fn foo() void {}

pub fn main() void {
    var x = foo(); // `foo` does not return a value
}
