const ifj = @import("ifj24.zig");

// testing divison rules
pub fn main() void {
    const b = 3; // int constant expression
    const a = 3.0; // float constant expression
    //const c = 3.1;

    const e = (a * b) / a;
    const f = 5.0 / 2; // should be fine
    const g = 5 / 2.0; // should be fine
    const h = 2 / a; // should be fine
    //const h = 2 / c; // should be error

    _ = e;
    _ = f;
    _ = g;
    _ = h;
}
