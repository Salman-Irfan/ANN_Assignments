#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

// Global variables
const int maxRows = 4; 
const int maxCols = 3; 
bool isFirstTry = true;
bool isFileExist = false;
string globalFileName;
int globalInput1Array[maxRows];
int globalInput2Array[maxRows];

float weight1 = 0.5;
float weight2 = 0.5;
float bias = 0.5;

int yDesired[maxRows]; // yDesired[i] = outputArray[i]

// Declarations of Functions
void showMenu();
int chooseOption();
string chooseFile();
bool readCsvFile(int data[maxRows][maxCols], string fileName);
void printArray(int data[maxRows][maxCols]);

int calculateActivation(int input1, int input2, int weight1, int weight2, int bias);

void trainPerceptron(int input1[], int input2[], int output[], int n, float &weight1, float &weight2, float &bias);

void testPerceptronArray(int input1[], int input2[], int output[], int n, int weight1, int weight2, int bias);

void testPerceptronUser(int input1, int input2, int output[], int n, int weight1, int weight2, int bias);

// ##### Main #### //
int main()
{
    showMenu();
    int option = chooseOption();

    do
    {
        switch (option)
        {

        case 1: // Training
        {
            string fileName = chooseFile();
            globalFileName = fileName;

            // Storing the Data from file
            int data[maxRows][maxCols];
            readCsvFile(data, fileName); // calling function for reading file
            printArray(data);

            // if file exists
            if (isFileExist)
            {
                int outputArray[maxRows];
                cout << "outputArray \n";
                for (int i = 0; i < maxRows; i++)
                {
                    outputArray[i] = data[i][2];
                    yDesired[i] = outputArray[i];
                    cout << outputArray[i] << "\n";
                }

                int input1Array[maxRows];
                cout << "input1Array \n";
                for (int i = 0; i < maxRows; i++)
                {
                    input1Array[i] = data[i][0];
                    globalInput1Array[i] = input1Array[i];
                    cout << input1Array[i] << "\n";
                }

                int input2Array[maxRows];
                cout << "input2Array \n";
                for (int i = 0; i < maxRows; i++)
                {
                    input2Array[i] = data[i][1];
                    globalInput2Array[i] = input2Array[i];
                    cout << input2Array[i] << "\n";
                }
                trainPerceptron(globalInput1Array, globalInput2Array, yDesired, maxRows, weight1, weight2, bias);

                isFirstTry = false;
                break;
            }
        }

        case 2:
        { // Testing
            if (!isFirstTry)
            {
                testPerceptronArray(globalInput1Array, globalInput2Array, yDesired, 4, weight1, weight2, bias);
                int input1;
                int input2;
                cout << "input1 = ";
                cin >> input1;
                cout << "input2 = ";
                cin >> input2;
                testPerceptronUser(input1, input2, yDesired, 4, weight1, weight2, bias);
            }
            else // if user enter 2 in first try
            {
                cout << "\033[33m"
                     << " First Train your model\n Select 1 to Train or 3 to exit\n "
                     << "\033[0m" << endl;
            }
            break;
        }

        default: // Exit
            cout << "enter valid options\n";
        }

        showMenu();
        option = chooseOption();

    } while (option != 3);
}

// Definitions of Functions
void showMenu()
{
    cout << "********************************\n";
    cout << "1.   Train\n";
    cout << "2.   Test\n";
    cout << "3.   Exit\n";
}

int chooseOption()
{
    int option;
    cout << "Enter Option = ";
    cin >> option;
    return option;
}

string chooseFile()
{
    string fileName;
    cout << "File to Train = ";
    cin >> fileName;
    return fileName;
}

