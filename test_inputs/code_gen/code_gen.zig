const ifj = @import("ifj24.zig");

// Hlavni funkce
pub fn main() void {
    // Testing built-in functions
    ifj.write("Test vestavenych funkci:\n");

    // Test ifj.i2f and ifj.f2i
    const int_value: i32 = 42;
    const float_value: f64 = ifj.i2f(int_value);
    const int_result: i32 = ifj.f2i(float_value);
    ifj.write("i2f(42) -> ");
    ifj.write(float_value); // Should print 42.0
    ifj.write("\n");
    ifj.write("f2i(42.0) -> ");
    ifj.write(int_result); // Should print 42
    ifj.write("\n");

    // Test string manipulation
    const str1 = ifj.string("Hello, ");
    const str2 = ifj.string("world!");
    const concatenated = ifj.concat(str1, str2);
    ifj.write("concat(\"Hello, \", \"world!\") -> ");
    ifj.write(concatenated); // Should print "Hello, world!"
    ifj.write("\n");

    const length = ifj.length(concatenated);
    ifj.write("length(\"Hello, world!\") -> ");
    ifj.write(length); // Should print 13
    ifj.write("\n");

    const substring_result = ifj.substring(concatenated, 7, 12);
    ifj.write("substring(\"Hello, world!\", 7, 12) -> ");
    if (substring_result) |sub| {
        ifj.write(sub); // Should print "world"
    } else {
        ifj.write("null");
    }
    ifj.write("\n");

    const substring_result1 = ifj.substring(concatenated, 7, 7);
    ifj.write("substring(\"Hello, world!\", 7, 7) -> ");
    if (substring_result1) |sub| {
        ifj.write(sub); // Should print ""
    } else {
        ifj.write("null");
    }
    ifj.write("\n");

    const substring_result2 = ifj.substring(concatenated, 7, 6);
    ifj.write("substring(\"Hello, world!\", 7, 6) -> ");
    if (substring_result2) |sub| {
        ifj.write(sub); // Should print "null"
    } else {
        ifj.write("null");
    }
    ifj.write("\n");

    const neg_number: i32 = 0 - 1;
    const substring_result3 = ifj.substring(concatenated, neg_number, 6);
    ifj.write("substring(\"Hello, world!\", -1, 6) -> ");
    if (substring_result3) |sub| {
        ifj.write(sub); // Should print "null"
    } else {
        ifj.write("null");
    }
    ifj.write("\n");

    const substring_result4 = ifj.substring(concatenated, 7, neg_number);
    ifj.write("substring(\"Hello, world!\", 7, -1) -> ");
    if (substring_result4) |sub| {
        ifj.write(sub); // Should print "null"
    } else {
        ifj.write("null");
    }
    ifj.write("\n");

    const substring_result5 = ifj.substring(concatenated, 7, 13);
    ifj.write("substring(\"Hello, world!\", 7, 13) -> ");
    if (substring_result5) |sub| {
        ifj.write(sub); // Should print "world!"
    } else {
        ifj.write("null");
    }
    ifj.write("\n");

    const substring_result6 = ifj.substring(concatenated, 7, 14);
    ifj.write("substring(\"Hello, world!\", 7, 14) -> ");
    if (substring_result6) |sub| {
        ifj.write(sub);
    } else {
        ifj.write("null"); // Should print "null"
    }
    ifj.write("\n");

    const substring_result7 = ifj.substring(concatenated, 13, 7);
    ifj.write("substring(\"Hello, world!\", 13, 7) -> ");
    if (substring_result7) |sub| {
        ifj.write(sub);
    } else {
        ifj.write("null"); // Should print "null"
    }
    ifj.write("\n");

    // Test strcmp
    const str_cmp1 = ifj.string("apple");
    const str_cmp2 = ifj.string("banana");
    const cmp_result = ifj.strcmp(str_cmp1, str_cmp2);
    ifj.write("strcmp(\"apple\", \"banana\") -> ");
    ifj.write(cmp_result); // Should print -1
    ifj.write("\n");

    // Test ord and chr
    const char_value = ifj.string("Z");
    const ascii_value = ifj.ord(char_value, 0);
    ifj.write("ord(\"Z\", 0) -> ");
    ifj.write(ascii_value); // Should print 90
    ifj.write("\n");

    const char_result = ifj.chr(65);
    ifj.write("chr(65) -> ");
    ifj.write(char_result); // Should print "A"
    ifj.write("\n");

    // End of built-in functions test
    ifj.write("Vestavene funkce otestovany.\n\n");

    // Factorial calculations
    ifj.write("Vypocet faktorialu pro cisla od 0 do 7:\n");

    // Explicit calls to factorial function
    const result0 = factorial(0);
    ifj.write("Faktorial 0: ");
    ifj.write(result0);
    ifj.write("\n");

    const result1 = factorial(1);
    ifj.write("Faktorial 1: ");
    ifj.write(result1);
    ifj.write("\n");

    const result2 = factorial(2);
    ifj.write("Faktorial 2: ");
    ifj.write(result2);
    ifj.write("\n");

    const result3 = factorial(3);
    ifj.write("Faktorial 3: ");
    ifj.write(result3);
    ifj.write("\n");

    const result4 = factorial(4);
    ifj.write("Faktorial 4: ");
    ifj.write(result4);
    ifj.write("\n");

    const result5 = factorial(5);
    ifj.write("Faktorial 5: ");
    ifj.write(result5);
    ifj.write("\n");

    const result6 = factorial(6);
    ifj.write("Faktorial 6: ");
    ifj.write(result6);
    ifj.write("\n");

    const result7 = factorial(7);
    ifj.write("Faktorial 7: ");
    ifj.write(result7);
    ifj.write("\n");

    // Add a while loop
    ifj.write("Spoustim while cyklus s pocatecnim cislem 10:\n");
    var a: i32 = 10;
    while (a >= 0) {
        ifj.write("Hodnota a: ");
        ifj.write(a);
        ifj.write("\n");
        a = a - 1; // Decrement a
    }
    ifj.write("While cyklus dokonceny.\n");

    // Nested while loop with if statements
    ifj.write("Spoustim while cyklus s vnorenymi if prikazy:\n");
    var b: i32 = 8; // Start from 8 and decrement
    while (b > 0) {
        if (b >= 5) {
            if (b >= 7) {
                if (b == 8) {
                    ifj.write("Hodnota b je 8.\n");
                } else {
                    ifj.write("Hodnota b je 7.\n");
                }
            } else {
                if (b == 6) {
                    ifj.write("Hodnota b je 6.\n");
                } else {
                    ifj.write("Hodnota b je 5.\n");
                }
            }
        } else {
            if (b >= 3) {
                if (b == 4) {
                    ifj.write("Hodnota b je 4.\n");
                } else {
                    ifj.write("Hodnota b je 3.\n");
                }
            } else {
                if (b == 2) {
                    ifj.write("Hodnota b je 2.\n");
                } else {
                    ifj.write("Hodnota b je 1.\n");
                }
            }
        }
        b = b - 1; // Decrement a
    }
    ifj.write("Vnorene if prikazy ukonceny.\n");
}

// Pomocna funkce pro dekrementaci celeho cisla o zadane cislo
pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}

// Definice funkce pro vypocet hodnoty faktorialu
pub fn factorial(n: i32) i32 {
    var result: i32 = 0 - 1; // Initializes to an invalid result by default
    if (n < 2) {
        result = 1;
    } else {
        const decremented_n = decrement(n, 1);
        const temp_result = factorial(decremented_n);
        result = n * temp_result;
    }
    return result;
}
