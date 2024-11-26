const ifj = @import("ifj24.zig");

pub fn main() void {
    const str = \\xAG; //error - chybí ; (zadaný se bere jako string)
    ifj.write(str);
}