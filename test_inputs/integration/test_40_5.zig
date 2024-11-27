const ifj = @import("ifj24.zig");

pub fn foo() f64 {
    return 2.0;
}
pub fn foo() i32 {
    return 2;
} // Redeclaration of function `foo` with a different return type

pub fn main() void {
    var a = foo();
}
