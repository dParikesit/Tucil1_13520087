#include <string.h>

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
namespace sc = std::chrono;

int main() {
  // Commparator tracker
  int cmpCount = 0;

  // Buka file
  fstream ifile;
  string namafile;
  cout << "Masukkan nama file termasuk ekstensi: ";
  cin >> namafile;
  ifile.open(namafile);

  // Masukkan puzzle dan kata ke vector
  vector<char> raw;
  vector<string> quest;

  string text;
  bool space = false;
  while (getline(ifile, text)) {
    // Output the text from the file
    if (space == false) {
      if (text.empty()) {
        space = true;
      } else {
        for (int i = 0; i < text.length(); i++) {
          if (text[i] != ' ') {
            raw.push_back(text[i]);
          }
        }

        raw.push_back('0');
      }
    } else {
      quest.push_back(text);
    }
  }
  ifile.close();

  // Mulai hitung waktu
  auto start = sc::high_resolution_clock::now();

  // Cari panjang per baris puzzle
  int lineLen = 0;
  while (raw[lineLen] != '0') {
    lineLen++;
  }
  // Jumlah baris puzzle
  int line = raw.size() / (lineLen + 1);

  // Masukkan vector puzzle menjadi array yang terstruktur
  char character[line][lineLen];
  int counterJ = 0;
  int counterI = 0;
  for (int i = 0; i < raw.size(); i++) {
    if (raw[i] != '0') {
      character[counterI][counterJ] = raw[i];
      counterJ++;
      if (counterJ > (lineLen - 1)) {
        counterJ = 0;
        counterI++;
      }
    }
  }

  // Jalankan algoritma bruteforce
  // Buat struct untuk menyimpan letak ditemukannya kata
  struct pos {
    int baris, kolom, arah;
  } question[quest.size()];
  /* Pada arah,
  1 dari poros ke kiri atas
  2 dari poros ke atas
  3 dari poros ke kanan atas
  4 dari poros ke kiri
  5 dari poros ke kanan
  6 dari poros ke kiri bawah
  7 dari poros ke bawah
  8 dari poros ke kanan bawah
   */

  // Menyimpan apakah indeks tertentu merupakan jawaban atau tidak
  int colored[line][lineLen];
  memset(colored, 0, sizeof(colored[0][0]) * line * lineLen);
  int currColor = 0;

  for (int x = 0; x < quest.size(); x++) {
    char first = quest[x][0];
    int questLen = quest[x].length();
    question[x].baris = -1;
    question[x].kolom = -1;
    question[x].arah = -1;

    // Looping semua puzzle cari character yang sama
    for (int i = 0; i < line; i++) {
      for (int j = 0; j < lineLen; j++) {
        if (question[x].arah == -1) {
          // Kalau ada character yang sama dicek lainnya
          cmpCount++;
          if (character[i][j] == first) {
            // Cek untuk tiap arah apakah ada yang benar
            bool bisaKiri = (j + 1 - questLen >= 0);
            bool bisaKanan = (j + questLen <= lineLen);
            bool bisaAtas = (i + 1 - questLen >= 0);
            bool bisaBawah = (i + questLen <= line);

            bool foundWord = false;
            bool foundChar = true;

            // Cek arah 1 Kiri Atas
            int progress = 0;
            if (bisaKiri && bisaAtas) {
              while (foundWord == false && foundChar == true && progress < (questLen - 1)) {
                progress++;
                cmpCount++;
                if (character[i - progress][j - progress] != quest[x][progress]) {
                  foundChar = false;
                }
                if (foundChar == true && progress == questLen - 1) {
                  foundWord = true;
                  question[x].baris = i;
                  question[x].kolom = j;
                  question[x].arah = 1;
                  for (int k = 0; k < questLen; k++) {
                    colored[i - k][j - k] = (currColor % 6) + 31;
                  }
                  currColor++;
                }
              }
            }

            // Cek arah 2 Atas
            progress = 0;
            if (bisaAtas) {
              foundChar = true;
              while (foundWord == false && foundChar == true && progress < (questLen - 1)) {
                progress++;
                cmpCount++;
                if (character[i - progress][j] != quest[x][progress]) {
                  foundChar = false;
                }
                if (foundChar == true && progress == questLen - 1) {
                  foundWord = true;
                  question[x].baris = i;
                  question[x].kolom = j;
                  question[x].arah = 2;
                  for (int k = 0; k < questLen; k++) {
                    colored[i - k][j] = (currColor % 6) + 31;
                  }
                  currColor++;
                }
              }
            }

            // Cek arah 3 Kanan Atas
            progress = 0;
            if (bisaKanan && bisaAtas) {
              foundChar = true;
              while (foundWord == false && foundChar == true && progress < (questLen - 1)) {
                progress++;
                cmpCount++;
                if (character[i - progress][j + progress] != quest[x][progress]) {
                  foundChar = false;
                }
                if (foundChar == true && progress == questLen - 1) {
                  foundWord = true;
                  question[x].baris = i;
                  question[x].kolom = j;
                  question[x].arah = 3;
                  for (int k = 0; k < questLen; k++) {
                    colored[i - k][j + k] = (currColor % 6) + 31;
                  }
                  currColor++;
                }
              }
            }

            // Cek arah 4 Kiri
            progress = 0;
            if (bisaKiri) {
              foundChar = true;
              while (foundWord == false && foundChar == true && progress < (questLen - 1)) {
                progress++;
                cmpCount++;
                if (character[i][j - progress] != quest[x][progress]) {
                  foundChar = false;
                }
                if (foundChar == true && progress == questLen - 1) {
                  foundWord = true;
                  question[x].baris = i;
                  question[x].kolom = j;
                  question[x].arah = 4;
                  for (int k = 0; k < questLen; k++) {
                    colored[i][j - k] = (currColor % 6) + 31;
                  }
                  currColor++;
                }
              }
            }

            // Cek arah 5 Kanan
            progress = 0;
            if (bisaKanan) {
              foundChar = true;
              while (foundWord == false && foundChar == true && progress < (questLen - 1)) {
                progress++;
                cmpCount++;
                if (character[i][j + progress] != quest[x][progress]) {
                  foundChar = false;
                }
                if (foundChar == true && progress == questLen - 1) {
                  foundWord = true;
                  question[x].baris = i;
                  question[x].kolom = j;
                  question[x].arah = 5;
                  for (int k = 0; k < questLen; k++) {
                    colored[i][j + k] = (currColor % 6) + 31;
                  }
                  currColor++;
                }
              }
            }

            // Cek arah 6 Kiri Bawah
            progress = 0;
            if (bisaKiri && bisaBawah) {
              foundChar = true;
              while (foundWord == false && foundChar == true && progress < (questLen - 1)) {
                progress++;
                cmpCount++;
                if (character[i + progress][j - progress] != quest[x][progress]) {
                  foundChar = false;
                }
                if (foundChar == true && progress == questLen - 1) {
                  foundWord = true;
                  question[x].baris = i;
                  question[x].kolom = j;
                  question[x].arah = 6;
                  for (int k = 0; k < questLen; k++) {
                    colored[i + k][j - k] = (currColor % 6) + 31;
                  }
                  currColor++;
                }
              }
            }

            // Cek arah 7 Bawah
            progress = 0;
            if (bisaBawah) {
              foundChar = true;
              while (foundWord == false && foundChar == true && progress < (questLen - 1)) {
                progress++;
                cmpCount++;
                if (character[i + progress][j] != quest[x][progress]) {
                  foundChar = false;
                }
                if (foundChar == true && progress == questLen - 1) {
                  foundWord = true;
                  question[x].baris = i;
                  question[x].kolom = j;
                  question[x].arah = 7;
                  for (int k = 0; k < questLen; k++) {
                    colored[i + k][j] = (currColor % 6) + 31;
                  }
                  currColor++;
                }
              }
            }

            // Cek arah 8 Kanan Bawah
            progress = 0;
            if (bisaKanan && bisaBawah) {
              foundChar = true;
              while (foundWord == false && foundChar == true && progress < (questLen - 1)) {
                progress++;
                cmpCount++;
                if (character[i + progress][j + progress] != quest[x][progress]) {
                  foundChar = false;
                }
                if (foundChar == true && progress == questLen - 1) {
                  foundWord = true;
                  question[x].baris = i;
                  question[x].kolom = j;
                  question[x].arah = 8;
                  for (int k = 0; k < questLen; k++) {
                    colored[i + k][j + k] = (currColor % 6) + 31;
                  }
                  currColor++;
                }
              }
            }
          }
        }
      }
    }
  }

  for (int i = 0; i < line; i++) {
    for (int j = 0; j < lineLen; j++) {
      if (colored[i][j] != 0) {
        cout << "\033[1m\033[" + to_string(colored[i][j]) + "m" << character[i][j] << "\033[0m";
        cout << " ";
      } else {
        cout << character[i][j] << " ";
      }
    }
    cout << endl;
  }

  auto end = sc::high_resolution_clock::now();
  sc::duration<double, std::milli> durasi = end - start;
  std::cout << "Waktu eksekusi program: " << durasi.count() << " ms" << endl;
  std::cout << "Total perbandingan huruf: " << cmpCount << endl;
  return 0;
}