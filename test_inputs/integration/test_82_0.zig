const ifj = @import("ifj24.zig");
// Hlavná funkcia programu
pub fn main() void {
    ifj.write("Vitaj v komplexnom testovacom programe pre jazyk IFJ24!\n");

    // Práca s číslami a základnými operáciami
    const a: i32 = 42;
    const b: i32 = 13;
    var result: i32 = a + b;
    ifj.write("Sucet cisel: ");
    ifj.write(result);
    ifj.write("\n");

    // Práca s podmienkami
    if (result > 50) {
        result = 10;
        ifj.write("Vysledok je vacsi ako 50.\n");
    } else {
        ifj.write("Vysledok je mensi alebo rovny 50.\n");
    }

    // Práca s cyklom while
    ifj.write("Vypis cisel od 1 do 5:\n");
    var i: i32 = 1;
    while (i <= 5) {
        ifj.write(i);
        ifj.write("\n");
        i = i + 1;
    }

    // Práca s funkciami (faktoriál - rekurzívne)
    const factorialInput: i32 = 5;
    const factorialResult = factorial(factorialInput);
    ifj.write("Faktorial cisla ");
    ifj.write(factorialInput);
    ifj.write(" je ");
    ifj.write(factorialResult);
    ifj.write(".\n");

    // Práca s reťazcami
    const str1 = ifj.string("Testovanie prace s retazcami.");
    const str2 = ifj.string(" IFJ24 je super!");
    const combined = ifj.concat(str1, str2);
    ifj.write("Spojeny retazec: ");
    ifj.write(combined);
    ifj.write("\n");

    // Cyklus while na čítanie vstupu
    var sum: i32 = 0;
    ifj.write("Zadajte cisla na scitanie (0 ukonci vstup):\n");

    var brk: i32 = 1;
    while (brk != 0) {
        const input = ifj.readi32();
        if (input) |num| {
            if (num == 0) {
                brk = 0;
            } else {
                sum = sum + num;
            }
        } else {
            ifj.write("Neplatny vstup! Skuste znova.\n");
        }
    }
    ifj.write("Sucet zadanych cisel je: ");
    ifj.write(sum);
    ifj.write("\n");

    // Práca s porovnávaním reťazcov
    ifj.write("Zadajte 'ahoj', aby ste pokracovali:\n");
    const inputStr = ifj.readstr();
    const expected = ifj.string("ahoj");
    if (inputStr) |in_str| {
        const same = ifj.strcmp(in_str, expected);
        if (same == 0) {
            ifj.write("Zadany retazec je spravny!\n");
        } else {
            ifj.write("Nespravny retazec. Koncim program. 1\n");
            return;
        }
    } else {
        ifj.write("Nespravny retazec. Koncim program. 2\n");
        return;
    }

    ifj.write("Testovaci program ukonceny uspesne.\n");
}

// Rekurzívna funkcia na výpočet faktoriálu
pub fn factorial(n: i32) i32 {
    if (n <= 1) {
        return 1;
    } else {}
    const n_ = n - 1;
    const a = factorial(n_);
    return n * a;
}
