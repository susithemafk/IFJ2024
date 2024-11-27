const ifj = @import("ifj24.zig");

pub fn main() void {
    const str = 
        \\text1
        \\text2
    ;
    ifj.write(str);
}
