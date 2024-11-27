const ifj = @import("ifj24.zig");

pub fn main() void {
    var counter: i32 = 10;
    while (counter > 0) {
        ifj.write(counter);
        counter = counter -1;
    }
    ifj.write("Countdown complete!\n");
}
