#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
namespace sc = std::chrono;

int main() {
  // Commparator tracker
  int cmpCount = 0;

  // Buka file
  fstream ifile;
  string namafile;
  cout<<"Masukkan nama file termasuk ekstensi: ";
  cin>>namafile;
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
  int line = raw.size()/(lineLen+1);

  // Masukkan vector puzzle menjadi array yang terstruktur
  char character[line][lineLen];
  int counterJ = 0;
  int counterI=0;
  for(int i=0; i<raw.size(); i++){
    if(raw[i]!='0'){
      character[counterI][counterJ] = raw[i];
      counterJ++;
      if(counterJ>(lineLen-1)){
        counterJ=0;
        counterI++;
      }
    }
  }

  // Jalankan algoritma bruteforce
  // Buat struct untuk menyimpan letak ditemukannya kata
  struct pos{
    int baris,kolom,arah;
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
  
  for(int x=0; x<quest.size(); x++){
    char first=quest[x][0];
    int questLen = quest[x].length();
    question[x].baris=-1;
    question[x].kolom=-1;
    question[x].arah=-1;

    // Looping semua puzzle cari character yang sama
    for(int i=0; i<line; i++){
      for(int j=0; j<lineLen; j++){
        cmpCount++;
        if(question[x].arah==-1){
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
                  question[x].baris=i;
                  question[x].kolom=j;
                  question[x].arah=1;
                }
              }
            }

            // Cek arah 2 Atas
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
                }
              }
            }

            // Cek arah 3 Kanan Atas
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
                }
              }
            }

            // Cek arah 4 Kiri
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
                }
              }
            }
            
            // Cek arah 5 Kanan
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
                }
              }
            }
            
            // Cek arah 6 Kiri Bawah
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
                }
              }
            }
            
            // Cek arah 7 Bawah
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
                }
              }
            }
            
            // Cek arah 8 Kanan Bawah
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
                }
              }
            }
          }
        }
      }
    }
  }

  for(int x=0; x<quest.size(); x++){
    if(question[x].arah==-1){
      std::cout<<"Kata tidak ditemukan"<<endl<<endl;
    } else{
      int questLen = quest[x].length();
      char output[line][lineLen];
      for(int i=0; i<line; i++){
        for(int j=0; j<lineLen; j++){
          output[i][j] = '-';
        }
      }

      if(question[x].arah==1){
        // Kiri atas
        for(int k=0; k<questLen; k++){
          output[question[x].baris - k][question[x].kolom - k] = quest[x][k];
        }
      } else if(question[x].arah==2){
        // Atas
        for (int k = 0; k < questLen; k++) {
          output[question[x].baris - k][question[x].kolom] = quest[x][k];
        }
      } else if (question[x].arah == 3) {
        // Kanan atas
        for (int k = 0; k < questLen; k++) {
          output[question[x].baris - k][question[x].kolom + k] = quest[x][k];
        }
      } else if (question[x].arah == 4) {
        // Kiri
        for (int k = 0; k < questLen; k++) {
          output[question[x].baris][question[x].kolom - k] = quest[x][k];
        }
      } else if (question[x].arah == 5) {
        // Kanan
        for (int k = 0; k < questLen; k++) {
          output[question[x].baris][question[x].kolom + k] = quest[x][k];
        }
      } else if (question[x].arah == 6) {
        // Kiri bawah
        for (int k = 0; k < questLen; k++) {
          output[question[x].baris + k][question[x].kolom - k] = quest[x][k];
        }
      } else if (question[x].arah == 7) {
        // Bawah
        for (int k = 0; k < questLen; k++) {
          output[question[x].baris + k][question[x].kolom] = quest[x][k];
        }
      } else if (question[x].arah == 8) {
        // Kanan Bawah
        for (int k = 0; k < questLen; k++) {
          output[question[x].baris + k][question[x].kolom + k] = quest[x][k];
        }
      }

      for(int i=0; i<line; i++){
        for(int j=0; j<lineLen; j++){
          if(j==lineLen-1){
            std::cout<<output[i][j]<<endl;
          } else{
            std::cout<<output[i][j]<<" ";
          }
        }
      }

      std::cout<<endl;
    }
  }

  auto end = sc::high_resolution_clock::now();
  sc::duration<double, std::milli> durasi = end - start;
  std::cout << "Waktu eksekusi program: " << durasi.count() << " ms" << endl;
  std::cout << "Total perbandingan huruf: " << cmpCount << endl;
  return 0;
}