bool readCsvFile(int data[maxRows][maxCols], string fileName)
{

    ifstream file(fileName);
    if (file.is_open())
    {
        cout << "\033[32m" << fileName << " is opened successfully"
             << "\033[0m" << endl;
        string line; // storing each line of the file
        int row = 0; // to keep track the current row, by default representing first row

        // getline (input stream, line by line, delimiter)
        while (getline(file, line) && row < maxRows)
        {
            stringstream ss(line); // Use stringstream to split each line into fields
            string field; // store each field / csv of the line
            int col = 0;  // to keep track the current col, by default representing first col

            while (getline(ss, field, ',') && col < maxCols)
            { // read fields upto maximum number of columns
                data[row][col] = stoi(field); // storing the data in the respective column
                col++;
            }
            row++;
        }
        file.close();
        isFileExist = true;
    }
    else
    { // if the file was not opened successfully
        cout << "\033[31m"
             << "Failed to open file: " << fileName << "\033[0m" << endl;
    }
    return isFileExist;
}

void printArray(int data[maxRows][maxCols])
{
    for (int i = 0; i < maxRows; i++)
    { // for each row
        for (int j = 0; j < maxCols; j++)
        {   // for each column
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
}

int calculateActivation(int input1, int input2, int weight1, int weight2, int bias)
{
    int activation = (input1 * weight1) + (input2 * weight2) + bias;
    if (activation > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void trainPerceptron(int input1[], int input2[], int output[], int n, float &weight1, float &weight2, float &bias)
{
    int errors = 1;
    int epoch = 0;

    cout << "\033[32m"
         << "printing learning logs\n"
         << "\033[0m";
    cout << "--------------------------------\n";
    cout << "\033[32m"
         << "Ep \t I1 \t I2 \t Yd \t w1 \t w2 \t Ya \t Err \t Fw1 \t Fw2 \n"
         << "\033[0m";

    while (errors != 0)
    {
        errors = 0;
        for (int i = 0; i < n; i++)
        {
            int prediction = calculateActivation(input1[i], input2[i], weight1, weight2, bias);
            int error = yDesired[i] - prediction;
            if (error != 0)
            {
                errors++;
                weight1 += error * input1[i];
                weight2 += error * input2[i];
                bias += error;
            }
            cout << "\033[32m" << epoch << "\t " << globalInput1Array[i] << "\t " << globalInput2Array[i] << "\t " << yDesired[i] << "\t " << weight1 << "\t " << weight2 << "\t " << prediction << "\t " << error << "\t " << weight1 << "\t " << weight2 << "\n"
                 << "\033[0m";
        }
        epoch++;
    }

    cout << "Training completed" << endl;
    cout << "\033[33m"
         << "Number of epochs: " << epoch << endl;
    cout << "Final weights: w1=" << weight1 << ", w2=" << weight2 << ", bias=" << bias << "\033[0m" << endl;
}

void testPerceptronArray(int input1[], int input2[], int output[], int n, int weight1, int weight2, int bias)
{
    cout << "Results:" << endl;
    for (int i = 0; i < n; i++)
    {
        int prediction = calculateActivation(input1[i], input2[i], weight1, weight2, bias);
        if (globalFileName == "andGate.txt")
        {
            cout << "\033[32m" << input1[i] << " AND " << input2[i] << " = " << prediction << " (Expected: " << yDesired[i] << ")"
                 << "\033[0m" << endl;
        }
        else if (globalFileName == "orGate.txt")
        {
            cout << "\033[32m" << input1[i] << " OR " << input2[i] << " = " << prediction << " (Expected: " << yDesired[i] << ")"
                 << "\033[0m" << endl;
        }
    }
}

void testPerceptronUser(int input1, int input2, int output[], int n, int weight1, int weight2, int bias)
{
    cout << "Results:" << endl;
    int prediction = calculateActivation(input1, input2, weight1, weight2, bias);
    if (globalFileName == "andGate.txt")
    {
        cout << "\033[32m" << input1 << " AND " << input2 << " = " << prediction << "\033[0m" << endl;
    }
    else if (globalFileName == "orGate.txt")
    {
        cout << "\033[32m" << input1 << " OR " << input2 << " = " << prediction << "\033[0m" << endl;
    }
}
