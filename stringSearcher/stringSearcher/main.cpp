#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <limits>
using namespace std;

void readFromFile(string file_path);
void findString(string line, int row, int lineLen, string inputStr);
//void checkArguments(int argc, char** argv);
void findAll(int N);
bool checkForward(int i, int N);
bool checkBackward(int i, int N);

//=================================================
void readFileFast(ifstream& file, void(*lineHandler)(char* str, int length, __int64 absPos), string regex);
void lineHandler(char* buf, int l, long long pos);
void loadFile(string regex);
int min(int buf, int file);
//=================================================


vector<int> ROW;
vector<int> RELATIVE_INDEX;
vector<int> ABSOLUTE_INDEX;


int main() {
    
    //checkArguments(argc, argv);
    string file = "test.txt";
    //readFromFile(file);
    loadFile("aa");
    findAll(3);
    cout << "\n" << "Finished Program" << endl;
    
    return 0;
}

/*
                         | row | rel | abs | len |
    BBxBBxxxxx\n         |  0  |  3  |  3  |  11 |
    xxxxxxxxBB\n         |  1  |  8  |  19 |  11 |
    BBxxxxxxxx\n         |  2  |  0  |  22 |  11 |
    xxxxxxxxBB\n         |  3  |  8  |  41 |  11 |

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
            if (file.good()) {
                line += "\n";
                cout << currentLine << ": " << line << ": <newline>: " << (int)line.length() << endl;
            }
            cout << currentLine << ": " << line << (int)line.length() << endl;
            findString(line, currentLine, counterLineLength, "BB");

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
    //regex re("[" + aa +"]");  when i want to find '\n'
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

void findAll(int N) 
{
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

//
//void checkArguments(int argc, char** argv)
//{
//
//}

void readFileFast(ifstream& file, void(*lineHandler)(char* str, int length, __int64 absPos), string regex) {
   
    long long lineCount = 0;
    int countLen = 0;
    int countLine = 0;
    int oneLineflag = 0;
    int BUF_SIZE = 5;
    string s;
    file.seekg(0, ios::end);
    ifstream::pos_type p = file.tellg();
#ifdef WIN32
    __int64 fileSize = *(__int64*)(((char*)&p) + 8);
#else
    __int64 fileSize = p;
#endif
    file.seekg(0, ios::beg);
    BUF_SIZE = min(BUF_SIZE, (int)fileSize);
    char* buf = new char[BUF_SIZE];
    int bufLength = BUF_SIZE;
    file.read(buf, bufLength);

    int strEnd = -1;
    int strStart;
    __int64 bufPosInFile = 0;
    while (bufLength > 0) {
        int i = strEnd + 1;
        strStart = strEnd;
        strEnd = -1;
        for (; i < bufLength && i + bufPosInFile < fileSize; i++) {
            if (buf[i] == '\n' ) {
                strEnd = i;
                oneLineflag = 1;
                lineCount++;
                break;
            }
        }

        if (strEnd == -1) { // scroll buffer
            if (strStart == -1) {
                
                s += string(buf + strStart + 1, bufLength);
                lineHandler(buf + strStart + 1, bufLength, lineCount);
                bufPosInFile += bufLength;
                bufLength = min(bufLength, (int)( fileSize - bufPosInFile));
                delete[]buf;
                buf = new char[bufLength];
                file.read(buf, bufLength);
            }
            else {
                int movedLength = bufLength - strStart - 1;
                memmove(buf, buf + strStart + 1, movedLength);
                bufPosInFile += strStart + 1;
                int readSize = min(bufLength - movedLength, (int)(fileSize - bufPosInFile - movedLength));

                if (readSize != 0)
                    file.read(buf + movedLength, readSize);
                if (movedLength + readSize < bufLength) {
                    char* tmpbuf = new char[movedLength + readSize];
                    memmove(tmpbuf, buf, movedLength + readSize);
                    delete[]buf;
                    buf = tmpbuf;
                    bufLength = movedLength + readSize;
                }
                strEnd = -1;
            }
        }
        else {
            lineHandler(buf + strStart + 1, strEnd - strStart, lineCount);
            s += string(buf + strStart + 1, strEnd - strStart);
            findString(s, countLine, countLen, regex);
            countLen += (int)s.length();
            countLine++;
            s = "";
            

        }
    }
    //lineHandler(0, 0, 0);//eof
    if (oneLineflag == 0 || bufLength == 0)
    {
        findString(s, countLine, countLen, regex);
    }
}

void lineHandler(char* buf, int l, long long  pos) {
    pos = 0;
    if (buf == 0) return;
    string s = string(buf, l);
    //cout << s;
    /*printf(s.c_str());*/
}

void loadFile(string regex) {
    ifstream infile("test.txt");
    readFileFast(infile, lineHandler, regex);
}

int min(int buf, int file)
{
    if (buf < file)
    {
        return buf;
    }
    return file;
}