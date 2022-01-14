#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::fstream;
using std::ofstream;
using std::ifstream;
using std::cin;
using std::cout;
using std::vector;
using std::string;

int GetFileSize(std::string file_name)
{
  ifstream ifs(file_name, std::ios::in | std::ios::binary); 
  if(!ifs)
    std::cerr<<"Cannot open the input file."<<std::endl;
  
  ifs.seekg(0,std::ios::end);
  int file_size = (int)ifs.tellg();
  ifs.close();
  return file_size;
}

bool ReadFile(vector<uint64_t>* primes, string file_name)
{
  ifstream ifs(file_name, std::ios::in | std::ios::binary); 
  if(!ifs)
  {
    std::cerr<<"Cannot open the input file."<<std::endl;
    return false;
  }

  ifs.seekg(0,std::ios::end);
  int file_size = (int)ifs.tellg();

  if ((file_size % sizeof(uint64_t)) != 0) {
    std::cerr<<"File has unexpected byte size: " << file_size <<std::endl;
    ifs.close();
    return false;
  }

  if (file_size != 0) {
    int primes_size = file_size / sizeof(uint64_t);
    primes->resize(primes_size);
    
    ifs.seekg(0);
    for (int i = 0; i < primes_size; i++) {
      ifs.read((char*) &primes->at(i), sizeof(uint64_t));
    }
  }

  ifs.close();

  return true;
}

bool ClearFile(std::string file_name)
{
  ofstream ofs(file_name, std::ios::out | std::ios::binary | std::ios::trunc); 
  if(!ofs)
  {
    std::cerr<<"Cannot open the output file."<<std::endl;
    return false;
  }

  ofs.close();
  return true;
}

bool AddPrimesToFile(string file_name, int iterations)
{
  vector<uint64_t> primes;
  if (!ReadFile(&primes,file_name))
    return false;

  uint64_t current_num;
  if (primes.size() == 0) {
    current_num = 2;
  }
  else {
    current_num = primes.back() + 1;
  }
  bool is_prime = true;

  ofstream ofs(file_name, std::ios::out | std::ios::binary | std::ios::app); 
  if(!ofs)
  {
    std::cerr<<"Cannot open the output file."<<std::endl;
    return false;
  }

  // while (current_num != 0)
  for (int i = 0; i < iterations; i++) {

    for(int j = 0; j < primes.size(); j++) {
      if (current_num % primes.at(j) == 0) {
        is_prime = false;
        break;
      }
    }

    if(is_prime) {
      primes.push_back(current_num);
      ofs.write((char*) &current_num,sizeof(uint64_t));
      cout << current_num << '\n';
    }
    else {
      is_prime = true;
    }

    current_num++;
  }

  ofs.close();
  return true;
}

int main()
{
  AddPrimesToFile("data.bin",10);
  return 0;
}
