#include <iostream>
#include <fstream>

int main() {
    std::ofstream fout("medium.txt");
    fout << 9 << std::endl;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            fout << i << " " << j << std::endl;
        }
    }
    fout << std::endl;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            if (i >= 5 && i <= 9 && j >= 5 && j <= 9) {
                continue;
            }
            if (i <= 2 or j <= 2) {
                continue;
            }
            if (i >= 8 or j >= 8) {
                fout << i << " " << j << std::endl;
            }
        }
    }
}