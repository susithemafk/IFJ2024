const ifj = @import("ifj24.zig");

pub fn main() void {
    const a = 3.0; // float constant expression
    const b = 3; // int constant expression
    const c = 2;

    const d = a * (3.0 + b); // float constant expression
    // tree should look like this:
    //          *
    //        /   \
    //       +     a    + will be type: int, a is float, => converting a to int
    //      / \
    //     3.0 b

    _ = d; // to not get unused warning

    const e = 3.0 + b; // float constant expression
    _ = e;

    const f = a + b; // float constant expression
    _ = f;

    const g = a * b; // float constant expression
    _ = g;

    const h = a + 2; // float constant expression
    _ = h;

    const j = c * (3.0 + 2); // should fail here
    // tree should look like this:
    //          *
    //        /   \
    //       +     c    here, + will be type: f64, b is int => error
    //      / \
    //     3.0 b

    _ = j; // to not get unused warning

}
