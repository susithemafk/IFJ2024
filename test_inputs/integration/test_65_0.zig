const ifj = @import("ifj24.zig");

pub fn main() void {
    // Načtení 5 hodnot
    ifj.write("Zadejte 5 celých čísel pro výpočet statistik:\n");

    const val1 = ifj.readi32();
    const val2 = ifj.readi32();
    const val3 = ifj.readi32();
    const val4 = ifj.readi32();
    const val5 = ifj.readi32();

    // Kontrola správnosti načtení
    if (val1 == null) {
        ifj.write("Chyba při načítání první hodnoty.\n");
        return;
    }
    if (val2 == null) {
        ifj.write("Chyba při načítání druhé hodnoty.\n");
        return;
    }
    if (val3 == null) {
        ifj.write("Chyba při načítání třetí hodnoty.\n");
        return;
    }
    if (val4 == null) {
        ifj.write("Chyba při načítání čtvrté hodnoty.\n");
        return;
    }
    if (val5 == null) {
        ifj.write("Chyba při načítání páté hodnoty.\n");
        return;
    }

    // Proměnné pro výpočty
    var sum: i32 = 0;
    var min: i32 = val1;
    var max: i32 = val1;

    // Výpočty
    sum = sum + val1;
    if (val1 < min) {min = val1;}
    if (val1 > max) {max = val1;}

    sum = sum + val2;
    if (val2 < min) {min = val2;}
    if (val2 > max) {max = val2;}

    sum = sum + val3;
    if (val3 < min) {min = val3;}
    if (val3 > max) {max = val3;}

    sum = sum + val4;
    if (val4 < min) {min = val4;}
    if (val4 > max) {max = val4;}

    sum = sum + val5;
    if (val5 < min) {min = val5;}
    if (val5 > max) {max = val5;}

    // Výpočet průměru
    const temp : f64 = ifj.i2f(sum);
    const avg: f64 = temp / 5.0;

    // Výstup výsledků
    ifj.write("Průměr: "); ifj.write(avg); ifj.write("\n");
    ifj.write("Minimum: "); ifj.write(min); ifj.write("\n");
    ifj.write("Maximum: "); ifj.write(max); ifj.write("\n");
}
