#include "dynamic.h"

void ExecuteDynamic()
{
	// Open the potential errors file and input code file
	ifstream potentialErrorFile("PotentialErrors.txt");
	ifstream codeFile("test.txt");
	if (!potentialErrorFile || !codeFile) {
		cerr << "Error opening input files." << endl;
	}

    // Read the potential error file and store the error line numbers
    vector<pair<int, string>> errorInfo;
    string line;
    while (std::getline(potentialErrorFile, line)) 
    {
        if (line.find("Potential ERROR (Line ") != std::string::npos) 
        {
            // Position after "Line"
            size_t startPos = line.find("Line") + 5; 
            size_t endPos = line.find(")", startPos);
            if (startPos != string::npos && endPos != string::npos) {
                string lineNumberStr = line.substr(startPos, endPos - startPos);
                int lineNumber = stoi(lineNumberStr);
                
                getline(potentialErrorFile, line); // Read the next line for error type
                size_t typeStartPos = line.find("(") + 1;
                size_t typeEndPos = line.find(":");
                if (typeStartPos != std::string::npos && typeEndPos != std::string::npos) 
                {
                    std::string errorType = line.substr(typeStartPos, typeEndPos - typeStartPos);
                    errorInfo.emplace_back(lineNumber, errorType);
                }
            }
        }
    }
    if (errorInfo.size() > 0)
    {
        // Read the lines from the code file and replace potential error lines with instrumented code
        std::vector<std::string> instrumentedCodeLines;
        int lineNumber = 1;
        bool addCUDAInstrumented = false;
        //bool check = false;
        
        while (getline(codeFile, line))
        {
            auto errorLineIt = find_if(errorInfo.begin(), errorInfo.end(),
                [lineNumber](const pair<int, string>& info) {
                    return info.first == lineNumber;
                });

            if (errorLineIt != errorInfo.end())
            {
                // Replace potential error lines
                instrumentedCodeLines.push_back(line);

                // Add the potential error line with a comment
                string nextLine;
                getline(codeFile, nextLine);
                
                // Separate the words within brackets at commas
                vector <string> separatedWords = separateAtCommas(nextLine);

                string trimmedErrorType = errorLineIt->second;
                trimmedErrorType.erase(std::remove_if(trimmedErrorType.begin(), trimmedErrorType.end(), ::isspace), trimmedErrorType.end());
                if (trimmedErrorType != "OpenMP)PARALLELSECTION" && trimmedErrorType != "CUDA)DATARACE")
                {
                    instrumentedCodeLines.push_back("            /* ------------------------------------- */ ");
                    instrumentedCodeLines.push_back("            // The commented line below is the original code that has been replaced with the instrumented code");
                    instrumentedCodeLines.push_back("            // " + nextLine);
                    instrumentedCodeLines.push_back("            /* ------------------------------------- */ ");

                }

                // If the potential error is related to MPI_Recv
                if (trimmedErrorType == "MPI)WILDCARDSOURCE" || trimmedErrorType == "MPI)WILDCARDTAG")
                {
                    instrumentedCodeLines.push_back("            // Instrumented Code");
                    instrumentedCodeLines.push_back("            MPI_Request requestRecv;");
                    instrumentedCodeLines.push_back("            MPI_Status statusRecv;");
                    instrumentedCodeLines.push_back("            int flagRecv;");
                    instrumentedCodeLines.push_back("            MPI_Irecv(" + separatedWords[0] + ", " + separatedWords[1] + ", " + separatedWords[2] + ", " + separatedWords[3] + ", " + separatedWords[4] + ", " + separatedWords[5] + ", " + "&requestRecv);");
                    instrumentedCodeLines.push_back("            MPI_Wait(&requestRecv, &statusRecv);");
                    instrumentedCodeLines.push_back("            double start, end = 0.1;");
                    instrumentedCodeLines.push_back("            start = MPI_Wtime();");
                    instrumentedCodeLines.push_back("            while ((end - start) < 10)");
                    instrumentedCodeLines.push_back("                end = MPI_Wtime();");
                    instrumentedCodeLines.push_back("            MPI_Test(&requestRecv, &flagRecv, &statusRecv);");
                    instrumentedCodeLines.push_back("            if (!flag)");
                    instrumentedCodeLines.push_back("            {");
                    instrumentedCodeLines.push_back("                cout << \"Actual Error : There is a deadlock in MPI_Recv\" << endl;");
                    instrumentedCodeLines.push_back("                EXIT_FAILURE;");
                    instrumentedCodeLines.push_back("            }");
                    instrumentedCodeLines.push_back("            else");
                    instrumentedCodeLines.push_back("                cout << \"NO Error\" << endl;");
                    lineNumber += 2;
                    //check = true;
                }
                else if (trimmedErrorType == "MPI)BROADCAST")
                {
                    instrumentedCodeLines.push_back("            //Instrumented Code");
                    instrumentedCodeLines.push_back("            MPI_Request requestBcast;");
                    instrumentedCodeLines.push_back("            MPI_Status statusBcast;");
                    instrumentedCodeLines.push_back("            int flagBcast;");
                    instrumentedCodeLines.push_back("            MPI_Ibcast(" + separatedWords[0] + ", " + separatedWords[1] + ", " + separatedWords[2] + ", " + separatedWords[3] + ", " + separatedWords[4] + "&requestBcast);");
                    instrumentedCodeLines.push_back("            MPI_Wait(&requestBcast, &statusBcast);");
                    instrumentedCodeLines.push_back("            double start, end = 0.1;");
                    instrumentedCodeLines.push_back("            start = MPI_Wtime();");
                    instrumentedCodeLines.push_back("            while ((end - start) < 10)");
                    instrumentedCodeLines.push_back("                end = MPI_Wtime();");
                    instrumentedCodeLines.push_back("            MPI_Test(&requestBcast, &flagBcast, &statusBcast);");
                    instrumentedCodeLines.push_back("            if (!flagBcast)");
                    instrumentedCodeLines.push_back("            {");
                    instrumentedCodeLines.push_back("                cout << \"Actual Error : There is a deadlock in MPI_Bcast\" << endl;");
                    instrumentedCodeLines.push_back("                EXIT_FAILURE;");
                    instrumentedCodeLines.push_back("            }");
                    instrumentedCodeLines.push_back("            else");
                    instrumentedCodeLines.push_back("                cout << \"NO Error\" << endl;");
                    lineNumber += 2;
                }
                else if (trimmedErrorType == "OpenMP)PARALLELSECTION")
                {
                    instrumentedCodeLines.push_back("   //Instrumented Code");
                    instrumentedCodeLines.push_back("   /*------------------------------------------*/");
                    instrumentedCodeLines.push_back("       double start_time = omp_get_wtime();");
                    instrumentedCodeLines.push_back("       double timeout = 5;");
                    instrumentedCodeLines.push_back("   /*------------------------------------------*/");
                    instrumentedCodeLines.push_back(nextLine);
                    int nextFewLines = 50;
                    while (nextFewLines > 0)
                    {
                        getline(codeFile, nextLine);
                        if (nextLine.find("omp_set_lock(&lock2);") != string::npos)
                        {
                            instrumentedCodeLines.push_back("        //Instrumented Code");
                            instrumentedCodeLines.push_back("        /*------------------------------------------*/");
                            instrumentedCodeLines.push_back("        here1:");
                            instrumentedCodeLines.push_back("            int lockCheck1 = omp_test_lock(&lock2);");
                            instrumentedCodeLines.push_back("            double duration = omp_get_wtime() - start_time;");
                            instrumentedCodeLines.push_back("            if (!lockCheck1 && duration <= timeout) {");
                            instrumentedCodeLines.push_back("                goto here1;");
                            instrumentedCodeLines.push_back("            }");
                            instrumentedCodeLines.push_back("            else {");
                            instrumentedCodeLines.push_back("                if (!lockCheck1 && duration > timeout) {");
                            instrumentedCodeLines.push_back("                    cout << endl << \" * ****Deadlock in OpenMP section * ****\" << endl;");
                            instrumentedCodeLines.push_back("                    exit(1);");
                            instrumentedCodeLines.push_back("                }");
                            instrumentedCodeLines.push_back("                else {");
                            instrumentedCodeLines.push_back("       " + nextLine);
                            getline(codeFile, nextLine);
                            instrumentedCodeLines.push_back("       " + nextLine);
                            getline(codeFile, nextLine);
                            instrumentedCodeLines.push_back("       " + nextLine);
                            instrumentedCodeLines.push_back("                }");
                            instrumentedCodeLines.push_back("            }");
                            instrumentedCodeLines.push_back("        /* --------------------------------------------------------- */");
                            break;
                        }
                        else
                        {
                            instrumentedCodeLines.push_back(nextLine);
                        }
                        nextFewLines--;
                    }
                    int nextFewMoreLines = 100;
                    while (nextFewMoreLines > 0)
                    {
                        getline(codeFile, nextLine);
                        if (nextLine.find("omp_set_lock(&lock);") != string::npos)
                        {
                            instrumentedCodeLines.push_back("        //Instrumented Code");
                            instrumentedCodeLines.push_back("        /*------------------------------------------*/");
                            instrumentedCodeLines.push_back("        here2:");
                            instrumentedCodeLines.push_back("            int lockCheck2 = omp_test_lock(&lock);");
                            instrumentedCodeLines.push_back("            double duration = omp_get_wtime() - start_time;");
                            instrumentedCodeLines.push_back("            if (!lockCheck2 && duration <= timeout) {");
                            instrumentedCodeLines.push_back("                goto here2;");
                            instrumentedCodeLines.push_back("            }");
                            instrumentedCodeLines.push_back("            else {");
                            instrumentedCodeLines.push_back("                if (!lockCheck2 && duration > timeout) {");
                            instrumentedCodeLines.push_back("                    cout << endl << \" * ****Deadlock in OpenMP section * ****\" << endl;");
                            instrumentedCodeLines.push_back("                    exit(1);");
                            instrumentedCodeLines.push_back("                }");
                            instrumentedCodeLines.push_back("                else {");
                            instrumentedCodeLines.push_back("       " + nextLine);
                            getline(codeFile, nextLine);
                            instrumentedCodeLines.push_back("       " + nextLine);
                            getline(codeFile, nextLine);
                            instrumentedCodeLines.push_back("       " + nextLine);
                            instrumentedCodeLines.push_back("                }");
                            instrumentedCodeLines.push_back("            }");
                            instrumentedCodeLines.push_back("        /* --------------------------------------------------------- */");
                            break;
                        }
                        else
                        {
                            instrumentedCodeLines.push_back(nextLine);
                        }
                        nextFewMoreLines--;
                    }
                    lineNumber += 2;
                }
                else if (trimmedErrorType == "CUDA)DATARACE")
                {
                    instrumentedCodeLines.push_back(nextLine);
                    addCUDAInstrumented = true;
                    lineNumber++;
                }
                else
                {
                    lineNumber++;
                }
            }
            else
            {
                // Add instrumented code for checking data race in CUDA
                if (line.find("return 0;") != string::npos)
                {
                    if (addCUDAInstrumented == true)
                    {
                        instrumentedCodeLines.push_back("        //Instrumented Code");
                        instrumentedCodeLines.push_back("        /*------------------------------------------*/");
                        instrumentedCodeLines.push_back("        double hostSum = 0;");
                        instrumentedCodeLines.push_back("        double deviceSum = 0;");
                        instrumentedCodeLines.push_back("        for (int i = 0; i < vecSize; i++)");
                        instrumentedCodeLines.push_back("        {");
                        instrumentedCodeLines.push_back("            if (i == vecSize - 1)");
                        instrumentedCodeLines.push_back("                 vecA[i] += 1;");
                        instrumentedCodeLines.push_back("            else");
                        instrumentedCodeLines.push_back("                 vecA[i] += vecA[i + 1];");
                        instrumentedCodeLines.push_back("            hostSum += vecA[i];");
                        instrumentedCodeLines.push_back("            deviceSum += resultant_vec[i];");
                        instrumentedCodeLines.push_back("        }");
                        instrumentedCodeLines.push_back("        if (hostSum == deviceSum)");
                        instrumentedCodeLines.push_back("            cout << \"No DATA RACE in Kernel\" << endl;");
                        instrumentedCodeLines.push_back("        else");
                        instrumentedCodeLines.push_back("            cout << \"DATA RACE in CUDA Kernel\" << endl;");
                        instrumentedCodeLines.push_back("        /*------------------------------------------*/");
                        instrumentedCodeLines.push_back(" ");
                    }
                    instrumentedCodeLines.push_back(line);
                    lineNumber++;
                }
                else
                {
                    instrumentedCodeLines.push_back(line);
                    lineNumber++;
                }
            }
            
        }

        // Write the modified code lines to the instrumented code file
        ofstream instrumentedCodeFile("instrumentedCode.txt");
        if (!instrumentedCodeFile) {
            cerr << "Error creating instrumented code file." << endl;
        }
        for (const string& instrumentedCodeLine : instrumentedCodeLines) {
            instrumentedCodeFile << instrumentedCodeLine << "\n";
        }
        instrumentedCodeFile.close();
        cout << endl << "Instrumented code file created" << endl;
    }
    else
    {
        cout << endl << "No Potential Errors Found" << endl;
    }
    // Close the input and output files
    codeFile.close();
    potentialErrorFile.close();
}