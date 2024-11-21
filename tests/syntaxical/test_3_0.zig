const ifj = @import("ifj24.zig");

pub fn calculate_area(length: f64, width: f64) f64 {
    return length * width;
}

pub fn main() void {
    const l = 5.5;
    const w = 3.2;
    const area = calculate_area(l, w);
    ifj.write("Area is: ");
    ifj.write(area);
}
