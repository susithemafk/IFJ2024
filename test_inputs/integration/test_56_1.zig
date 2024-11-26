const ifj = @import("ifj24.zig");

pub fn main() void {
    const str = \\xAG; //error 1 nevalidní sekvence
    ifj.write(str);
}