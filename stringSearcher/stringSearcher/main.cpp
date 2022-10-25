#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <limits>
using namespace std;

void readFromFile(string file_path);
void findString(string line, int row, int lineLen, string inputStr);
void findAll();
bool checkForward(int i, int N);
bool checkBackward(int i, int N);


vector<int> ROW;
vector<int> RELATIVE_INDEX;
vector<int> ABSOLUTE_INDEX;

int main() {
    

    string file = "test.txt";
    readFromFile(file);
    findAll();
    cout << "\n" << "Finished Program" << endl;
    
    return 0;
}

/*
                         | row | rel | abs |
    BBxBBxxxxx\n         |  0  |  3  |  3  |
    xxxxxxxxBB\n         |  1  |  8  |  19 |
    BBxxxxxxxx\n         |  2  |  0  |  22 |
    xxxxxxxxBB\n         |  3  |  8  |  41 |

    program.exe "input txt file" BB 3

    vystup  1 8
            2 0
*/


void readFromFile(string file_path)
{
	ifstream file(file_path);
	int currentLine = 0;
    int counterLineLength = 0;
	string line;
	vector< vector<int>> v;


	if (file.is_open())
	{
		while (getline(file, line))
		{
            //cout << currentLine << ": " << line << " ,line length" << line.length() << endl;
            //cout << counterLineLength << endl;

            findString(line, currentLine, counterLineLength, "bbb");

            counterLineLength += (int)line.length() + 1;
            currentLine++;
		}
		file.close();
	}
	else
	{
		cout << "Can't open file" << '\n';
	}
}

void findString(string line, int row, int lineLen, string inputStr)
{
    string input_seq = line;
    string aa = inputStr;
    regex re("(?=(" + aa + ")).");
    sregex_iterator next(input_seq.begin(), input_seq.end(), re);
    sregex_iterator end;
    while (next != end) {
        smatch match = *next;
        
        ROW.push_back(row);
        RELATIVE_INDEX.push_back((int) match.position());
        ABSOLUTE_INDEX.push_back((int) match.position() + lineLen);

        //cout << match.str(1) << ": " << "\t" << row << "\t" << match.position() << "\t" << match.position() + lineLen << "\n";
        next++;
    }
}

void findAll() 
{
    int N = 20;
    for (int i = 0; (unsigned)i < ROW.size(); i++)
    {

        // CHECK LAST
        if ((unsigned)(i + 1) == ROW.size())
        {
            if (checkBackward(i, N))
            {
                cout << ROW[i] << "\t" << RELATIVE_INDEX[i] << "\n";
            }
        }

        // CHECK FIRST 
        else if (i == 0)
        {
            if (checkForward(i, N))
            {
                cout << ROW[i] << "\t" << RELATIVE_INDEX[i] << "\n" << ROW[i + 1] << "\t" << RELATIVE_INDEX[i + 1] << "\n";
                i++;
            }
        }

        else 
        {
            if (checkForward(i, N) || checkBackward(i, N))
            {
                if (checkForward(i, N))
                {
                    cout << ROW[i] << "\t" << RELATIVE_INDEX[i] << "\n" << ROW[i + 1] << "\t" << RELATIVE_INDEX[i + 1] << "\n";
                    i++;
                }
                else 
                {
                    cout << ROW[i] << "\t" << RELATIVE_INDEX[i] << "\n";
                }
            }
        }
    }
}

bool checkForward(int i, int N) 
{
   
    if (abs(ABSOLUTE_INDEX[i] - ABSOLUTE_INDEX[i + 1]) <= N)
    {
        return true;
    }
    return false;
}

bool checkBackward(int i, int N)
{
  
    if (i == 0)
    {
        return false;
    }
    if (abs(ABSOLUTE_INDEX[i] - ABSOLUTE_INDEX[i - 1]) <= N)
    {
        return true;
    }
    return false;
}

