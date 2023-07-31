#include <iostream>
#include <fstream>
#include <iomanip>


using namespace std;

bool* load_QTlist(int& give_size, string filename)
{
    ifstream kwantowy_ciag;
    kwantowy_ciag.open(filename, ios::binary); //robimy w binarnym wiec musi byc w ten sposob
    if (kwantowy_ciag.good())
    {
        //Get the size of the file in bytes
        kwantowy_ciag.seekg(0, std::ios::end);
        int file_size = kwantowy_ciag.tellg();
        give_size = file_size * 8;
        kwantowy_ciag.seekg(0, std::ios::beg);

        // Allocate memory for loaded data
        char* buffer = new char[file_size];
        //cout << file_size << endl;

        // Read data from file
        kwantowy_ciag.read(buffer, file_size);
        //bity w bool
        bool* bits = new bool[file_size * 8];
        for (int i = 0; i < file_size; i++) //przepisuje bity z kazdego bajta
        {
            for (int j = 0; j < 8; j++)
            {
                bits[i * 8 + j] = buffer[i] & (1 << j);
            }
        }
        delete[] buffer;
        kwantowy_ciag.close();
        return bits;
    }
    cout << "error loading a file" << endl;
    return NULL;
}

void write_QTlist(bool* list, int size, string filename)
{
    ofstream outFile(filename, ios::binary);
    if (outFile.good())
    {
        size_t numBytes = size / 8;
        for (size_t i = 0; i < numBytes; i++)
        {
            unsigned char byte = 0;
            for (size_t j = 0; j < 8; j++)
            {
                byte |= (list[i * 8 + j] ? 1 : 0) << (j);
            }
            outFile.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
        }
        outFile.close();
        cout << "Binary file written successfully." << std::endl;
    }
    else
    {
        cout << "Error opening the file for writing." << std::endl;
    }
}

int main()
{
    bool* A,*B,*C;
    int size;
    //size = 20;
    //C = new bool[size * 3];
    //A =new bool[20] { true,true,true,false,true,false,true,false,true,true,false,true,false,true,false,true,false,true,false,true };
    //B = new bool[20]{ true,true,false,true,false,true,false,false,true,false,true,false,true,false,true,false,true,false,true,false};

    A = load_QTlist(size, "QNGFile1.dat");
    B = load_QTlist(size, "QNGFile2.dat");
    C = new bool[size * 3];
    //write_QTlist(A, size, "test.dat");
    /*
    for (int i = 0; i < size; i++)
    {
        C[i] = A[i];
        //cout << C[i];
    }
    //cout << endl << endl;
    for (int i = size; i < size*2; i++)
    {
        C[i] = B[i-size];
        //cout << C[i];
    }
    //cout << endl << endl;
    for (int i = size * 2; i < size*3; i++)
    {
        if (A[i - size * 2] == B[i - size * 2])
        {
            C[i] = 0;
        }
        else C[i] = 1;
        //cout << C[i];
    }*/
    int smallblock=2000;
    cout<<size<<endl;
    for(int j=0;j<size/smallblock;j++)
    {
        int start=j*smallblock*3;
        for (int i = start; i < start+smallblock; i++)
        {
            C[i] = A[i-start+j*smallblock];
            //cout << C[i];
        }
       // cout << endl << endl;
        for (int i = start+smallblock; i < start+smallblock*2; i++)
        {
            C[i] = B[i-start+(j-1)*smallblock];
            //cout << C[i];
        }
        //cout << endl << endl;
        for (int i = start+smallblock * 2; i < start+smallblock*3; i++)
        {
            if (A[i-start+(j-2)*smallblock] == B[i-start+(j-2)*smallblock])
            {
                C[i] = 0;
            }
            else C[i] = 1;
            //cout << C[i];
        }
        //cout << endl << endl;
    }
    /*for(int i=0;i<size*3;i++)
    {
        cout<<C[i];
        if((i+1)%2==0) cout<<' ';
    }*/
    write_QTlist(C, size*3, "1XOR2.dat");
}

