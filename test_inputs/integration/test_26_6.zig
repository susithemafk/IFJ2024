const ifj = @import("ifj24.zig");

pub fn foo() i32 {} // Function `foo` does not return a value

pub fn main() void {
    const res = foo(); 
}
