/*  ____          _____          _____          
 * |  _ \   /\   / ____|   /\   / ____|   /\    
 * | |_) | /  \ | |  __   /  \ | |  __   /  \   
 * |  _ < / /\ \| | |_ | / /\ \| | |_ | / /\ \  
 * | |_) / ____ \ |__| |/ ____ \ |__| |/ ____ \ 
 * |____/_/    \_\_____/_/    \_\_____/_/    \_\
 *
 * Bagaga - Bloody Amazing Game Architecture Game
 * Copyright 2020 Bagaga Development Team. All rights reserved.                                             
 */

// C++ Standard Library includes
#include <iostream>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// Constants.
const string kAppName           = "Bin2Hdr";
const string kAppVersion        =  "1.0.0";
const string kCopyrightHolder   = "Roman Divotkey";
const string kCopyrightYear     = "2021";
const int kGroupSize            = 12;


void PrintHeader()
{
    cout << kAppName << " - " << kAppVersion << endl;
    cout << "Copyright " << kCopyrightHolder << " " << kCopyrightYear;
    cout << ". All rights reserved." << endl << endl;
}

void PrintUsage(const char* prgName)
{
    cout << "Usage: " << prgName << " input_file output_file var_name" << endl;
}


void ConvertBinary(const string & input, const string & output, const string & varName)
{
    cout << "Converting input file '" << input << "' to C++ header file '" << output << "'" << endl;

    // Open input file.
    std::ifstream ifs(input, ios::in | ios::binary);
    if (!ifs) {
        throw std::runtime_error(std::string("Unable to open binary file '") 
            + input + "' for reading");
    }

    // Open output file.
    std::ofstream ofs(output, ios::out);
    if (!ofs) {
        throw std::runtime_error(std::string("Unable to open output file '") 
            + output + "' for writing");
    }

    // Write C++ header.
    ofs << "#pragma once" << endl << endl;
    ofs << "#intluce <vector>" << endl << endl;
    ofs << "std::vector<unsigned char> " << varName << " = {" << endl << "\t";
    ofs << setfill('0');

    int cnt = 0;
    for (auto it = istreambuf_iterator<char>(ifs); it != istreambuf_iterator<char>();) {
        ofs << "0x" << hex << setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(*it));
        if (++it != istreambuf_iterator<char>()) {
            ofs << ",";
        }
        if (++cnt >= kGroupSize) {
            cnt = 0;
            ofs << endl << "\t";
        } else {
            ofs << " ";
        }
    }

    ofs << endl << "};" << endl;

    ifs.close();
    ofs.close();
}

int main(int argc, char *argv[])
{
    PrintHeader();

    if (argc != 4) {
        PrintUsage(kAppName.c_str());
        return -1;
    }

    try {
        ConvertBinary(argv[1], argv[2], argv[3]);
        // ConvertBinary("foo.bin", "foo.h", "kVertexShader");
    } catch (const runtime_error & e) {
        cerr << e.what() << endl;
    }

    return 0;
}