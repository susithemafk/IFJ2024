const ifj = @import("ifj24.zig");

pub fn main() void { //error 1 - chybí ukončující uvozovka
    const msg = "Hello;
    ifj.write(msg);
}
