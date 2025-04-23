
int main(void)
{
    // Array for storing lines of code
    string codeLines[MAX_LINES];

    // 2D array for storing words of code
    string codeWords[MAX_LINES][MAX_WORDS_PER_LINE];

    // Variables for recording data of lines
    int numLines = 0, mainLine = 0, startLine = 0, kernelArgs = 0;

    // Variables for storing the IDs of mp messages;
    int sendID = 1, recvID = 1, bcastID = 1, reduceID = 1, IsendID = 1, IrecvID = 1;

    // Variables for ensuring correct headers are included and initialization is done
    bool mpiHeaderCheck = false, mpiInitCheck = false;
    bool cudaHeaderCheck = false;
    bool ompHeaderCheck = false;

    // Variables for storing variables of MPI, CUDA and OpenMP
    string mpiRank = "NULL";
    string kernelFuncName = "NULL";
    string ompLockVariable = "NULL";
    string cudaTidVariable = "NULL";
    string ompForLoopVariable = "NULL";

    // Open the input source code file
    std::ifstream inFile("DRB104.txt");
    std::string buffer;
    std::string sourceString;
    // Open the text file for recording some information about various key words in the input code
    ofstream OutFileStatic("StaticPhaseInfo.txt");

    // Vector for storing the return messages
    vector <returnMessage> messagesData;

    // Vectors for storing data of various commands
    vector <grammar_MPI_Send> mpiSendData;
    vector <grammar_MPI_Recv> mpiRecvData;
    vector <grammar_MPI_Isend> mpiIsendData;
    vector <grammar_MPI_Irecv> mpiIrecvData;
    vector <grammar_MPI_Bcast> mpiBcastData;
    vector <grammar_MPI_Reduce> mpiReduceData;
    vector <grammar_blockGridDims> cudaDimsData;
    vector <grammar_cudaMalloc> cudaMallocData;
    vector <grammar_cudaMemcpy> cudaMemcpyData;
    vector <grammar_omp_init_lock> ompLockData;

    // Variables for time measurement
    clock_t start, end;
    double cpu_time_used;

    // Start clock
    start = clock();

    // If file opened successfully
    if (inFile.is_open())
    {
      

                // Find the starting line of the code
                if (word == "#include")
                    startLine = numLines;

                // Find the main line of the code
                if (word == "main")
                    mainLine = numLines;

                // Add the word to the 2D words array
                codeWords[numLines][wordIndex] = word;
                wordIndex++;
            }
            // Add the line to 1D line array
            codeLines[numLines] = buffer;
            numLines++;
        }
        // Close the input file
        inFile.close();
    }
    // Print an error if unable to open file
    else
        OutFileStatic << "Error: Unable to open input file" << endl;

    OutFileStatic << "########################################################################" << endl;
    OutFileStatic << "################*******  Static Phase Information ******################" << endl;
    OutFileStatic << "########################################################################" << endl << endl;

    OutFileStatic << "Numer of lines = " << numLines << endl;
    OutFileStatic << "Main Line = " << mainLine << endl;
    OutFileStatic << "Start Line = " << startLine << endl;

    // Search the code for various keywords
    for (int i = 0; i < numLines; i++) {  
        for (int j = 0; j < MAX_WORDS_PER_LINE; j++) 
        {
            
            
            // If MPI_Comm_rank is found
            else if (codeWords[i][j] == "MPI_Comm_rank")
            {
                if (mpiInitCheck == true && mpiHeaderCheck == true)
                {
                    int commRankLineNumber = i;
                    OutFileStatic << "MPI_Comm_rank found at line " << commRankLineNumber << endl << endl;

                    // Extract the code line containing MPI_Comm_rank
                    string codeLine = codeLines[commRankLineNumber];

                    // Separate the words within brackets at commas
                    vector <string> separatedWords = separateAtCommas(codeLine);

                    // Determine the length
                    int vecLength = size(separatedWords);

                    // Check the grammar of MPI_Comm_rank
                    grammar_MPI_Comm_rank mpiCommRank;

                    // MPI_Comm_rank must have 2 arguments
                    if (vecLength != 2)
                    {
                        returnMessage message;
                        message.errorLine = commRankLineNumber;
                        message.Error = "\t(MPI) SYNTAX: Invalid number of arguments of MPI_Comm_rank";
                        message.Check = false;
                        message.Potential = false;
                        messagesData.push_back(message);
                    }
                    else
                    {
                        // Store MPI_Comm_rank data in structure
                        mpiCommRank.commrankLineNumber = commRankLineNumber;
                        mpiCommRank.commrankCommunicator = separatedWords[0];
                        mpiCommRank.commrankRank = separatedWords[1];

                        // Get the mpi rank variable
                        mpiRank = Get_Rank_Variable(mpiCommRank.commrankRank);
                        OutFileStatic << "MPI Rank variable is: " << mpiRank << endl << endl;
                    }
                }
                else
                {
                    if (mpiHeaderCheck == false)
                    {
                        returnMessage message;
                        message.errorLine = i;
                        message.Error = "\t(MPI) <mpi.h> HEADER file NOT INCLUDED";
                        message.Check = false;
                        message.Potential = false;
                        messagesData.push_back(message);
                    }
                    else
                    {
                        returnMessage message;
                        message.errorLine = i;
                        message.Error = "\t(MPI) Using MPI Commands WITHOUT PROPER MPI INITIALIZATION";
                        message.Check = false;
                        message.Potential = false;
                        messagesData.push_back(message);
                    }
                }
            }
                                // Find the rank of the Sender 
                                matchRankLine = findMPIRank(rankLine);

                                if (matchRankLine != "NULL")
                                {
                                    // Get the rank
                                    senderRank = matchRankLine.substr(matchRankLine.length() - 1);
                                    break;
                                }
                            }
                        }
                        if (senderRank != "NULL")
                            break;
                    }                   
                    // Extract the code line containing MPI_Send
                    string codeLine = codeLines[sendLineNumber];

                    // Separate the words within brackets at commas
                    vector <string> separatedWords = separateAtCommas(codeLine);

                    // Determine the length
                    int vecLength = size(separatedWords);
                        

                                // Test whether there is a corresponsding receive for this MPI_Send
                                returnMessage message = Check_MPI_Send(mpiSend, codeWords, codeLines, numLines, mpiRank, senderRank);

                                // If a correct matching receive is found
                                if (message.Check == true)
                                    mpiSend.correspondingReceive = message.errorLine;

                                // Save this MPI_Send's data in mpiSendData vector
                                mpiSendData.push_back(mpiSend);
                                messagesData.push_back(message);
                            }
                        }
                    }
                    // If send is not in a for loop
                    else {
                        // Check the grammar of this MPI_Send
                        grammar_MPI_Send mpiSend;

                        // MPI_Send must have 6 arguments
                        if (vecLength != 6)
                        {
                            returnMessage message;
                            message.errorLine = sendLineNumber;
                            message.Error = "\t(MPI) SYNTAX: Invalid number of arguments of MPI_Send";
                            message.Check = false;
                            message.Potential = false;
                            messagesData.push_back(message);
                        }
                        else
                        {
                            

                            // Test whether there is a corresponsding receive for this MPI_Send
                            returnMessage message = Check_MPI_Send(mpiSend, codeWords, codeLines, numLines, mpiRank, senderRank);

                            // If a correct matching receive is found
                            if (message.Check == true)
                                mpiSend.correspondingReceive = message.errorLine;

                            // Save this MPI_Send's data in mpiSendData vector
                            mpiSendData.push_back(mpiSend);
                            messagesData.push_back(message);
                        }
                    }
                }
                else
                {
                    if (mpiHeaderCheck == false)
                    {
                        returnMessage message;
                        message.errorLine = i;
                        message.Error = "\t(MPI) <mpi.h> HEADER file NOT INCLUDED";
                        message.Check = false;
                        message.Potential = false;
                        messagesData.push_back(message);
                    }
                    else
                    {
                        returnMessage message;
                        message.errorLine = i;
                        message.Error = "\t(MPI) Using MPI Commands WITHOUT PROPER MPI INITIALIZATION";
                        message.Check = false;
                        message.Potential = false;
                        messagesData.push_back(message);
                    }
                }
            }
            // If an MPI_Recv is found
            else if (codeWords[i][j] == "MPI_Recv")
            {
                if (mpiInitCheck == true && mpiHeaderCheck == true)
                {
                    string rankLine, receiverRank = "NULL";
                    int recvLineNumber = i;
                    int forLoopSize = 1;
                    OutFileStatic << "MPI_Recv found at line " << recvLineNumber << endl << endl;

                    // Identify the rank of receiver
                    for (int k = recvLineNumber - 1; k > 0; k--)
                    {
                        for (int l = 0; l < MAX_WORDS_PER_LINE; l++)
                        {
                                  message.Error = "\t(OpenMP) DEADLOCK: Cannot have nested openAcc for loop inside openMP parallel region";
                                    message.Check = false;
                                    message.Potential = false;
                                    messagesData.push_back(message);
                                    break;
                                }
                            }
                        }

                        critical section of the code is protected with locks
                        bool lockFound = false;
                        bool nestedFor = false;
                        bool criticalScenario = true;
                        int forCount = 0;
                        for (int k = pragmaLineNumber; k < numLines; k++)
                        {
                            // Check for nested for loops inside omp parallel block
                            string bracketLine = codeLines[k];
                            // Remove all spaces and tabs from the string
                            bracketLine.erase(std::remove_if(bracketLine.begin(), bracketLine.end(), [](unsigned char c) {
                                return std::isspace(c);
                                }), bracketLine.end());
                            if (bracketLine == "#pragmaompfor")
                            {
                                forCount++;
                                if (forCount == 2)
                                    nestedFor = true;
                                criticalScenario = false;
                            }
                            for (int l = 0; l < MAX_WORDS_PER_LINE; l++)
                            {
                                if (codeWords[k][l] == "omp_set_lock")
                                {
                                    lockFound = true;
                                    break;
                                }
                            }
                            if (lockFound == true)
                                break;
                        }
                        if (criticalScenario == false)
                        {
                            if (forCount == 2)
                            {
                                returnMessage message;
                                message.errorLine = pragmaLineNumber + 1;
                                message.Error = "\t(OpenMP) RACE CONDITION: Nested for loops in OpenMP parallel block";
                                message.Check = false;
                                message.Potential = false;
                                messagesData.push_back(message);
                            }
                        }
                        else
                        {
                            if (lockFound == true)
                            {
                                returnMessage message;
                                message.errorLine = pragmaLineNumber + 1;
                                message.Error = "\t(OpenMP) Critical section protected";
                                message.Check = true;
                                message.Potential = false;
                                messagesData.push_back(message);
                            }
                            else
                            {
                                returnMessage message;
                                message.errorLine = pragmaLineNumber + 1;
                                message.Error = "\t(OpenMP) RACE CONDITION: Critical section is not protected";
                                message.Check = false;
                                message.Potential = false;
                                messagesData.push_back(message);
                            }
                        }
                        
