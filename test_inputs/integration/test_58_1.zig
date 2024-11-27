const ifj = @import("ifj24.zig");

pub fn main() void {
    const greeting = ifj.string('Hello, IFJ24!'); //error 1? - řetězec ohraničen jednoduchými uvozovkami
    ifj.write(greeting);
    ifj.write("\nProgram executed successfully.\n");
}
