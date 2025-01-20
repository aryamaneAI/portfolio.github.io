#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <cstdlib>
#include <unordered_set>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>
#include <queue>
#include <climits>
#include <tuple>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <conio.h>

using namespace std;
using namespace std::chrono;

const int INF = INT_MAX;

// Function to simulate a delayed output with a customizable delay
void delayedPrint(const string& message, int delayInSeconds = 1, bool typeEffect = false, int delayPerCharMs = 100)
{
    // If typeEffect is true, print one character at a time with delay
    if (typeEffect)
    {
        for (char c : message)
        {
            cout << c;
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(delayPerCharMs)); // Delay for typing effect
        }
    }
    else
    {
        // Default behavior: Print the message and wait for the specified delay in seconds
        cout << message << endl;
    }

    // Sleep for the specified number of seconds after printing the message
    this_thread::sleep_for(chrono::seconds(delayInSeconds));
}
// Function to print the message letter by letter in red with '.' in white
void delayedRedPrintInvalidLogin(const string& message) {
    int delayInMilliseconds = 100;  // Set the fixed delay time to 100 milliseconds
// Define color codes using ANSI escape sequences
const string RESET = "\033[0m";
const string RED= "\033[31m";
const string WHITE = "\033[37m";

    for (size_t i = 0; i < message.length(); ++i) {
        if (message[i] == '.') {
            // Print the '.' in white color
            cout << WHITE << message[i] << RESET;
        } else {
            // Print all other characters in red color
            cout << RED << message[i] << RESET;
        }

        // Flush the output to ensure it's printed immediately
        cout.flush();

        // Delay for the specified amount of time (100 ms)
        this_thread::sleep_for(chrono::milliseconds(delayInMilliseconds));
    }

    cout << endl;  // Newline after the message is printed
}

// Function to print the message letter by letter in green with '!' in white
void delayedGreenPrint(const string& message) {
    int delayInMilliseconds = 100;  // Fixed delay time of 100 milliseconds
// Define color codes using ANSI escape sequences
const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string WHITE = "\033[37m";

    for (size_t i = 0; i < message.length(); ++i) {
        if (message[i] == '!') {
            // Print the '!' in white color
            cout << WHITE << message[i] << RESET;
        } else {
            // Print all other characters in green color
            cout << GREEN << message[i] << RESET;
        }

        // Flush the output to ensure it's printed immediately
        cout.flush();

        // Delay for the specified amount of time (100 ms)
        this_thread::sleep_for(chrono::milliseconds(delayInMilliseconds));
    }

    cout << endl;  // Newline after the message is printed
}

// Boyer-Moore string matching algorithm
int boyerMooreSearch(const string& text, const string& pattern)
{
    int m = pattern.length();
    int n = text.length();

    // Create bad character shift table
    vector<int> badChar(256, -1);
    for (int i = 0; i < m; i++)
    {
        badChar[pattern[i]] = i;
    }

    int s = 0;  // shift of the pattern with respect to text
    while (s <= n - m)
        {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[s + j])
        {
            j--;
        }

        if (j < 0)
        {
            return s;  // Match found at shift s
            s += (s + m < n) ? m - badChar[text[s + m]] : 1;
        } else {
            s += max(1, j - badChar[text[s + j]]);
        }
    }

    return -1;  // No match found
}

// KMP (Knuth-Morris-Pratt) string matching algorithm
vector<int> computeLPSArray(const string& pattern)
{
    int m = pattern.length();
    vector<int> lps(m, 0);
    int length = 0;  // length of the previous longest prefix suffix

    int i = 1;
    while (i < m)
    {
        if (pattern[i] == pattern[length])
        {
            length++;
            lps[i] = length;
            i++;
        }
      else
        {
            if (length != 0)
            {
                length = lps[length - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

int KMPSearch(const string& text, const string& pattern)
{
    vector<int> lps = computeLPSArray(pattern);
    int n = text.length();
    int m = pattern.length();
    int i = 0;
    int j = 0;

    while (i < n)
    {
        if (pattern[j] == text[i])
        {
            i++;
            j++;
        }
        if (j == m)
        {
            return i - j;  // Match found
            j = lps[j - 1];
        }
        else if (i < n && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
    }
    return -1;  // No match found
}

// Masked password function (works as expected)
#ifdef _WIN32
#include <conio.h>  // For _getch() on Windows

string getMaskedPassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {  // '\r' is the Enter key
        if (ch == '\b') {  // Handle backspace
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}
#else
#include <termios.h>
#include <unistd.h>  // For read() on Unix-like systems

string getMaskedPassword()
{
    string password;
    char ch;
    struct termios oldt, newt;

    // Get current terminal attributes
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable echoing of input characters
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read password character by character
    while ((ch = getchar()) != '\n')
    {
        if (ch == 127 || ch == '\b')
        {  // Handle backspace
            if (!password.empty())
            {
                cout << "\b \b";
                password.pop_back();
            }
        }
    else
        {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;

    // Restore terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return password;
}
#endif

// Authentication function using Boyer-Moore (for login ID and password)
bool authenticate()
{
    string loginId, password;
    delayedPrint("\nEnter Login ID: ", 1);
    cin >> loginId;
    delayedPrint("Enter Password: ", 1);
    password = getMaskedPassword();  // Use the masked password input function

    // Predefined login credentials
    if (boyerMooreSearch("admin1", loginId) != -1 && password == "1234")
    {
        delayedGreenPrint("\nLogin successful!\n");
        return true;
    }
    else if (boyerMooreSearch("admin2", loginId) != -1 && password == "4567")
    {
       delayedGreenPrint("\nLogin successful!\n");
        return true;
    }
    else if (boyerMooreSearch("admin3", loginId) != -1 && password == "8901")
    {
        delayedGreenPrint("\nLogin successful!\n");
        return true;
    }
    else if (boyerMooreSearch("admin4", loginId) != -1 && password == "2345")
    {
       delayedGreenPrint("\nLogin successful!\n");
        return true;
    }
    else
    {
        delayedRedPrintInvalidLogin("\nInvalid Login ID or Password.\n");
        return false;
    }
}

// Authentication function using KMP (for login ID and password)
bool authenticate2()
{
    string loginId, password;
    delayedPrint("\nEnter Login ID: ", 1);
    cin >> loginId;
    delayedPrint("Enter Password: ", 1);
    password = getMaskedPassword();  // Use the masked password input function

    // Predefined login credentials
    if (KMPSearch("admin1", loginId) != -1 && password == "1234")
    {
      delayedGreenPrint("\nLogin successful!\n");
        return true;
    } else if (KMPSearch("admin2", loginId) != -1 && password == "4567")
    {
        delayedGreenPrint("\nLogin successful!\n");
        return true;
    } else if (KMPSearch("admin3", loginId) != -1 && password == "8901")
    {
       delayedGreenPrint("\nLogin successful!\n");
        return true;
    } else if (KMPSearch("admin4", loginId) != -1 && password == "2345")
    {
       delayedGreenPrint("\nLogin successful!\n");
        return true;
    } else {
        delayedRedPrintInvalidLogin("\nInvalid Login ID or Password.\n");
        return false;
    }
}

// Function to generate random garbage collection values between 50 and 70 kg
int generateRandomGarbage()
{
    return rand() % 21 + 50; // Random number between 50 and 70
}

// Function to print a separator line with a decorative border
void printSeparator()
{
    cout << "\n******************************************************\n";
}

// Function to generate records for each day of the month
void generateMonthlyGarbageRecords()
{
    ofstream file("garbage_records_full_month.txt");

    if (!file.is_open())
    {
        cout << "Error opening the file.\n";
        return;
    }

    // Set the seed for random number generation
    srand(time(0));

    // Variables to track total garbage generated throughout the month
    int totalMonthlyGarbage = 0;
    int colonyTotalGarbage[9] = {0};  // Array to track garbage by each colony

    // Iterate over 31 days (January)
    for (int day = 1; day <= 31; ++day)
    {
       string date = "2025-01-" + (day < 10 ? "0" + to_string(day) : to_string(day));// Pad day with leading zero if necessary

        // Begin the design section with a border
        printSeparator();

        // Print for the date and header
        cout << "     *** Garbage Collection Report ***     " << endl;
        cout << "                 " + date + "                 " << endl;
        printSeparator();

        int totalGarbage = 0;
        int maxGarbage = 0;
        string largestColony;

        // Simulate garbage collection for 9 colonies and track total and max garbage
        for (int colony = 1; colony <= 9; ++colony)
            {
            int garbage = generateRandomGarbage();
            totalGarbage += garbage;
            colonyTotalGarbage[colony - 1] += garbage;  // Add to the specific colony's total

            // Track the colony with the largest garbage production
            if (garbage > maxGarbage)
            {
                maxGarbage = garbage;
                largestColony = "Colony-" + to_string(colony);
            }

            // Print the colony data without delay
            string colonyInfo = "Colony-" + to_string(colony) + ": " + to_string(garbage) + " kg";
            cout << colonyInfo << endl;

            // Write the colony data to the file
            file << colonyInfo << endl;
        }

        // Print for the total garbage and largest producer
        cout << "Total Garbage Collected: " + to_string(totalGarbage) + " kg" << endl;
        cout << "Largest Garbage Producer: " + largestColony + " with " + to_string(maxGarbage) + " kg" << endl;

        // End the date section with a border
        printSeparator();

        // Write the summary data to the file
        file << "Total Garbage Collected: " << totalGarbage << " kg" << endl;
        file << "Largest Garbage Producer: " << largestColony << " with " << maxGarbage << " kg" << endl;
        file << endl; // Add a blank line for separation

        // Accumulate the total garbage for the month
        totalMonthlyGarbage += totalGarbage;
    }

    // After the loop, print the total garbage for the month and the total garbage for each colony
    delayedPrint("\n*** Monthly Garbage Summary ***\n", 2);
    delayedPrint("Total Garbage Collected for the Month: " + to_string(totalMonthlyGarbage) + " kg", 2);

    // Print total garbage generated by each colony throughout the month
    for (int colony = 1; colony <= 9; ++colony)
    {
        delayedPrint("Total Garbage Collected by Colony-" + to_string(colony) + ": " + to_string(colonyTotalGarbage[colony - 1]) + " kg", 2);
    }

    // Find and display the colony with the largest total garbage collected
    int maxColonyIndex = 0;
    for (int i = 1; i < 9; ++i)
    {
        if (colonyTotalGarbage[i] > colonyTotalGarbage[maxColonyIndex])
        {
            maxColonyIndex = i;
        }
    }

    // Print the colony with the highest total garbage collected
    delayedPrint("\nLargest Garbage Collected from Colony-" + to_string(maxColonyIndex + 1) + ": " + to_string(colonyTotalGarbage[maxColonyIndex]) + " kg", 2);
}

// Function to display the contents of the generated file
void displayFileContents()
{
    ifstream file("garbage_records_full_month.txt");

    if (!file.is_open())
    {
        cout << "Error opening the file to display its contents.\n";
        return;
    }

    // Check if the file is empty
    if (file.peek() == ifstream::traits_type::eof())
    {
        delayedPrint("The file is empty. No data found.", 2);
        return;
    }

    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
}

// Function to calculate and display the average garbage collected per day
void displayAverageGarbagePerDay()
{
    ifstream file("garbage_records_full_month.txt");

    if (!file.is_open())
    {
        cout << "Error opening the file to read its contents.\n";
        return;
    }

    // Check if the file is empty
    if (file.peek() == ifstream::traits_type::eof())
    {
        delayedPrint("The file is empty. No data found.", 2);
        return;
    }

    string line;
    int totalGarbage = 0;
    int dayCount = 0;

    while (getline(file, line))
        {
        if (line.find("Total Garbage Collected:") != string::npos)
        {
            size_t pos = line.find(":");
            int garbage = stoi(line.substr(pos + 2));
            totalGarbage += garbage;
            dayCount++;
        }
    }

    if (dayCount > 0)
    {
        int averageGarbage = totalGarbage / dayCount;
        delayedPrint("Average Garbage Collected per Day: " + to_string(averageGarbage) + " kg", 2);
    }
    else
    {
        delayedPrint("No garbage collection data found.", 2);
    }
}

// Function to generate summary reports for each colony
void generateColonyReports()
{
    ifstream file("garbage_records_full_month.txt");

    if (!file.is_open())
    {
        cout << "Error opening the file to read its contents.\n";
        return;
    }

    // Check if the file is empty
    if (file.peek() == ifstream::traits_type::eof())
    {
        delayedPrint("The file is empty. No data found.", 2);
        return;
    }

    string line;
    int colonyTotalGarbage[9] = {0};
    int colonyDayCount[9] = {0};

    while (getline(file, line))
    {
        if (line.find("Colony-") != string::npos)
        {
            size_t pos1 = line.find("-") + 1;
            size_t pos2 = line.find(":");
            int colony = stoi(line.substr(pos1, pos2 - pos1)) - 1;
            int garbage = stoi(line.substr(pos2 + 2));
            colonyTotalGarbage[colony] += garbage;
            colonyDayCount[colony]++;
        }
    }

    // Print summary reports for each colony
    delayedPrint("\n*** Colony Garbage Summary ***\n", 2);
    for (int colony = 0; colony < 9; ++colony)
        {
        delayedPrint("Colony-" + to_string(colony + 1) + ":", 2);
        delayedPrint("  Total Garbage Collected: " + to_string(colonyTotalGarbage[colony]) + " kg", 2);
        if (colonyDayCount[colony] > 0)
        {
            int averageGarbage = colonyTotalGarbage[colony] / colonyDayCount[colony];
            delayedPrint("  Average Garbage per Day: " + to_string(averageGarbage) + " kg", 2);
        } else {
            delayedPrint("  No data available.", 2);
        }
    }
}

// Function to save the monthly records to a file
void saveMonthlyRecords()
{
    ifstream file("garbage_records_full_month.txt");

    if (!file.is_open())
    {
        cout << "Error opening the file to read its contents.\n";
        return;
    }

    ofstream backupFile("garbage_records_backup.txt");

    if (!backupFile.is_open())
    {
        cout << "Error opening the backup file for writing.\n";
        return;
    }

    string line;
    while (getline(file, line))
    {
        backupFile << line << endl;
    }

    delayedPrint("Monthly records have been saved to garbage_records_backup.txt", 2);
}

// Function to load the monthly records from a file
void loadMonthlyRecords()
{
    ifstream backupFile("garbage_records_backup.txt");

    if (!backupFile.is_open())
    {
        cout << "Error opening the backup file to read its contents.\n";
        return;
    }

    ofstream file("garbage_records_full_month.txt");

    if (!file.is_open())
    {
        cout << "Error opening the file for writing.\n";
        return;
    }

    string line;
    while (getline(backupFile, line))
    {
        file << line << endl;
    }

    delayedPrint("Monthly records have been loaded from garbage_records_backup.txt", 2);
}

// Function to clear the monthly records
void clearMonthlyRecords()
{
    ofstream file("garbage_records_full_month.txt", ios::trunc); // Open file in truncate mode

    if (!file.is_open())
    {
        cout << "Error opening the file to clear its contents.\n";
        return;
    }

    delayedPrint("Monthly records have been cleared.", 2);
}

// Driver and Worker details
struct Driver
{
    string name;
    int id;
    string license;
};

struct Worker
{
    string name;
    int id;
    vector<string> accessories;  // Accessories taken by the worker
};

// Garbage Truck with multiple drivers and workers
struct GarbageTruck {
    Driver driver;
    vector<Worker> workers;
    string location;
    vector<string> servicedBins;  // Track serviced bins for each truck
    int currentBinStatus[8];      // Status of bins in the colony (0: empty, 1: full)
    int garbageCollected;         // Amount of garbage collected by the truck
    int petrolUsed;               // Amount of petrol used by the truck
};


// City map as an adjacency matrix
class CityMap
{
    int numNodes;
    vector<vector<int>> adjacencyMatrix;

public:
    CityMap(int nodes) : numNodes(nodes), adjacencyMatrix(nodes, vector<int>(nodes, INF))
    {
        for (int i = 0; i < nodes; i++)
        {
            adjacencyMatrix[i][i] = 0;
        }
    }

    void addRoad(int u, int v, int weight)
    {
        if (u >= 0 && u < numNodes && v >= 0 && v < numNodes)
        {
            adjacencyMatrix[u][v] = weight;
            adjacencyMatrix[v][u] = weight;
        }
        else
        {
            cout << "Invalid road between " << u << " and " << v << endl;
        }
    }

    const vector<vector<int>>& getAdjacencyMatrix() const
    {
        return adjacencyMatrix;
    }

    int getNumNodes() const
    {
        return numNodes;
    }
};

// Dijkstra's Algorithm to find shortest paths from a source
void dijkstra(int src, const vector<vector<int>>& graph, vector<int>& dist, vector<int>& path)
{
    int n = graph.size();
    dist = vector<int>(n, INF);
    path = vector<int>(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty())
        {
        int u = pq.top().second;
        pq.pop();

        for (int v = 0; v < n; ++v)
            {
            if (graph[u][v] != INF && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                path[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

// Function to display accessories taken by workers of each truck
void displayAccessories(const vector<GarbageTruck>& trucks)
{
    delayedPrint("\nAccessories Taken by Workers of Each Truck:\n", 1);

    map<string, int> accessoryCount; // Map to count the occurrences of each accessory
    int maxAccessories = 0;
    string workerWithMaxAccessories;
    string truckWithMaxAccessories;

    for (int i = 0; i < trucks.size(); ++i)
        {
        delayedPrint("Truck " + to_string(i + 1) + " at " + trucks[i].location + ":\n", 1);
        int totalAccessoriesForTruck = 0;

        for (const auto& worker : trucks[i].workers)
            {
            delayedPrint("  Worker " + worker.name + " (ID: " + to_string(worker.id) + "): ", 1);
            for (const auto& accessory : worker.accessories)
            {
                delayedPrint(accessory + " ", 1);
                accessoryCount[accessory]++;
                totalAccessoriesForTruck++;
            }
            delayedPrint("\n", 1);

            if (worker.accessories.size() > maxAccessories)
            {
                maxAccessories = worker.accessories.size();
                workerWithMaxAccessories = worker.name;
                truckWithMaxAccessories = "Truck " + to_string(i + 1) + " at " + trucks[i].location;
            }
        }
        delayedPrint("  Total Accessories for Truck " + to_string(i + 1) + ": " + to_string(totalAccessoriesForTruck) + "\n", 1);
    }

    if (!accessoryCount.empty())
    {
        auto mostCommonAccessory = max_element(accessoryCount.begin(), accessoryCount.end(),
            [](const pair<string, int>& a, const pair<string, int>& b)
            {
                return a.second < b.second;
            });

        delayedPrint("\nMost Common Accessory: " + mostCommonAccessory->first + " (used " + to_string(mostCommonAccessory->second) + " times)\n", 1);
    }

    if (maxAccessories > 0)
    {
        delayedPrint("Worker with Most Accessories: " + workerWithMaxAccessories + " from " + truckWithMaxAccessories + " with " + to_string(maxAccessories) + " accessories\n", 1);
    }
}
// Function to check the bin status of a colony
void checkBinStatus(const vector<GarbageTruck>& trucks) {
    delayedPrint("\nChecking bin status for each colony:\n", 50);
    for (const auto& truck : trucks) {
        delayedPrint("\n" + truck.location + ":\n", 50);
        for (int i = 0; i < 8; ++i) {
            string status = truck.currentBinStatus[i] == 1 ? "Full" : "Empty";
            delayedPrint("Bin " + to_string(i + 1) + ": " + status + "\n", 50);
        }
    }
}


// Function to find the nearest garbage truck to a full bin
void findNearestTruck(int binLocation, const vector<string>& places, const vector<GarbageTruck>& trucks, const vector<vector<int>>& graph)
{
    vector<int> dist;
    vector<int> path;
    dijkstra(binLocation, graph, dist, path);

    int nearestTruckIndex = -1;
    int minDist = INF;

    for (int i = 0; i < trucks.size(); ++i)
    {
        int truckLocation = distance(places.begin(), find(places.begin(), places.end(), trucks[i].location));
        if (dist[truckLocation] < minDist)
        {
            minDist = dist[truckLocation];
            nearestTruckIndex = i;
        }
    }

    if (nearestTruckIndex != -1)
    {
        delayedPrint("Nearest garbage truck is at " + trucks[nearestTruckIndex].location + " with distance " + to_string(minDist) + " km.\n", 1);
    }
    else
    {
        delayedPrint("No garbage trucks available nearby.\n", 1);
    }
}
// Function to display the daily totals of garbage collected and petrol used
void displayDailyTotals(const vector<GarbageTruck>& trucks)
{
    delayedPrint("\nDaily Totals:\n", 1);
    int totalGarbageCollected = 0;
    int totalPetrolUsed = 0;

    for (const auto& truck : trucks)
    {
        totalGarbageCollected += truck.garbageCollected;
        totalPetrolUsed += truck.petrolUsed;
    }

    delayedPrint("Total Garbage Collected Today: " + to_string(totalGarbageCollected) + " kg\n", 1);
    delayedPrint("Total Petrol Used Today: " + to_string(totalPetrolUsed) + " liters\n", 1);
}
// Function to display the amount of petrol used by each truck
void displayPetrolUsed(const vector<GarbageTruck>& trucks)
{
    delayedPrint("\nPetrol Used by Each Truck:\n", 1);
    int totalPetrolUsed = 0;
    int maxPetrolUsed = 0;
    int maxPetrolTruckIndex = -1;

    for (int i = 0; i < trucks.size(); ++i)
    {
        delayedPrint("Truck " + to_string(i + 1) + " at " + trucks[i].location + ": " + to_string(trucks[i].petrolUsed) + " liters\n", 1);
        totalPetrolUsed += trucks[i].petrolUsed;
        if (trucks[i].petrolUsed > maxPetrolUsed)
        {
            maxPetrolUsed = trucks[i].petrolUsed;
            maxPetrolTruckIndex = i;
        }
    }

    delayedPrint("\nTotal Petrol Used by All Trucks: " + to_string(totalPetrolUsed) + " liters\n", 1);

    if (!trucks.empty())
    {
        int averagePetrolUsed = totalPetrolUsed / trucks.size();
        delayedPrint("Average Petrol Used per Truck: " + to_string(averagePetrolUsed) + " liters\n", 1);
    }

    if (maxPetrolTruckIndex != -1)
    {
        delayedPrint("Truck with Highest Petrol Usage: Truck " + to_string(maxPetrolTruckIndex + 1) + " at " + trucks[maxPetrolTruckIndex].location + " with " + to_string(maxPetrolUsed) + " liters\n", 1);
    }
}

// Display shortest distances and paths with delays
void displayShortestPaths(int src, const vector<int>& dist, const vector<int>& path, const vector<string>& places)
{
    int n = dist.size();
    delayedPrint("\nSource Place: " + places[src] + "\n", 1);
    delayedPrint("Shortest distances from " + places[src] + ":\n", 1);
    for (int i = 0; i < n; ++i)
    {
        if (dist[i] == INF)
        {
            delayedPrint(places[i] + ": Unreachable\n", 1);
        }
        else
        {
            delayedPrint(places[i] + ": " + to_string(dist[i]) + " km\n", 1);
        }
    }

    delayedPrint("\nPaths from " + places[src] + ":\n", 1);
    for (int i = 0; i < n; ++i)
    {
        if (i != src && dist[i] != INF)
        {
            delayedPrint("Path to " + places[i] + ": ", 1);
            int current = i;
            string path_str = places[i];
            while (current != src)
            {
                current = path[current];
                path_str = places[current] + " <- " + path_str;
            }
            delayedPrint(path_str + "\n", 1);
        }
    }
}

// Function to display garbage truck details in tabular format
void displayTruckDetails(const vector<GarbageTruck>& trucks)
{
    const string brightRed = "\033[1;31m";  // Bright Red color code for table border
    const string cyan = "\033[36m";        // Cyan color code for data
    const string reset = "\033[0m";        // Reset color to default

    delayedPrint("\nGarbage Truck Details:\n", 1);

    // Header for the table with bright red border
    cout << brightRed << "+----+----------------+---------+----------------+----------------------------------+" << reset << endl;
    cout << brightRed << "| No | Driver Name    | ID      | License Plate  | Location & Workers              |" << reset << endl;
    cout << brightRed << "+----+----------------+---------+----------------+----------------------------------+" << reset << endl;

    // Display the details for each garbage truck
    for (int i = 0; i < trucks.size(); ++i)
    {
        // Row for each truck with bright red border and cyan data
        cout << brightRed << "| " << setw(2) << (i + 1) << " | "
             << cyan << setw(14) << trucks[i].driver.name << reset << " | "
             << cyan << setw(7) << trucks[i].driver.id << reset << " | "
             << cyan << setw(14) << trucks[i].driver.license << reset << " | "
             << cyan << setw(30) << trucks[i].location << reset << " |" << endl;

        // Add workers under each truck, in cyan
        for (const auto& worker : trucks[i].workers)
        {
            cout << brightRed << "|    |                |         |                | Worker: "
                 << cyan << setw(14) << worker.name << reset << " (ID: "
                 << cyan << worker.id << reset << ") |" << endl;
        }

        // End of row for each truck with bright red border
        cout << brightRed << "+----+----------------+---------+----------------+----------------------------------+" << reset << endl;

        // Add a small delay between each truck
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}
// Function to print a styled, centered message with a delay
// Function to print a styled centered message with colors
void printStyledCenteredMessage(const string& message, int delayMs = 100, int screenWidth = 80)
{
    const string borderColor = "\033[38;5;45m";  // Extended Color 3 (Border)
    const string textColor = "\033[94m";         // Bright Cyan (Text)
    const string resetColor = "\033[0m";         // Reset color to default

    // Calculate the number of spaces needed to center the message
    int spacesBeforeText = (screenWidth - message.length()) / 2;

    // Print the top border in Extended Color 3
    for (int i = 0; i < screenWidth; ++i)
    {
        cout << borderColor << "#";  // Border symbol in Extended Color 3
    }
    cout << resetColor << endl;

    // Print a small space before the message
    cout << endl;

    // Print spaces to center the message
    for (int i = 0; i < spacesBeforeText; ++i)
    {
        cout << " ";  // Spaces for centering
    }

    // Print the message in Bright Cyan with a delay for each character (typing effect)
    cout << textColor;
    for (char c : message)
    {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delayMs)); // Adjustable delay
    }
    cout << resetColor << endl;

    // Print a small space after the message
    cout << endl;

    // Print the bottom border in Extended Color 3
    for (int i = 0; i < screenWidth; ++i)
    {
        cout << borderColor << "#";  // Border symbol in Extended Color 3
    }
    cout << resetColor << endl;
}
// Function to display the main menu for garbage truck management
void displayTruckManagementMenu()
{
    printStyledCenteredMessage("\n\t\t*** Garbage Truck Management System ***\n", 50, 80);
    delayedPrint("1. Shortcut Routes\n", 1);
    delayedPrint("2. Login for More Details\n", 1);
    delayedPrint("3. Service Bins at All Locations\n", 1);
    delayedPrint("4. Check Bin Status\n", 1);
    delayedPrint("5. Find Nearest Garbage Truck to Full Bin\n", 1);
    delayedPrint("6. Display Total Garbage Collected and Petrol Used Today\n", 1);
    delayedPrint("7. Exit\n", 1);
    delayedPrint("Enter your choice: ", 1);
}
// Function to display the amount of garbage collected by each truck
void displayGarbageCollected(const vector<GarbageTruck>& trucks)
{
    delayedPrint("\nGarbage Collected by Each Truck:\n", 1);
    int totalGarbageCollected = 0;
    int maxGarbageCollected = 0;
    int maxGarbageTruckIndex = -1;

    for (int i = 0; i < trucks.size(); ++i)
    {
        delayedPrint("Truck " + to_string(i + 1) + " at " + trucks[i].location + ": " + to_string(trucks[i].garbageCollected) + " kg\n", 1);
        totalGarbageCollected += trucks[i].garbageCollected;
        if (trucks[i].garbageCollected > maxGarbageCollected)
        {
            maxGarbageCollected = trucks[i].garbageCollected;
            maxGarbageTruckIndex = i;
        }
    }

    delayedPrint("\nTotal Garbage Collected by All Trucks: " + to_string(totalGarbageCollected) + " kg\n", 1);

    if (!trucks.empty())
    {
        int averageGarbageCollected = totalGarbageCollected / trucks.size();
        delayedPrint("Average Garbage Collected per Truck: " + to_string(averageGarbageCollected) + " kg\n", 1);
    }

    if (maxGarbageTruckIndex != -1)
    {
        delayedPrint("Truck with Highest Garbage Collection: Truck " + to_string(maxGarbageTruckIndex + 1) + " at " + trucks[maxGarbageTruckIndex].location + " with " + to_string(maxGarbageCollected) + " kg\n", 1);
    }
}

// Function to print a message with a delay, centered, and aesthetic framing
void printCenteredWithDelay(const string& message, int delayMs = 100, int screenWidth = 80)
{
    const string borderColor = "\033[38;5;45m";  // Extended Color 3 for border
    const string textColor = "\033[94m";         // Bright Blue for text
    const string resetColor = "\033[0m";         // Reset color to default

    // Calculate the number of spaces to center the message
    int spacesBeforeText = (screenWidth - message.length()) / 2;

    // Print the top border in Extended Color 3
    for (int i = 0; i < screenWidth; ++i)
    {
        cout << borderColor << "#";  // Border symbol in Extended Color 3
    }
    cout << resetColor << endl;

    // Print a small space before the message
    cout << endl;

    // Print spaces to center the message
    for (int i = 0; i < spacesBeforeText; ++i)
    {
        cout << " ";  // Spaces for centering
    }

    // Print the message in Bright Blue with a delay for each character (typing effect)
    cout << textColor;
    for (char c : message)
    {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delayMs)); // Adjustable delay
    }
    cout << resetColor << endl;

    // Print a small space after the message
    cout << endl;

    // Print the bottom border in Extended Color 3
    for (int i = 0; i < screenWidth; ++i)
    {
        cout << borderColor << "#";  // Border symbol in Extended Color 3
    }
    cout << resetColor << endl;
}
void serviceBin(vector<GarbageTruck>& trucks, int truckIndex, const vector<string>& places) {
    if (truckIndex < 0 || truckIndex >= trucks.size()) {
        cout << "Invalid truck index!" << endl;
        return;
    }
    delayedPrint(".....................................\n", 50);
    this_thread::sleep_for(milliseconds(2000));
    delayedPrint("Servicing bins by truck at " + trucks[truckIndex].location + "...\n", 50);
    this_thread::sleep_for(milliseconds(5000));

    // Simulate servicing process
    trucks[truckIndex].servicedBins.push_back(trucks[truckIndex].location);
    trucks[truckIndex].garbageCollected += rand() % 50 + 1;  // Random amount of garbage collected
    trucks[truckIndex].petrolUsed += rand() % 10 + 1;        // Random amount of petrol used
    delayedPrint("\nBin serviced at " + trucks[truckIndex].location + "\n", 50);
}

void displayAllBinStatus(vector<GarbageTruck>& trucks) {
    for (auto& truck : trucks) {
        cout<<"\n" + truck.location + ":\n", 50;
        for (int i = 0; i < 8; ++i) {
            // Randomly set bin status to full or empty
            truck.currentBinStatus[i] = rand() % 2;
            string status = truck.currentBinStatus[i] == 1 ? "Full" : "Empty";
            cout<<"Bin " + to_string(i + 1) + ": " + status + "\n", 50;
        }
    }
}
void slowPrint(const std::string& message, int delayMs) {
    for (char ch : message) {
        std::cout << ch;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs)); // Delay between characters
    }
    std::cout << std::endl;
}

// Main function for garbage truck management
void garbageTruckManagement()
{
    // Colonies (node indices mapped to places)
    vector<string> places =
    {
        "Colony-1",
        "Colony-2",
        "Colony-3",
        "Colony-4",
        "Colony-5",
        "Colony-6",
        "Colony-7",
        "Colony-8",
        "Colony-9"
    };

    int numNodes = places.size();  // Number of places (nodes)

    CityMap city(numNodes);

    // Roads and distances (weights)
    vector<tuple<int, int, int>> roads =
    {
        {0, 1, 10},
        {0, 2, 5},
        {1, 2, 2},
        {1, 3, 6},
        {2, 3, 9},
        {3, 4, 4},
        {4, 5, 8},
        {5, 6, 3},
        {7, 8, 5},
        {0, 7, 12},
        {8, 2, 6}
    };

    // Adding the roads to the city map
    for (const auto& road : roads)
    {
        city.addRoad(get<0>(road), get<1>(road), get<2>(road));
    }
   // Garbage trucks setup
    vector<GarbageTruck> trucks(9); // Now there are 9 trucks for 9 colonies

    // First truck details
    trucks[0].driver = {"Rajesh Kumar", 1, "KA12345"};
    trucks[0].workers = {
        {"Amit Sharma", 1, {"Gloves", "Mask"}},
        {"Deepika Verma", 2, {"Gloves", "Mask", "Helmet"}},
        {"Anil Patel", 3, {"Gloves"}}
    };
    trucks[0].location = "Colony-1";
    trucks[0].garbageCollected = 0;
    trucks[0].petrolUsed = 0;

    // Second truck details
    trucks[1].driver = {"Suresh Rao", 2, "KA67890"};
    trucks[1].workers = {
        {"Sonal Singh", 4, {"Gloves", "Mask"}},
        {"Krishna Kumar", 5, {"Gloves", "Mask"}},
        {"Neha Iyer", 6, {"Gloves", "Mask", "Helmet"}}
    };
    trucks[1].location = "Colony-2";
    trucks[1].garbageCollected = 0;
    trucks[1].petrolUsed = 0;

    // Third truck details
    trucks[2].driver = {"Vikram Yadav", 3, "KA11223"};
    trucks[2].workers = {
        {"Priya Joshi", 7, {"Gloves", "Mask"}},
        {"Ravi Naidu", 8, {"Gloves", "Mask"}},
        {"Sunita Bhat", 9, {"Gloves", "Mask", "Helmet"}}
    };
    trucks[2].location = "Colony-3";
    trucks[2].garbageCollected = 0;
    trucks[2].petrolUsed = 0;

    // Fourth truck details
    trucks[3].driver = {"Arjun Malshet", 4, "KA4362"};
    trucks[3].workers = {
        {"Reshma Jadhav", 10, {"Gloves", "Mask"}},
        {"Savita Gunde", 11, {"Gloves", "Mask"}},
        {"Kanta Kudal", 12, {"Gloves", "Mask", "Helmet"}}
    };
    trucks[3].location = "Colony-4";
    trucks[3].garbageCollected = 0;
    trucks[3].petrolUsed = 0;

    // Fifth truck details
    trucks[4].driver = {"Manoj Kumar", 5, "KA1455"};
    trucks[4].workers = {
        {"Kallu Bamnekar", 10, {"Gloves", "Mask"}},
        {"Leela Matre", 11, {"Gloves", "Mask"}},
        {"Keshav Karwari", 12, {"Gloves", "Mask", "Helmet"}}
    };
    trucks[4].location = "Colony-5";
    trucks[4].garbageCollected = 0;
    trucks[4].petrolUsed = 0;

    // Sixth truck details
    trucks[5].driver = {"Baman Kandur", 6, "KA1545"};
    trucks[5].workers = {
        {"Ram Boker", 13, {"Gloves", "Mask"}},
        {"Sita Matre", 14, {"Gloves", "Mask"}},
        {"Gopal Karwari", 15, {"Gloves", "Mask", "Helmet"}}
    };
    trucks[5].location = "Colony-6";
    trucks[5].garbageCollected = 0;
    trucks[5].petrolUsed = 0;

    // Seventh truck details
    trucks[6].driver = {"Mangesh Gavade", 7, "KA1655"};
    trucks[6].workers = {
        {"Ravi Apte", 16, {"Gloves", "Mask"}},
        {"Jayshree Chavhan", 17, {"Gloves", "Mask"}},
        {"Ramya Devidas", 18, {"Gloves", "Mask", "Helmet"}}
    };
    trucks[6].location = "Colony-7";
    trucks[6].garbageCollected = 0;
    trucks[6].petrolUsed = 0;

    // Eighth truck details
    trucks[7].driver = {"Amit Jadhav", 8, "KA1755"};
    trucks[7].workers = {
        {"Ravi Devidas", 19, {"Gloves", "Mask"}},
        {"Kiran Devidas", 20, {"Gloves", "Mask"}},
        {"Surendra Karoli", 21, {"Gloves", "Mask", "Helmet"}}
    };
    trucks[7].location = "Colony-8";
    trucks[7].garbageCollected = 0;
    trucks[7].petrolUsed = 0;

    // Ninth truck details
    trucks[8].driver = {"Ramesh Chadarmod", 9, "KA1855"};
    trucks[8].workers = {
        {"Vikram Salunkhe", 22, {"Gloves", "Mask"}},
        {"Jayanti Payil", 23, {"Gloves", "Mask"}},
        {"Ashmita Kalvekar", 24, {"Gloves", "Mask", "Helmet"}}
    };
    trucks[8].location = "Colony-9";
    trucks[8].garbageCollected = 0;
    trucks[8].petrolUsed = 0;

    // Initialize bin status (randomly setting some bins as full)
    srand(time(0));
    for (auto& truck : trucks)
    {
        for (int i = 0; i < 8; ++i)
        {
            truck.currentBinStatus[i] = rand() % 2;
        }
    }

    // Menu
    while (true)
    {
        displayTruckManagementMenu();

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            delayedPrint("\nAvailable places to choose from as the current Location:\n", 1);
            for (int i = 0; i < numNodes; ++i)
            {
                delayedPrint(to_string(i) + ": " + places[i] + "\n", 1);
            }

            int src;
            delayedPrint("\nEnter the location (0 to " + to_string(numNodes - 1) + "): ", 1);
            cin >> src;

            if (src < 0 || src >= numNodes)
            {
                cout << "Invalid node selected.\n";
                continue;
            }

            vector<int> dist, path;
            dijkstra(src, city.getAdjacencyMatrix(), dist, path);
            displayShortestPaths(src, dist, path, places);

        }
        else if (choice == 2)
            {
            if (authenticate())
            {
                while (true)
                {
                    printCenteredWithDelay("Detailed Menu:", 50, 80);
                    delayedPrint("1. View Truck Details\n", 1);
                    delayedPrint("2. View Garbage Collected by Each Truck\n", 1);
                    delayedPrint("3. View Petrol Used by Each Truck\n", 1);
                    delayedPrint("4. View Accessories Taken by Workers of Each Truck\n", 1);
                    delayedPrint("5. Exit\n", 1);
                    delayedPrint("\nEnter your choice: ", 1);

                    int detailedChoice;
                    cin >> detailedChoice;

                    if (detailedChoice == 1)
                    {
                        displayTruckDetails(trucks);
                    }
                    else if (detailedChoice == 2)
                    {
                        displayGarbageCollected(trucks);
                    }
                    else if (detailedChoice == 3)
                    {
                        displayPetrolUsed(trucks);
                    }
                    else if (detailedChoice == 4)
                    {
                        displayAccessories(trucks);
                    }
                    else if (detailedChoice == 5)
                    {
                        break;
                    }
                    else
                    {
                        delayedPrint("\nInvalid choice. Please try again.\n", 1);
                    }
                }
            }
        }
        else if (choice == 3)
        {

          cout<<"\nServicing bins at all locations...\n", 50;
            for (int i = 0; i < trucks.size(); ++i) {
               cout<<"\nServicing bins by truck at " + trucks[i].location + "...\n", 50;
                this_thread::sleep_for(milliseconds(2000));
                // Simulate servicing process
                trucks[i].servicedBins.push_back(trucks[i].location);
                int randomGarbage = rand() % 50 + 1; // Random amount of garbage collected
                int randomPetrol = rand() % 10 + 1;  // Random amount of petrol used
                trucks[i].garbageCollected += randomGarbage;
                trucks[i].petrolUsed += randomPetrol;

               cout<<"Bin serviced at " + trucks[i].location + "\n", 50;
               cout<<"Garbage Collected: " + to_string(randomGarbage) + " kg\n", 50;
                cout<<"Petrol Used: " + to_string(randomPetrol) + " liters\n", 50;
                // Set all bins to empty after servicing
                for (int j = 0; j < 8; ++j) {
                    trucks[i].currentBinStatus[j] = 0;
                }
            }
            cout<<"All bins serviced.\n", 50;


        }
        else if (choice == 4)
        {
             displayAllBinStatus(trucks);

        }
        else if (choice == 5)
        {
            delayedPrint("\nAvailable colonies to choose from:\n", 1);
            for (int i = 0; i < numNodes; ++i)
            {
                delayedPrint(to_string(i) + ": " + places[i] + "\n", 1);
            }

            int binLocation;
            delayedPrint("\nEnter the location of the full bin (0 to " + to_string(numNodes - 1) + "): ", 1);
            cin >> binLocation;

            if (binLocation < 0 || binLocation >= numNodes)
            {
                cout << "Invalid location selected.\n";
                continue;
            }

            findNearestTruck(binLocation, places, trucks, city.getAdjacencyMatrix());
      }
        else if (choice == 6)
        {
            displayDailyTotals(trucks);

        }
        else if (choice == 7)
        {
            delayedPrint("\nThank You!!\nHave a Nice Day!!\n", 1);
            break;
        }
        else
        {
            delayedPrint("\nInvalid choice. Please try again.\n", 1);
        }
    }
}
// Function to print a message with a delay and aesthetic framing
void printWithAestheticDelay(const string& message, int delayMs = 100, const string& border = "#", int screenWidth = 80)
{
    // ANSI Escape Codes for colors
    const string borderColor = "\033[38;5;33m"; // Extended Color 4 (a unique color for the border)
    const string textColor = "\033[38;5;11m";   // Extended Color 11 (Unique Yellow for text)
    const string resetColor = "\033[0m";         // Reset color to default

    // Calculate the space for centering the text
    int spacesBeforeText = (screenWidth - message.length()) / 2;

    // Print the top border in the unique color
    for (int i = 0; i < screenWidth; ++i)
    {
        cout << borderColor << border;  // Print the border in the unique color
    }
    cout << resetColor << endl;

    // Add a small space above the message
    cout << endl;

    // Print the message with a delay for each character (typing effect)
    for (int i = 0; i < spacesBeforeText; ++i)
    {
        cout << " "; // Add space for centering
    }

    // Print the message in Unique Yellow with a delay for each character
    cout << textColor;
    for (char c : message)
    {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delayMs)); // Adjustable delay
    }
    cout << resetColor << endl;

    // Add a small space below the message
    cout << endl;

    // Print the bottom border in the unique color
    for (int i = 0; i < screenWidth; ++i)
    {
        cout << borderColor << border;  // Print the border in the unique color
    }
    cout << resetColor << endl;
}

// Display the main menu for both programs
void displayMainMenu()
{
    printWithAestheticDelay("*** Main Menu ***", 50, "#", 80);
    delayedPrint("1. Garbage Collection Records\n", 1);
    delayedPrint("2. Garbage Truck Management\n", 1);
    delayedPrint("3. Exit\n", 1);
    delayedPrint("Enter your choice: ", 1);
}

// Waste Management System functions
struct WasteItem
{
    string name;
    string category;
    double weight;
    string wasteType;  // Make sure wasteType is not 'const'
};

void displayWasteMenu();
void addWasteItems(vector<WasteItem>& wasteItems);
void viewCategorizedWaste(const vector<WasteItem>& wasteItems);
void viewSummary(const vector<WasteItem>& wasteItems);
void saveWasteDataToFile(const vector<WasteItem>& wasteItems);
void loadWasteDataFromFile(vector<WasteItem>& wasteItems);
void sortAndDisplayWasteItems(vector<WasteItem>& wasteItems);
void predictWasteGenerationTrends();
void calculateRecyclingEfficiency();
void trackMicroplastics();
void behavioralAnalytics();
void wasteManagementGamification();
// Function to print a message with a delay between each character
void printWithDelay4(const string& message, int delayMs = 100, bool newlineAfter = true, bool lineByLine = false)
{
    string yellow = "\033[33m";  // Yellow color
    string green = "\033[32m";   // Green color
    string reset = "\033[0m";    // Reset to default color

    if (lineByLine)
    {
        // Print message line by line
        string line;
        for (char c : message)
        {
            if (c == '\n')
            {
                // Print the current line with delay, then clear the line
                for (char lc : line)
                {
                    if (lc == '=')
                    {
                        cout << yellow << lc;  // Print '=' in yellow
                    }
                    else
                    {
                        cout << green << lc;  // Print other text in green
                    }
                    cout.flush();
                    this_thread::sleep_for(chrono::milliseconds(delayMs)); // Delay per character
                }
                cout << reset << endl;
                line.clear();  // Reset line for the next segment
            }
            else
            {
                line += c;  // Add character to the current line
            }
        }
        // Print any remaining characters after the last newline
        if (!line.empty())
        {
            for (char lc : line)
            {
                if (lc == '=')
                {
                    cout << yellow << lc;  // Print '=' in yellow
                }
                else
                {
                    cout << green << lc;  // Print other text in green
                }
                cout.flush();
                this_thread::sleep_for(chrono::milliseconds(delayMs)); // Delay per character
            }
            if (newlineAfter)
            {
                cout << reset << endl;  // Ensure newline after the last part and reset color
            }
        }
    }
    else
    {
        // Print the entire message in one go
        for (char c : message)
        {
            if (c == '=')
            {
                cout << yellow << c;  // Print '=' in yellow
            }
            else
            {
                cout << green << c;  // Print other text in green
            }
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(delayMs)); // Delay per character
        }
        if (newlineAfter)
        {
            cout << reset << endl;  // Ensure newline after the message and reset color
        }
    }
}
// Function to read waste items from the file 'waste_items_data.txt'
void readWasteItemsFromFile(vector<WasteItem>& wasteItems, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    // Skip the header line (assuming it's present)
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        WasteItem item;

        // Read name, category, and weight from each line
        getline(ss, item.name, ',');
        getline(ss, item.category, ',');
        ss >> item.weight;

        // Add the item to the vector
        wasteItems.push_back(item);
    }
    file.close();
}

// Function to display the content of the file in a table-like format
void displayWasteItems(const vector<WasteItem>& wasteItems) {
    cout << "\nFile contents:\n";
    cout << "_________________________________________________________\n";
    cout << "| Name              | Category          | Weight (kg)   | \n";
    cout << "|___________________|___________________|_______________|\n";

    for (const auto& item : wasteItems) {
        cout << "| " << setw(18) << left << item.name
             << "| " << setw(18) << left << item.category
             << "| " << setw(13) << fixed << setprecision(1) << item.weight << " |\n";
    }
    cout << "|___________________|___________________|_______________|\n";
}

// Selection Sort Function to sort by Name
void selectionSortByName(vector<WasteItem>& wasteItems, bool ascending) {
    int n = wasteItems.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if ((ascending && wasteItems[j].name < wasteItems[minIndex].name) ||
                (!ascending && wasteItems[j].name > wasteItems[minIndex].name)) {
                minIndex = j;
            }
        }
        swap(wasteItems[i], wasteItems[minIndex]);
    }
}

// Insertion Sort Function to sort by Category or Weight
void insertionSortByCategoryOrWeight(vector<WasteItem>& wasteItems, bool ascending, bool byCategory) {
    int n = wasteItems.size();
    for (int i = 1; i < n; ++i) {
        WasteItem key = wasteItems[i];
        int j = i - 1;

        while (j >= 0 && ((ascending && (byCategory ? wasteItems[j].category > key.category : wasteItems[j].weight > key.weight)) ||
                          (!ascending && (byCategory ? wasteItems[j].category < key.category : wasteItems[j].weight < key.weight)))) {
            wasteItems[j + 1] = wasteItems[j];
            j--;
        }
        wasteItems[j + 1] = key;
    }
}

// Function to sort and display waste items
void sortAndDisplayWasteItems(vector<WasteItem>& wasteItems) {
    int sortChoice = 0;
    while (true) {
        cout << "\nChoose how to sort waste items:\n";
        cout << "1. Sort by Name\n";
        cout << "2. Sort by Category\n";
        cout << "3. Sort by Weight\n";
        cout << "4. Exit Sorting\n";
        cout << "Enter your choice: ";
        cin >> sortChoice;

        if (sortChoice == 4) {
            cout << "Exiting sorting..." << endl;
            break;
        }

        int orderChoice;
        cout << "\nChoose sorting order:\n";
        cout << "1. Ascending Order\n";
        cout << "2. Descending Order\n";
        cout << "Enter your choice: ";
        cin >> orderChoice;

        switch (sortChoice) {
            case 1:
                if (orderChoice == 1) {
                    selectionSortByName(wasteItems, true);  // Ascending order by name
                } else if (orderChoice == 2) {
                    selectionSortByName(wasteItems, false); // Descending order by name
                } else {
                    cout << "Invalid choice. Exiting sorting." << endl;
                    return;
                }
                break;
            case 2:
                if (orderChoice == 1) {
                    insertionSortByCategoryOrWeight(wasteItems, true, true);  // Ascending order by category
                } else if (orderChoice == 2) {
                    insertionSortByCategoryOrWeight(wasteItems, false, true); // Descending order by category
                } else {
                    cout << "Invalid choice. Exiting sorting." << endl;
                    return;
                }
                break;
            case 3:
                if (orderChoice == 1) {
                    insertionSortByCategoryOrWeight(wasteItems, true, false);  // Ascending order by weight
                } else if (orderChoice == 2) {
                    insertionSortByCategoryOrWeight(wasteItems, false, false); // Descending order by weight
                } else {
                    cout << "Invalid choice. Exiting sorting." << endl;
                    return;
                }
                break;
            default:
                cout << "Invalid choice. Exiting sorting." << endl;
                return;
        }

        // Display the sorted waste items
        cout << "\nSorted Waste Items:";
        displayWasteItems(wasteItems);
    }
}

// Function to read waste items from the file and sort them
void readAndSortWasteItems() {
    string filename = "waste_data.txt";  // Filename specified here
    vector<WasteItem> wasteItems;

    // Read waste items from file
    readWasteItemsFromFile(wasteItems, filename);

    // Sort and display the waste items
    sortAndDisplayWasteItems(wasteItems);
}


void displayWasteMenu()
{
    int choice;
    vector<WasteItem> wasteItems;  // Declare wasteItems vector here to be used by all functions

    do {
      string message = "\n==================== WASTE MANAGEMENT SYSTEM ====================";
    printWithDelay4(message, 100);  // Adjust the delay if needed

  string magenta = "\033[35m";  // Magenta color
    string reset = "\033[0m";     // Reset color

    cout << magenta << "Welcome to the Waste Management System!\n";
    cout << "Here you can manage all waste-related data.\n";
    cout << "Please choose an option from the menu.\n\n" << reset;
     // Print line by line with a shorter delay
        cout << "1. Add waste items" << endl;
        cout << "2. View categorized waste" << endl;
        cout << "3. View summary" << endl;
        cout << "4. Save waste data to file" << endl;
        cout << "5. Load waste data from file" << endl;
        cout << "6. Sort and display waste items" << endl;
        cout << "7. Predict waste generation trends" << endl;
        cout << "8. Calculate recycling efficiency" << endl;
        cout << "9. Track microplastics" << endl;
        cout << "10. Behavioral analytics" << endl;
        cout << "11. Waste management gamification" << endl;
        cout << "12. Exit\n";
        cout << "Enter your choice (1-12): ";
        cin >> choice;
        cin.ignore();  // To clear the input buffer

        switch (choice)
        {
            case 1:
                addWasteItems(wasteItems);
                break;
            case 2:
                viewCategorizedWaste(wasteItems);
                break;
            case 3:
                viewSummary(wasteItems);
                break;
            case 4:
                saveWasteDataToFile(wasteItems);
                break;
            case 5:
                loadWasteDataFromFile(wasteItems);
                break;
            case 6:
                 readAndSortWasteItems();
                break;
            case 7:
                predictWasteGenerationTrends();
                break;
            case 8:
                calculateRecyclingEfficiency();
                break;
            case 9:
                trackMicroplastics();
                break;
            case 10:
                behavioralAnalytics();
                break;
            case 11:
                wasteManagementGamification();
                break;
            case 12:
                cout << "Exiting the Waste Management System." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        cout << endl;
    } while (choice != 12);
}

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

void addWasteItems(vector<WasteItem>& wasteItems) {
    char continueChoice;
    do {
        WasteItem newItem;
        int categoryChoice;

        cout << "Enter waste item name: ";
        getline(cin, newItem.name);

        // Display category options
        string blue = "\033[34m";   // Blue color
        string reset = "\033[0m";   // Reset color

        cout << blue << "Select a category for the waste item:" << endl << reset;
        cout << "1. Plastic" << endl;
        cout << "2. Glass" << endl;
        cout << "3. Paper" << endl;
        cout << "4. Food" << endl;
        cout << "5. Vegetable" << endl;
        cout << "6. Electronics" << endl;
        cout << "7. Metal" << endl;
        cout << "8. Hazardous" << endl;
        cout << "Enter your choice (1-8): ";
        cin >> categoryChoice;
        cin.ignore();  // To clear the input buffer

        // Set the category based on the user's choice
        switch (categoryChoice) {
            case 1:
                newItem.category = "Plastic";
                break;
            case 2:
                newItem.category = "Glass";
                break;
            case 3:
                newItem.category = "Paper";
                break;
            case 4:
                newItem.category = "Food";
                break;
            case 5:
                newItem.category = "Vegetable";
                break;
            case 6:
                newItem.category = "Electronics";
                break;
            case 7:
                newItem.category = "Metal";
                break;
            case 8:
                newItem.category = "Hazardous";
                break;
            default:
                cout << "Invalid choice! Defaulting to 'Plastic'." << endl;
                newItem.category = "Plastic";
                break;
        }

        cout << "Enter weight of the item in kilograms: ";
        cin >> newItem.weight;
        cin.ignore();  // To clear any leftover input

        wasteItems.push_back(newItem);
        string green = "\033[32m";  // Green color
        cout << green << "Waste item added successfully!" << endl << reset;

        cout << "Do you want to add more waste items? (y/n): ";
        cin >> continueChoice;
        cin.ignore(); // To clear the input buffer
    } while (toLower(string(1, continueChoice)) == "y");

    cout << "Finished adding waste items." << endl;

    // Display added items for verification
    cout << "Added Waste Items:" << endl;
    for (const auto& item : wasteItems) {
        cout << "Name: " << item.name << ", Category: " << item.category << ", Weight: " << item.weight << " kg" << endl;
    }
}

// Function to convert string to lowercase
string toLowerCase2(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Function to display categorized waste items
void viewCategorizedWaste(const vector<WasteItem>& wasteItems) {
    int categoryChoice;
    string category;
    unordered_map<string, string> categoryTypes = {
        {"plastic", "Non-Biodegradable, Recyclable"},
        {"glass", "Non-Biodegradable, Recyclable"},
        {"paper", "Biodegradable, Recyclable"},
        {"food", "Biodegradable, Non-Recyclable"},
        {"vegetable", "Biodegradable, Non-Recyclable"},
        {"electronics", "Non-Biodegradable, Non-Recyclable"},
        {"metal", "Non-Biodegradable, Recyclable"},
        {"hazardous", "Non-Biodegradable, Non-Recyclable"}
    };

    do {
        cout << "Choose the type of waste category to view:" << endl;
        cout << "1. View a specific category (e.g., Paper, Plastic, Food)" << endl;
        cout << "2. View waste by type (Biodegradable, Non-Biodegradable, Recyclable, Non-Recyclable)" << endl;
        cout << "3. View all categories" << endl;
        cout << "4. Exit category view" << endl;
        cout << "Enter your choice: ";
        cin >> categoryChoice;
        cin.ignore(); // Ignore newline after reading the choice

        switch (categoryChoice) {
            case 1: { // View a specific category
                cout << "Enter category to view (e.g., Plastic, Glass, Paper, Food, etc.): ";
                getline(cin, category);
                category = toLowerCase2(category);  // Convert to lowercase for case-insensitive matching

                bool found = false;
                cout << "Items in category " << category << ":" << endl;
                cout << "_________________________________________________________\n";
                cout << "| Name              | Category          | Weight (kg)   |\n";
                cout << "|___________________|___________________|_______________|\n";
                for (const auto& item : wasteItems) {
                    if (toLowerCase2(item.category) == category) {
                        cout << "| " << setw(18) << left << item.name
                             << "| " << setw(18) << left << item.category
                             << "| " << setw(13) << fixed << setprecision(1) << item.weight << " |\n";
                        found = true;
                    }
                }
                cout << "|___________________|___________________|_______________|\n";

                if (!found) {
                    cout << "No items found for category: " << category << endl;
                }

                break;
            }

            case 2: { // View waste by type (Biodegradable, Non-Biodegradable, Recyclable, Non-Recyclable)
                int typeChoice;

                cout << "Choose waste type to view:" << endl;
                cout << "1. Biodegradable" << endl;
                cout << "2. Non-Biodegradable" << endl;
                cout << "3. Recyclable" << endl;
                cout << "4. Non-Recyclable" << endl;

                cout << "Enter your choice: ";
                cin >> typeChoice;
                cin.ignore(); // Ignore newline

                bool found = false;
                switch (typeChoice) {
                    case 1: { // Biodegradable
                        cout << "Items in Biodegradable category:" << endl;
                        cout << "_________________________________________________________\n";
                        cout << "| Name              | Category          | Weight (kg)   |\n";
                        cout << "|___________________|___________________|_______________|\n";
                        for (const auto& item : wasteItems) {
                            if (categoryTypes[toLowerCase2(item.category)].find("Biodegradable") != string::npos) {
                                cout << "| " << setw(18) << left << item.name
                                     << "| " << setw(18) << left << item.category
                                     << "| " << setw(13) << fixed << setprecision(1) << item.weight << " |\n";
                                found = true;
                            }
                        }
                        cout << "|___________________|___________________|_______________|\n";
                        break;
                    }
                    case 2: { // Non-Biodegradable
                        cout << "Items in Non-Biodegradable category:" << endl;
                        cout << "_________________________________________________________\n";
                        cout << "| Name              | Category          | Weight (kg)   |\n";
                        cout << "|___________________|___________________|_______________|\n";
                        for (const auto& item : wasteItems) {
                            if (categoryTypes[toLowerCase2(item.category)].find("Non-Biodegradable") != string::npos) {
                                cout << "| " << setw(18) << left << item.name
                                     << "| " << setw(18) << left << item.category
                                     << "| " << setw(13) << fixed << setprecision(1) << item.weight << " |\n";
                                found = true;
                            }
                        }
                        cout << "|___________________|___________________|_______________|\n";
                        break;
                    }
                    case 3: { // Recyclable
                        cout << "Items in Recyclable category:" << endl;
                        cout << "_________________________________________________________\n";
                        cout << "| Name              | Category          | Weight (kg)   |\n";
                        cout << "|___________________|___________________|_______________|\n";
                        for (const auto& item : wasteItems) {
                            if (categoryTypes[toLowerCase2(item.category)].find("Recyclable") != string::npos) {
                                cout << "| " << setw(18) << left << item.name
                                     << "| " << setw(18) << left << item.category
                                     << "| " << setw(13) << fixed << setprecision(1) << item.weight << " |\n";
                                found = true;
                            }
                        }
                        cout << "|___________________|___________________|_______________|\n";
                        break;
                    }
                    case 4: { // Non-Recyclable
                        cout << "Items in Non-Recyclable category:" << endl;
                        cout << "_________________________________________________________\n";
                        cout << "| Name              | Category          | Weight (kg)   |\n";
                        cout << "|___________________|___________________|_______________|\n";
                        for (const auto& item : wasteItems) {
                            if (categoryTypes[toLowerCase2(item.category)].find("Non-Recyclable") != string::npos) {
                                cout << "| " << setw(18) << left << item.name
                                     << "| " << setw(18) << left << item.category
                                     << "| " << setw(13) << fixed << setprecision(1) << item.weight << " |\n";
                                found = true;
                            }
                        }
                        cout << "|___________________|___________________|_______________|\n";
                        break;
                    }
                    default:
                        cout << "Invalid choice. Exiting category view." << endl;
                        return;
                }

                if (!found) {
                    cout << "No items found for the selected type." << endl;
                }
                break;
            }

            case 3: { // View all categories
                if (wasteItems.empty()) {
                    cout << "No items to display." << endl;
                } else {
                    cout << "Items in all categories:" << endl;
                    cout << "_________________________________________________________\n";
                    cout << "| Name              | Category          | Weight (kg)   |\n";
                    cout << "|___________________|___________________|_______________|\n";
                    for (const auto& item : wasteItems) {
                        cout << "| " << setw(18) << left << item.name
                             << "| " << setw(18) << left << item.category
                             << "| " << setw(13) << fixed << setprecision(1) << item.weight << " |\n";
                    }
                    cout << "|___________________|___________________|_______________|\n";
                }
                break;
            }

            case 4:
                cout << "Exiting category view..." << endl;
                return;

            default:
                cout << "Invalid choice. Exiting category view." << endl;
                return;
        }
    } while (true);
}

void viewSummary(const vector<WasteItem>& wasteItems)
{
    int summaryChoice;
    double totalWeight = 0;
    unordered_map<string, double> categoryWeight;
     const string blue = "\033[34m";  // Blue color
    const string reset = "\033[0m";  // Reset color to default
    cout <<blue<< "Choose the type of waste summary you want to view:" << endl<<reset;
    cout << "1. Total number of waste items and total weight" << endl;
    cout << "2. Breakdown of waste by category" << endl;
    cout << "3. Additional statistics (average weight per item)" << endl;
    cout << "4. Exit summary view" << endl;
    cout << "Enter your choice: ";
    cin >> summaryChoice;

    switch (summaryChoice)
    {
        case 1:
        { // Total number of items and total weight
            cout << "Waste Summary:" << endl;
            for (const auto& item : wasteItems)
            {
                totalWeight += item.weight;
            }
            cout << "Total number of waste items: " << wasteItems.size() << endl;
            cout << "Total weight of waste: " << totalWeight << " kg" << endl;
            break;
        }

        case 2:
        { // Breakdown by category
            cout << "Waste Breakdown by Category:" << endl;
            for (const auto& item : wasteItems)
            {
                categoryWeight[item.category] += item.weight;
            }

            for (const auto& category : categoryWeight)
            {
                cout << "Category: " << category.first
                     << ", Total weight: " << category.second << " kg" << endl;
            }
            break;
        }

        case 3:
        { // Additional statistics (average weight per item)
            double averageWeight = 0;
            if (!wasteItems.empty())
            {
                for (const auto& item : wasteItems)
                {
                    totalWeight += item.weight;
                }
                averageWeight = totalWeight / wasteItems.size();
            }
            cout << "Average weight per waste item: " << averageWeight << " kg" << endl;
            break;
        }

        case 4:
            cout << "Exiting summary view..." << endl;
            return;

        default:
            cout << "Invalid choice. Exiting summary view." << endl;
            return;
    }
}

void saveWasteDataToFile(const vector<WasteItem>& wasteItems)
{
    int fileChoice;
     const string blue = "\033[34m";  // Blue color
    const string reset = "\033[0m";  // Reset color to default
    cout <<blue<< "Choose an option for saving waste data to a file:" << endl<<reset;
    cout << "1. Save to default file (waste_data.txt)" << endl;
    cout << "2. Exit saving" << endl;
    cout << "Enter your choice: ";
    cin >> fileChoice;

    switch (fileChoice) {
        case 1: {
            ofstream outFile("waste_data.txt", ios::app); // Open file in append mode
            if (outFile.is_open()) {
                for (const auto& item : wasteItems) {
                    outFile << item.name << ", " << item.category << ", " << item.weight << endl;
                }
                outFile.close();
                cout << "Data saved to default file (waste_data.txt) successfully!" << endl;
            } else {
                cout << "Error saving data to default file." << endl;
            }
            break;
        }
        case 3:
            {
            string fileName;
            int fileFormatChoice;
            int fileModeChoice;

            cout << "\nYou chose to save to a custom file." << endl;
            cout << "Enter the file name: ";
            cin >> fileName;

            cout << "Choose the file format:" << endl;
            cout << "1. CSV (Comma Separated)" << endl;
            cout << "2. TSV (Tab Separated)" << endl;
            cout << "Enter your choice: ";
            cin >> fileFormatChoice;

            cout << "Choose the file mode:" << endl;
            cout << "1. Overwrite the file" << endl;
            cout << "2. Append to the file" << endl;
            cout << "Enter your choice: ";
            cin >> fileModeChoice;

            ofstream outFile;

            // Determine the file mode (overwrite or append)
            switch (fileModeChoice)
            {
                case 1:
                    outFile.open(fileName, ios::trunc); // Overwrite the file
                    break;
                case 2:
                    outFile.open(fileName, ios::app); // Append to the file
                    break;
                default:
                    cout << "Invalid file mode choice. Exiting saving." << endl;
                    return;
            }

            if (outFile.is_open())
            {
                switch (fileFormatChoice)
                {
                    case 1: { // CSV format
                        for (const auto& item : wasteItems)
                        {
                            outFile << item.name << ", " << item.category << ", " << item.weight << endl;
                        }
                        cout << "Data saved to " << fileName << " (CSV format) successfully!" << endl;
                        break;
                    }
                    case 2: { // TSV format
                        for (const auto& item : wasteItems)
                        {
                            outFile << item.name << "\t" << item.category << "\t" << item.weight << endl;
                        }
                        cout << "Data saved to " << fileName << " (TSV format) successfully!" << endl;
                        break;
                    }
                    default:
                        cout << "Invalid file format choice. Exiting saving." << endl;
                        return;
                }
                outFile.close();
            }
            else
            {
                cout << "Error saving data to custom file: " << fileName << endl;
            }
            break;
        }

        case 2:
            cout << "Exiting saving process..." << endl;
            return;

        default:
            cout << "Invalid choice. Exiting saving process." << endl;
            return;
    }
}
void loadWasteDataFromFile(vector<WasteItem>& wasteItems)
{
    int fileChoice;
     const string blue = "\033[34m";  // Blue color
    const string reset = "\033[0m";  // Reset color to default

    cout<<blue << "Choose an option for loading waste data from a file:" << endl<<reset;
    cout << "1. Load from default file (waste_data.txt)" << endl;
    cout << "2. Load from a custom file" << endl;
    cout << "3. Exit loading" << endl;
    cout << "Enter your choice: ";
    cin >> fileChoice;

    switch (fileChoice)
    {
        case 1:
        {
            ifstream inFile("waste_data.txt");
            if (inFile.is_open())
            {
                wasteItems.clear();
                string name, category;
                double weight;
                while (getline(inFile, name, ',') && getline(inFile, category, ',') && inFile >> weight)
                {
                    inFile.ignore(); // Ignore the newline character
                    wasteItems.push_back({name, category, weight});
                }
                inFile.close();
                cout << "Data loaded from default file (waste_data.txt) successfully!" << endl;
            }
            else
            {
                cout << "Error loading data from default file." << endl;
            }
            break;
        }

        case 2:
        {
            string fileName;
            int fileFormatChoice;

            cout << "\nYou chose to load from a custom file." << endl;
            cout << "Enter the file name: ";
            cin >> fileName;

            cout << "Choose the file format:" << endl;
            cout << "1. CSV (Comma Separated)" << endl;
            cout << "2. TSV (Tab Separated)" << endl;
            cout << "Enter your choice: ";
            cin >> fileFormatChoice;

            ifstream inFile(fileName);
            if (inFile.is_open())
            {
                wasteItems.clear();
                string name, category;
                double weight;

                switch (fileFormatChoice)
                {
                    case 1: { // CSV format
                        while (getline(inFile, name, ',') && getline(inFile, category, ',') && inFile >> weight)
                        {
                            inFile.ignore(); // Ignore the newline character
                            wasteItems.push_back({name, category, weight});
                        }
                        cout << "Data loaded from " << fileName << " (CSV format) successfully!" << endl;
                        break;
                    }
                    case 2:
                    { // TSV format
                        while (getline(inFile, name, '\t') && getline(inFile, category, '\t') && inFile >> weight)
                        {
                            inFile.ignore(); // Ignore the newline character
                            wasteItems.push_back({name, category, weight});
                        }
                        cout << "Data loaded from " << fileName << " (TSV format) successfully!" << endl;
                        break;
                    }
                    default:
                        cout << "Invalid file format choice. Exiting loading." << endl;
                        return;
                }
                inFile.close();
            }
            else
            {
                cout << "Error loading data from custom file: " << fileName << endl;
            }
            break;
        }

        case 3:
            cout << "Exiting loading process..." << endl;
            return;

        default:
            cout << "Invalid choice. Exiting loading process." << endl;
            return;
    }
}

void predictWasteGenerationTrends()
{
    int trendChoice;
     const string blue = "\033[34m";  // Blue color
    const string reset = "\033[0m";  // Reset color to default
    const string green = "\033[32m";
    cout <<blue<< "Predicting waste generation trends..." << endl<<reset;
    cout <<green<< "Choose the type of waste generation trend prediction you want to explore:" << endl<<reset;
    cout << "1. Waste Generation by Waste Type" << endl;
    cout << "2. Waste Generation Based on Population Growth" << endl;
    cout << "3. Impact of Environmental Policies on Waste Generation" << endl;
    cout << "4. Forecast Waste Generation Over Time" << endl;
    cout << "5. Exit Trend Prediction" << endl;
    cout << "Enter your choice: ";
    cin >> trendChoice;

    switch (trendChoice)
    {
        case 1: {
            srand(time(0)); // Initialize random number generator with current time

    int wasteTypeChoice;
    cout << "\nYou chose 'Waste Generation by Waste Type'." << endl;
    cout << "Choose a waste type to analyze generation trends:" << endl;
    cout << "1. Paper" << endl;
    cout << "2. Plastic" << endl;
    cout << "3. Food Waste" << endl;
    cout << "4. Electronic Waste" << endl;
    cout << "5. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> wasteTypeChoice;

    switch (wasteTypeChoice)
    {
        case 1:
            {
                // Generate random increase percentage between 2% and 4% for paper waste
                int randomPercentage = rand() % 3 + 2; // Random value between 2 and 4
                cout << "Paper waste generation has been steadily increasing by "
                     << randomPercentage << "% annually due to higher consumption." << endl;
            }
            break;
        case 2:
            {
                // Generate random increase percentage between 4% and 6% for plastic waste
                int randomPercentage = rand() % 3 + 4; // Random value between 4 and 6
                cout << "Plastic waste generation is projected to increase by "
                     << randomPercentage << "% annually due to the continued reliance on single-use plastics." << endl;
            }
            break;
        case 3:
            {
                // Generate random increase percentage between 3% and 5% for food waste
                int randomPercentage = rand() % 3 + 3; // Random value between 3 and 5
                cout << "Food waste generation is expected to rise by "
                     << randomPercentage << "% annually, driven by increased food production and consumption." << endl;
            }
            break;
        case 4:
            {
                // Generate random increase percentage between 7% and 9% for electronic waste
                int randomPercentage = rand() % 3 + 7; // Random value between 7 and 9
                cout << "Electronic waste is growing rapidly, with an estimated "
                     << randomPercentage << "% increase per year as more devices are discarded." << endl;
            }
            break;
        case 5:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }

            break;
        }

        case 2:
        {
           srand(time(0)); // Initialize random number generator with current time

    int populationGrowthChoice;
    cout << "\nYou chose 'Waste Generation Based on Population Growth'." << endl;
    cout << "Choose a region to predict waste generation based on population growth:" << endl;
    cout << "1. Global Trend" << endl;
    cout << "2. North America" << endl;
    cout << "3. Asia" << endl;
    cout << "4. Africa" << endl;
    cout << "5. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> populationGrowthChoice;

    switch (populationGrowthChoice)
    {
        case 1:
            {
                // Generate random increase percentage between 2% and 3% for global trend
                int randomPercentage = rand() % 2 + 2; // Random value between 2 and 3
                cout << "Globally, waste generation is expected to increase by "
                     << randomPercentage << "% annually due to population growth." << endl;
            }
            break;
        case 2:
            {
                // Generate random increase percentage between 2% and 4% for North America
                int randomPercentage = rand() % 3 + 2; // Random value between 2 and 4
                cout << "In North America, waste generation may increase by "
                     << randomPercentage << "% annually, driven by population and consumer habits." << endl;
            }
            break;
        case 3:
            {
                // Generate random increase percentage between 3% and 5% for Asia
                int randomPercentage = rand() % 3 + 3; // Random value between 3 and 5
                cout << "Asia's waste generation will rise by "
                     << randomPercentage << "% annually as urbanization and consumption increase." << endl;
            }
            break;
        case 4:
            {
                // Generate random increase percentage between 4% and 6% for Africa
                int randomPercentage = rand() % 3 + 4; // Random value between 4 and 6
                cout << "In Africa, waste generation is projected to increase by "
                     << randomPercentage << "% annually due to rapid population growth and urbanization." << endl;
            }
            break;
        case 5:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }
            break;
        }

        case 3:
    {
          srand(time(0)); // Initialize random number generator with current time

    int policyImpactChoice;
    cout << "\nYou chose 'Impact of Environmental Policies on Waste Generation'." << endl;
    cout << "Choose a policy to analyze its impact on waste generation trends:" << endl;
    cout << "1. Plastic Ban" << endl;
    cout << "2. Recycling Incentives" << endl;
    cout << "3. Waste Reduction Targets" << endl;
    cout << "4. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> policyImpactChoice;

    switch (policyImpactChoice)
    {
        case 1:
            {
                // Generate random reduction percentage between 5% and 15% for plastic ban
                int randomPercentage = rand() % 11 + 5; // Random value between 5 and 15
                cout << "The plastic ban is expected to reduce plastic waste generation by "
                     << randomPercentage << "% within the next 5 years." << endl;
            }
            break;
        case 2:
            {
                // Generate random reduction percentage between 3% and 7% for recycling incentives
                int randomPercentage = rand() % 5 + 3; // Random value between 3 and 7
                cout << "Recycling incentives are projected to increase recycling rates, reducing waste generation by "
                     << randomPercentage << "% annually." << endl;
            }
            break;
        case 3:
            {
                // Generate random reduction percentage between 2% and 5% for waste reduction targets
                int randomPercentage = rand() % 4 + 2; // Random value between 2 and 5
                cout << "National waste reduction targets aim to reduce overall waste generation by "
                     << randomPercentage << "% annually." << endl;
            }
            break;
        case 4:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }
            break;
        }

        case 4:
        {
       srand(time(0)); // Initialize random number generator with current time

    int forecastChoice;
    cout << "\nYou chose 'Forecast Waste Generation Over Time'." << endl;
    cout << "Choose a time period to forecast waste generation:" << endl;
    cout << "1. Next 5 Years" << endl;
    cout << "2. Next 10 Years" << endl;
    cout << "3. Next 20 Years" << endl;
    cout << "4. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> forecastChoice;

    switch (forecastChoice)
    {
        case 1:
            {
                // Generate random percentage between 10% and 15% for 5 years
                int randomPercentage = rand() % 6 + 10; // Random value between 10 and 15
                cout << "In the next 5 years, waste generation is projected to increase by "
                     << randomPercentage << "%, mainly due to population growth." << endl;
            }
            break;
        case 2:
            {
                // Generate random percentage between 20% and 30% for 10 years
                int randomPercentage = rand() % 11 + 20; // Random value between 20 and 30
                cout << "In the next 10 years, waste generation may rise by "
                     << randomPercentage << "%, considering the current trends in consumption and population growth." << endl;
            }
            break;
        case 3:
            {
                // Generate random percentage between 35% and 45% for 20 years
                int randomPercentage = rand() % 11 + 35; // Random value between 35 and 45
                cout << "In the next 20 years, waste generation is expected to increase by "
                     << randomPercentage << "%, with significant rises in e-waste and plastic waste." << endl;
            }
            break;
        case 4:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }

            break;
        }

        case 5:
            cout << "Exiting trend prediction. See you next time!" << endl;
            break;

        default:
            cout << "Invalid choice. Exiting trend prediction." << endl;
    }
}

void calculateRecyclingEfficiency()
{
    int efficiencyChoice;
     const string blue = "\033[34m";  // Blue color
    const string reset = "\033[0m";  // Reset color to default
    const string green = "\033[32m";
    cout <<blue<< "Calculating recycling efficiency..." << endl<<reset;
    cout <<green<< "Choose the type of recycling efficiency analysis you want to perform:" << endl<<green;
    cout << "1. Recycling Efficiency by Waste Type" << endl;
    cout << "2. Recycling Efficiency Over Time" << endl;
    cout << "3. Recycling Efficiency by Region" << endl;
    cout << "4. Improvement in Recycling Processes" << endl;
    cout << "5. Exit Recycling Efficiency Calculation" << endl;
    cout << "Enter your choice: ";
    cin >> efficiencyChoice;

    switch (efficiencyChoice)
{
        case 1:
    {
srand(time(0)); // Initialize random number generator with current time

    int wasteTypeChoice;
    cout << "\nYou chose 'Recycling Efficiency by Waste Type'." << endl;
    cout << "Choose a waste type to analyze recycling efficiency:" << endl;
    cout << "1. Paper" << endl;
    cout << "2. Plastic" << endl;
    cout << "3. Metal" << endl;
    cout << "4. Glass" << endl;
    cout << "5. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> wasteTypeChoice;

    switch (wasteTypeChoice)
    {
        case 1:
            {
                // Generate random percentage between 70% and 85% for paper recycling efficiency
                int randomPercentage = rand() % 16 + 70; // Random value between 70 and 85
                cout << "Paper recycling efficiency: "
                     << randomPercentage << "% of paper is recycled, with a recovery rate of "
                     << (randomPercentage - 5) << "%!" << endl;
            }
            break;
        case 2:
            {
                // Generate random percentage between 20% and 35% for plastic recycling efficiency
                int randomPercentage = rand() % 16 + 20; // Random value between 20 and 35
                cout << "Plastic recycling efficiency: Only "
                     << randomPercentage << "% of plastic waste is recycled efficiently!" << endl;
            }
            break;
        case 3:
            {
                // Generate random percentage between 85% and 95% for metal recycling efficiency
                int randomPercentage = rand() % 11 + 85; // Random value between 85 and 95
                cout << "Metal recycling efficiency: "
                     << randomPercentage << "% of metal waste is recycled due to high recovery rates!" << endl;
            }
            break;
        case 4:
            {
                // Generate random percentage between 80% and 90% for glass recycling efficiency
                int randomPercentage = rand() % 11 + 80; // Random value between 80 and 90
                cout << "Glass recycling efficiency: "
                     << randomPercentage << "% of glass is recycled with a near 100% recovery rate!" << endl;
            }
            break;
        case 5:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }

            break;
        }

        case 2:
        {
srand(time(0)); // Initialize random number generator with current time

    int timeChoice;
    cout << "\nYou chose 'Recycling Efficiency Over Time'." << endl;
    cout << "Choose the time period to analyze recycling efficiency:" << endl;
    cout << "1. Monthly Efficiency" << endl;
    cout << "2. Quarterly Efficiency" << endl;
    cout << "3. Annual Efficiency" << endl;
    cout << "4. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> timeChoice;

    switch (timeChoice)
    {
        case 1:
            {
                // Generate random improvement percentage between 3% and 7% for monthly efficiency
                int randomPercentage = rand() % 5 + 3; // Random value between 3 and 7
                cout << "Monthly efficiency: Recycling efficiency has improved by "
                     << randomPercentage << "% in the last month!" << endl;
            }
            break;
        case 2:
            {
                // Generate random improvement percentage between 2% and 5% for quarterly efficiency
                int randomPercentage = rand() % 4 + 2; // Random value between 2 and 5
                cout << "Quarterly efficiency: Recycling efficiency has increased by "
                     << randomPercentage << "% in the past quarter." << endl;
            }
            break;
        case 3:
            {
                // Generate random improvement percentage between 8% and 12% for annual efficiency
                int randomPercentage = rand() % 5 + 8; // Random value between 8 and 12
                cout << "Annual efficiency: Over the last year, recycling efficiency has improved by "
                     << randomPercentage << "%." << endl;
            }
            break;
        case 4:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }
            break;
        }

        case 3:
        {
            srand(time(0)); // Initialize random number generator with current time

    int regionChoice;
    cout << "\nYou chose 'Recycling Efficiency by Region'." << endl;
    cout << "Choose a region to analyze recycling efficiency:" << endl;
    cout << "1. North America" << endl;
    cout << "2. Europe" << endl;
    cout << "3. Asia" << endl;
    cout << "4. Africa" << endl;
    cout << "5. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> regionChoice;

    switch (regionChoice)
    {
        case 1:
            {
                // Generate random percentage between 50% and 70% for North America
                int randomPercentage = rand() % 21 + 50; // Random value between 50 and 70
                cout << "North America recycling efficiency: "
                     << randomPercentage << "% of recyclable waste is processed efficiently." << endl;
            }
            break;
        case 2:
            {
                // Generate random percentage between 70% and 90% for Europe
                int randomPercentage = rand() % 21 + 70; // Random value between 70 and 90
                cout << "Europe recycling efficiency: "
                     << randomPercentage << "% of recyclable waste is processed efficiently." << endl;
            }
            break;
        case 3:
            {
                // Generate random percentage between 60% and 80% for Asia
                int randomPercentage = rand() % 21 + 60; // Random value between 60 and 80
                cout << "Asia recycling efficiency: Recycling rates vary, but some countries recycle up to "
                     << randomPercentage << "% of waste." << endl;
            }
            break;
        case 4:
            {
                // Generate random percentage between 10% and 30% for Africa
                int randomPercentage = rand() % 21 + 10; // Random value between 10 and 30
                cout << "Africa recycling efficiency: Recycling is low, with less than "
                     << randomPercentage << "% of waste being processed." << endl;
            }
            break;
        case 5:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }
            break;
        }

        case 4:
        {
              srand(time(0)); // Initialize random number generator with current time

    int improvementChoice;
    cout << "\nYou chose 'Improvement in Recycling Processes'." << endl;
    cout << "Choose a process to analyze improvements:" << endl;
    cout << "1. Sorting Technology Improvements" << endl;
    cout << "2. Increased Public Awareness" << endl;
    cout << "3. Advanced Recycling Facilities" << endl;
    cout << "4. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> improvementChoice;

    switch (improvementChoice)
    {
        case 1:
            {
                // Generate random percentage between 15% and 25% for sorting technology improvement
                int randomPercentage = rand() % 11 + 15; // Random value between 15 and 25
                cout << "Sorting technology: Automation and AI-based sorting systems have increased recycling efficiency by "
                     << randomPercentage << "%!" << endl;
            }
            break;
        case 2:
            {
                // Generate random percentage between 10% and 20% for increased public awareness
                int randomPercentage = rand() % 11 + 10; // Random value between 10 and 20
                cout << "Public awareness: Awareness campaigns have led to a "
                     << randomPercentage << "% increase in recycling participation." << endl;
            }
            break;
        case 3:
            {
                // Generate random percentage between 25% and 35% for advanced recycling facilities
                int randomPercentage = rand() % 11 + 25; // Random value between 25 and 35
                cout << "Recycling facilities: New facilities with advanced machinery are processing up to "
                     << randomPercentage << "% more materials." << endl;
            }
            break;
        case 4:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }
            break;
        }

        case 5:
            cout << "Exiting recycling efficiency calculation. See you next time!" << endl;
            break;

        default:
            cout << "Invalid choice. Exiting recycling efficiency calculation." << endl;
    }
}

void trackMicroplastics()
{
    int microplasticChoice;
     const string blue = "\033[34m";  // Blue color
    const string reset = "\033[0m";  // Reset color to default
    const string green = "\033[32m";
    cout <<blue<< "Tracking microplastics..." << endl<<reset;
    cout <<green<< "Choose the type of microplastic tracking you want to perform:" << endl<<reset;
    cout << "1. Microplastic Sources" << endl;
    cout << "2. Microplastic Contamination Levels" << endl;
    cout << "3. Microplastic Reduction Efforts" << endl;
    cout << "4. Types of Microplastics" << endl;
    cout << "5. Exit Microplastic Tracking" << endl;
    cout << "Enter your choice: ";
    cin >> microplasticChoice;

    switch (microplasticChoice)
    {
        case 1:
        {
            int sourceChoice;
            cout << "\nYou chose 'Microplastic Sources'." << endl;
            cout << "Choose an option to analyze:" << endl;
            cout << "1. Plastic Waste in Oceans" << endl;
            cout << "2. Industrial Microplastic Release" << endl;
            cout << "3. Household Microplastic Pollution" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            cin >> sourceChoice;

            switch (sourceChoice)
            {
                case 1:
                    cout << "Analyzing ocean plastic waste: 8 million tons of plastic enter oceans annually!" << endl;
                    break;
                case 2:
                    cout << "Industrial release: Microplastics from manufacturing processes contaminate rivers and lakes." << endl;
                    break;
                case 3:
                    cout << "Household pollution: Everyday activities like laundry contribute microplastics to wastewater." << endl;
                    break;
                case 4:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Going back to the main menu." << endl;
            }
            break;
        }

        case 2:
    {
            int contaminationChoice;
            cout << "\nYou chose 'Microplastic Contamination Levels'." << endl;
            cout << "Choose an option to analyze:" << endl;
            cout << "1. Ocean Contamination Levels" << endl;
            cout << "2. Freshwater Contamination Levels" << endl;
            cout << "3. Airborne Microplastics" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            cin >> contaminationChoice;

            switch (contaminationChoice)
            {
                case 1:
                    cout << "Ocean contamination: Microplastics have been detected in all major ocean regions." << endl;
                    break;
                case 2:
                    cout << "Freshwater contamination: Lakes and rivers are increasingly contaminated with microplastics." << endl;
                    break;
                case 3:
                    cout << "Airborne microplastics: Studies show that microplastics are in the air we breathe in urban areas." << endl;
                    break;
                case 4:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Going back to the main menu." << endl;
            }
            break;
        }

        case 3:
    {
            int reductionChoice;
            cout << "\nYou chose 'Microplastic Reduction Efforts'." << endl;
            cout << "Choose an option to analyze:" << endl;
            cout << "1. Bans on Single-Use Plastics" << endl;
            cout << "2. Filtration Technologies" << endl;
            cout << "3. Public Awareness Campaigns" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            cin >> reductionChoice;

            switch (reductionChoice)
            {
                case 1:
                    cout << "Single-use plastic bans: Countries and cities are banning single-use plastics to reduce waste." << endl;
                    break;
                case 2:
                    cout << "Filtration technologies: Innovations like microfiber filters in washing machines reduce microplastic release." << endl;
                    break;
                case 3:
                    cout << "Awareness campaigns: Increased awareness is driving changes in consumer behavior to reduce plastic waste." << endl;
                    break;
                case 4:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Going back to the main menu." << endl;
            }
            break;
        }

        case 4:
        {
            int typesChoice;
            cout << "\nYou chose 'Types of Microplastics'." << endl;
            cout << "Choose an option to analyze:" << endl;
            cout << "1. Primary Microplastics" << endl;
            cout << "2. Secondary Microplastics" << endl;
            cout << "3. Microplastics from Synthetic Fabrics" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            cin >> typesChoice;

            switch (typesChoice)
            {
                case 1:
                    cout << "Primary microplastics: These are small plastic particles intentionally produced, like in personal care products." << endl;
                    break;
                case 2:
                    cout << "Secondary microplastics: These are fragments of larger plastic items that break down over time in the environment." << endl;
                    break;
                case 3:
                    cout << "Microplastics from synthetic fabrics: Fabrics like polyester shed microplastics when washed." << endl;
                    break;
                case 4:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Going back to the main menu." << endl;
            }
            break;
        }

        case 5:
            cout << "Exiting microplastic tracking. See you next time!" << endl;
            break;

        default:
            cout << "Invalid choice. Exiting microplastic tracking." << endl;
    }
}

void behavioralAnalytics()
{
    int behavioralChoice;
    const string blue = "\033[34m";  // Blue color
    const string reset = "\033[0m";  // Reset color to default
    const string green = "\033[32m";
    cout <<blue<< "Performing behavioral analytics..." << endl<<reset;
    cout <<green<< "Choose the behavioral analysis you want to perform:" << endl<<reset;
    cout << "1. Waste Sorting Behavior" << endl;
    cout << "2. Frequency of Recycling" << endl;
    cout << "3. Waste Reduction Goals" << endl;
    cout << "4. Exit Behavioral Analytics" << endl;
    cout << "Enter your choice: ";
    cin >> behavioralChoice;

    switch (behavioralChoice)
    {
        case 1:
        {
 srand(time(0)); // Initialize random number generator with current time

    int sortingChoice;
    cout << "\nYou chose 'Waste Sorting Behavior'." << endl;
    cout << "Choose an option to analyze:" << endl;
    cout << "1. Accuracy of Sorting" << endl;
    cout << "2. Common Mistakes in Sorting" << endl;
    cout << "3. Sorting Trends Over Time" << endl;
    cout << "4. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> sortingChoice;

    switch (sortingChoice)
    {
        case 1:
            {
                // Generate random accuracy percentage between 80% and 90%
                int randomPercentage = rand() % 11 + 80; // Random value between 80 and 90
                cout << "Analyzing sorting accuracy: "
                     << randomPercentage << "% of users correctly sort recyclables!" << endl;
            }
            break;
        case 2:
            {
                // Generate random mistake percentage between 30% and 50%
                int randomPercentage = rand() % 21 + 30; // Random value between 30 and 50
                cout << "Common sorting mistakes: "
                     << randomPercentage << "% of users confuse paper with plastic!" << endl;
            }
            break;
        case 3:
            {
                // Generate random improvement percentage between 5% and 15% for sorting trends
                int randomPercentage = rand() % 11 + 5; // Random value between 5 and 15
                cout << "Sorting trends: Over the past month, sorting accuracy improved by "
                     << randomPercentage << "%!" << endl;
            }
            break;
        case 4:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
    }
            break;
        }

        case 2:
        {
            srand(time(0)); // Initialize random number generator with current time

    int recyclingChoice;
    cout << "\nYou chose 'Frequency of Recycling'." << endl;
    cout << "Choose an option to analyze:" << endl;
    cout << "1. Weekly Recycling Frequency" << endl;
    cout << "2. Monthly Recycling Frequency" << endl;
    cout << "3. Seasonal Recycling Trends" << endl;
    cout << "4. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> recyclingChoice;

    switch (recyclingChoice)
    {
        case 1:
            {
                // Generate random percentage between 60% and 80% for weekly recycling
                int randomPercentage = rand() % 21 + 60; // Random value between 60 and 80
                cout << "Weekly recycling frequency: "
                     << randomPercentage << "% of users recycle at least once a week!" << endl;
            }
            break;
        case 2:
            {
                // Generate random number of items recycled between 10 and 20 items per month
                int randomItems = rand() % 11 + 10; // Random value between 10 and 20
                cout << "Monthly recycling frequency: On average, users recycle "
                     << randomItems << " items per month." << endl;
            }
            break;
        case 3:
            {
                // Generate random percentage increase between 20% and 30% for seasonal recycling trends
                int randomPercentage = rand() % 11 + 20; // Random value between 20 and 30
                cout << "Seasonal trends: Recycling increases by "
                     << randomPercentage << "% during summer months!" << endl;
            }
            break;
        case 4:
            cout << "Going back to the main menu." << endl;
            break;
        default:
            cout << "Invalid choice. Going back to the main menu." << endl;
        }

            break;
        }

        case 3:
        {
            int reductionChoice;
            cout << "\nYou chose 'Waste Reduction Goals'." << endl;
            cout << "Choose an option to analyze:" << endl;
            cout << "1. Personal Waste Reduction Targets" << endl;
            cout << "2. Community Waste Reduction Goals" << endl;
            cout << "3. Success Rate of Waste Reduction Goals" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            cin >> reductionChoice;

            switch (reductionChoice)
            {
                case 1:
                    cout << "Personal goals: 60% of individuals achieve their waste reduction target of 30%." << endl;
                    break;
                case 2:
                    cout << "Community goals: Local community reduced waste by 20% last year!" << endl;
                    break;
                case 3:
                    cout << "Goal success rate: 50% of users achieve their waste reduction goals within 6 months." << endl;
                    break;
                case 4:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Going back to the main menu." << endl;
            }
            break;
        }

        case 4:
            cout << "Exiting behavioral analytics. See you next time!" << endl;
            break;

        default:
            cout << "Invalid choice. Exiting behavioral analytics." << endl;
    }
}

void wasteManagementGamification()
{
    int gamificationChoice;
     const string blue = "\033[34m";  // Blue color
    const string reset = "\033[0m";  // Reset color to default
    const string green = "\033[32m";
    cout <<blue<< "Initiating waste management gamification..." << endl<<reset;
    cout <<green<< "Choose a gamification feature:" << endl<<reset;
    cout << "1. Rewards System" << endl;
    cout << "2. Points System" << endl;
    cout << "3. Challenges" << endl;
    cout << "4. Exit Gamification" << endl;
    cout << "Enter your choice: ";
    cin >> gamificationChoice;

    switch (gamificationChoice) {
        case 1: {
            int rewardChoice;
            cout << "\nYou chose the Rewards System." << endl;
            cout << "Choose a reward type:" << endl;
            cout << "1. Badges" << endl;
            cout << "2. Coupons" << endl;
            cout << "3. Achievement Unlocks" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            cin >> rewardChoice;

            switch (rewardChoice)
        {
                case 1:
                    cout << "You earned a 'Green Warrior' badge for reducing plastic waste!" << endl;
                    break;
                case 2:
                    cout << "You earned a 10% off coupon for recycling electronics!" << endl;
                    break;
                case 3:
                    cout << "Achievement unlocked: 'Waste Reduction Master'!" << endl;
                    break;
                case 4:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Going back to the main menu." << endl;
            }
            break;
        }

        case 2:
        {
            int pointsChoice;
            cout << "\nYou chose the Points System." << endl;
            cout << "Choose an action to earn points:" << endl;
            cout << "1. Recycling Paper" << endl;
            cout << "2. Recycling Plastic" << endl;
            cout << "3. Composting Organic Waste" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            cin >> pointsChoice;

            switch (pointsChoice)
            {
                case 1:
                    cout << "You earned 10 points for recycling paper!" << endl;
                    break;
                case 2:
                    cout << "You earned 15 points for recycling plastic!" << endl;
                    break;
                case 3:
                    cout << "You earned 20 points for composting organic waste!" << endl;
                    break;
                case 4:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Going back to the main menu." << endl;
            }
            break;
        }

        case 3:
    {
            int challengeChoice;
            cout << "\nYou chose the Challenges." << endl;
            cout << "Choose a challenge to participate in:" << endl;
            cout << "1. Plastic-Free Week Challenge" << endl;
            cout << "2. Zero Waste Month Challenge" << endl;
            cout << "3. Neighborhood Cleanup Challenge" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            cin >> challengeChoice;

            switch (challengeChoice)
            {
                case 1:
                    cout << "Challenge started: 'Plastic-Free Week'! Good luck!" << endl;
                    break;
                case 2:
                    cout << "Challenge started: 'Zero Waste Month'! Let's reduce waste!" << endl;
                    break;
                case 3:
                    cout << "Challenge started: 'Neighborhood Cleanup'! Make your community clean!" << endl;
                    break;
                case 4:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Going back to the main menu." << endl;
            }
            break;
        }

        case 4:
            cout << "Exiting gamification features. See you next time!" << endl;
            break;

        default:
            cout << "Invalid choice. Exiting gamification features." << endl;
    }
}

// Structure to hold vehicle information
struct Vehicle
{
    string userId;  // User ID of the driver
    string driver;  // Name of the driver
    string wasteCollector;  // Name of the waste collector
    float organicWeight;  // Weight of organic waste
    float inorganicWeight;  // Weight of inorganic waste
    float electronicWeight;  // Weight of electronic waste
    string truckNumber;  // Truck number
    string vehicleType;         // Ensure these members are correctly defined
    string registrationNumber;
    string color;
};

// Function prototypes
void displayAreas(const vector<string>& areas, const unordered_map<string, vector<Vehicle>>& areaVehicles);
void displaySpecificAreaDetails(const string& area, const unordered_map<string, vector<Vehicle>>& areaVehicles);
void areaSubMenu(const string& area, unordered_map<string, vector<Vehicle>>& areaVehicles);
void displayDriverDetails(const string& area, const unordered_map<string, vector<Vehicle>>& areaVehicles);
void displayCollectorDetails(const string& area, const unordered_map<string, vector<Vehicle>>& areaVehicles);
void viewAllAreasWithUpdatedCollectors(const unordered_map<string, vector<Vehicle>>& areaVehicles);
void calculateBilling(const unordered_map<string, vector<Vehicle>>& areaVehicles, const string& area);
void updateWasteCollectors(unordered_map<string, vector<Vehicle>>& areaVehicles, const vector<string>& areas);
void displayAllUpdatedAreas(const vector<string>& areas, const unordered_map<string, vector<Vehicle>>& areaVehicles);
void searchDriverById(const unordered_map<string, vector<Vehicle>>& areaVehicles, const string& userId);
void searchDriverByName(const unordered_map<string, vector<Vehicle>>& areaVehicles, const string& driverName);
bool bruteForceStringMatch(const string& text, const string& pattern);
void searchAreaBySubstring(const vector<string>& areaNames, const string& query);
void addVehicle(unordered_map<string, vector<Vehicle>>& areaVehicles, const string& area);
void removeVehicle(unordered_map<string, vector<Vehicle>>& areaVehicles, const string& area);
void displaySearchHistory(const vector<string>& searchHistory);
void searchCollectorByName(const unordered_map<string, vector<Vehicle>>& areaVehicles, const string& collectorName);
void loadAreaVehiclesFromFile(const string& fileName, unordered_map<string, vector<Vehicle>>& areaVehicles, vector<string>& areas);
bool login();
void displayMenu(unordered_map<string, vector<Vehicle>>& areaVehicles, vector<string>& areas);

unordered_set<string> validUsers = {"admin"};
unordered_set<string> validPasswords = {"password"};

// Function to simulate a delayed print effect
void delayedPrint(const string& text, unsigned int delay = 50)
{
    for (const char& c : text)
    {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
}

// Function to display all areas
void displayAreas(const vector<string>& areas)
{
    cout << "Available Areas:\n";
    cout << "----------------\n";
    for (const auto& area : areas)
    {
        cout << area << '\n';
    }
    cout << "-----------------\n";
}
// Function to display specific area details without driver and collector
// Function to display details of vehicles in a specific area
void displaySpecificAreaDetails(const string& area, unordered_map<string, vector<Vehicle>>& areaVehicles)
{
    const string yellow = "\033[33m";  // Yellow color code for outline
    const string reset = "\033[0m";    // Reset color to default for text

    if (areaVehicles.find(area) == areaVehicles.end())
    {
        cout << "Area \"" << area << "\" not found.\n";
        return;
    }

    // Print the table with the outline in yellow and text in default color
    cout << yellow << "\nDetails for area \"" << area << "\":\n";
    cout << "+------------------------+" << reset << endl;  // Yellow outline
    cout << "|       User ID          |" << endl;
    cout << "+------------------------+" << reset << endl;  // Yellow outline
    for (const auto& vehicle : areaVehicles[area])
    {
        cout << "| " << left << setw(22) << vehicle.userId << " |" << endl;
    }
    cout << yellow << "+------------------------+" << reset << endl;  // Yellow outline
}
// Function to search for a driver by name (case-sensitive)
void searchDriverByName(const unordered_map<string, vector<Vehicle>>& areaVehicles, const string& driverName)
{
    bool found = false;
    for (const auto& area : areaVehicles)
    {
        for (const auto& vehicle : area.second)
        {
            if (vehicle.driver == driverName)
            { // Compare directly without toLower
                cout << "Driver found in area " << area.first << ":\n";
                cout << "-------------------------------------------------------------------------------------\n";
                cout << left << setw(20) << "User ID" << setw(20) << "Driver" << setw(20) << "Waste Collector" << setw(20) << "Truck Number" << endl;
                cout << "--------------------------------------------------------------------------------------\n";
                cout << left << setw(20) << vehicle.userId << setw(20) << vehicle.driver << setw(20) << vehicle.wasteCollector << setw(20) << vehicle.truckNumber << endl;
                cout << "---------------------------------------------------------------------------------------\n";
                found = true;
                break; // Exit the loop after finding the driver
            }
        }

        if (found) break; // Exit the outer loop if the driver is found
    }
    if (!found)
    {
        cout<<"Driver not found.\n";
    }
}

// Function to search for a collector by name (case-sensitive)
void searchCollectorByName(const unordered_map<string, vector<Vehicle>>& areaVehicles, const string& collectorName)
{
    bool found = false;
    for (const auto& area : areaVehicles)
    {
        for (const auto& vehicle : area.second)
        {
            if (vehicle.wasteCollector == collectorName)
            { // Compare directly without toLower
                cout << "Collector found in area " << area.first << ":\n";
                cout << "---------------------------------------------------------------------------\n";
                cout << left << setw(20) << "User ID" << setw(20) << "Driver" << setw(20) << "Waste Collector" << setw(20) << "Truck Number" << endl;
                cout << "----------------------------------------------------------------------------\n";
                cout << left << setw(20) << vehicle.userId << setw(20) << vehicle.driver << setw(20) << vehicle.wasteCollector << setw(20) << vehicle.truckNumber << endl;
                cout << "-----------------------------------------------------------------------------\n";
                found = true;
                break; // Exit the loop after finding the collector
            }
        }
        if (found) break; // Exit the outer loop if the collector is found
    }
    if (!found)
    {
       cout<<"Collector not found.\n";
    }
}

// Function to display driver details
void displayDriverDetails(const string& area, const unordered_map<string, vector<Vehicle>>& areaVehicles)
{
    auto vehicleIt = areaVehicles.find(area);
    if (vehicleIt == areaVehicles.end())
    {
       cout<<"Area \"" + area + "\" not found.\n";
        return;
    }

    cout << "\nDriver Details for " << area << ":\n";
    cout << "----------------------------------------\n";
    cout << left << setw(20) << "User ID" << setw(20) << "Driver" << endl;
    cout << "----------------------------------------\n";

    for (const auto& vehicle : vehicleIt->second) {
        cout << setw(20) << vehicle.userId << setw(20) << vehicle.driver << endl;
    }
    cout << "----------------------------------------\n";
}

// Function to display collector details
void displayCollectorDetails(const string& area, const unordered_map<string, vector<Vehicle>>& areaVehicles)
{
    auto vehicleIt = areaVehicles.find(area);
    if (vehicleIt == areaVehicles.end())
    {
       cout<<"Area \"" + area + "\" not found.\n";
        return;
    }

    cout << "\nCollector Details for " << area << ":\n";
    cout << "----------------------------------------\n";
    cout << left << setw(20) << "Waste Collector" << endl;
    cout << "----------------------------------------\n";

    for (const auto& vehicle : vehicleIt->second)
    {
        cout << setw(20) << vehicle.wasteCollector << endl;
    }
    cout << "----------------------------------------\n";
}

// Function to view all areas with updated collectors
void viewAllAreasWithUpdatedCollectors(const unordered_map<string, vector<Vehicle>>& areaVehicles)
{
    cout << "\nAll Areas with Updated Collectors:\n";
    cout << "----------------------------------------\n";
    cout << left << setw(20) << "Area" << setw(20) << "Updated Collector" << endl;
    cout << "----------------------------------------\n";

    for (const auto& area : areaVehicles)
    {
        for (const auto& vehicle : area.second)
        {
            cout << setw(20) << area.first << setw(20) << vehicle.wasteCollector << endl;
        }
    }
    cout << "----------------------------------------\n";
}

struct Vehicle2 {
    float organicWeight;    // Weight of organic waste
    float inorganicWeight;  // Weight of inorganic waste
    float electronicWeight; // Weight of electronic waste
};

// Function to calculate the billing details for an area
void calculateBilling(const unordered_map<string, vector<Vehicle>>& areaVehicles, const string& area)
{
    // Billing rates per kg
    float organicRate = 5.0f;  // Rate per kg for organic waste
    float inorganicRate = 8.0f;  // Rate per kg for inorganic waste
    float electronicRate = 10.0f;  // Rate per kg for electronic waste

    // Random number seed initialization
    srand(time(0)); // Seed for random number generation based on time

    auto vehicleIt = areaVehicles.find(area);
    if (vehicleIt == areaVehicles.end())
    {
        cout << "Area \"" + area + "\" not found.\n";
        return;
    }

    float totalBillingAmount = 0.0f;

    cout << "\nBilling details for Area: " << area << "\n";
    cout << "-----------------------------------------------------------------------------\n";
    cout << left << setw(20) << "Waste Type" << setw(20) << "Weight (kg)"
         << setw(20) << "Rate (per kg)" << setw(20) << "Total Cost" << endl;
    cout << "-----------------------------------------------------------------------------\n";

    // Loop through all vehicles in the area and calculate the billing
    for (const auto& vehicle : vehicleIt->second)
    {
        // Generate random weights for each waste type up to 50 kg
        float organicWeight = rand() % 51;  // Random number between 0 and 50
        float inorganicWeight = rand() % 51; // Random number between 0 and 50
        float electronicWeight = rand() % 51; // Random number between 0 and 50

        // Calculate costs for each waste type
        float organicCost = organicWeight * organicRate;
        float inorganicCost = inorganicWeight * inorganicRate;
        float electronicCost = electronicWeight * electronicRate;

        // Display the breakdown
        cout << setw(20) << "Organic"
             << setw(20) << organicWeight
             << setw(20) << organicRate
             << setw(20) << organicCost << endl;

        cout << setw(20) << "Inorganic"
             << setw(20) << inorganicWeight
             << setw(20) << inorganicRate
             << setw(20) << inorganicCost << endl;

        cout << setw(20) << "Electronic"
             << setw(20) << electronicWeight
             << setw(20) << electronicRate
             << setw(20) << electronicCost << endl;

        totalBillingAmount += organicCost + inorganicCost + electronicCost;
    }

    cout << "-----------------------------------------------------------------------------\n";
    cout << "Total Billing Amount: " << totalBillingAmount << " units\n";
    cout << "-----------------------------------------------------------------------------\n";
}
// Function to update waste collectors cyclically after 3 days
void updateWasteCollectors(unordered_map<string, vector<Vehicle>>& areaVehicles, const vector<string>& areas)
{
    string tempCollector;
    for (size_t i = 0; i < areas.size(); ++i)
    {
        size_t nextIndex = (i + 1) % areas.size();  // Get the next area in the list, cyclically
        tempCollector = areaVehicles[areas[nextIndex]][0].wasteCollector;
        areaVehicles[areas[nextIndex]][0].wasteCollector = areaVehicles[areas[i]][0].wasteCollector;
        areaVehicles[areas[i]][0].wasteCollector = tempCollector;
    }
   cout<<"Waste collectors updated for all areas after 3 days.\n";
}

// Function to display updated areas with collectors
void displayAllUpdatedAreas(const vector<string>& areas, const unordered_map<string, vector<Vehicle>>& areaVehicles)
{
    cout << "\nUpdated Waste Collector Details for All Areas:\n";
    cout << "--------------------------------------------------\n";
    cout << left << setw(20) << "Area" << setw(20) << "Driver"
         << setw(20) << "Waste Collector" << setw(20) << "Truck Number" << endl;
    cout << "---------------------------------------------------\n";

    for (const auto& area : areas)
    {
        auto vehicleIt = areaVehicles.find(area);
        if (vehicleIt != areaVehicles.end())
        {
            for (const auto& vehicle : vehicleIt->second)
            {
                cout << setw(20) << area
                     << setw(20) << vehicle.driver
                     << setw(20) << vehicle.wasteCollector
                     << setw(20) << vehicle.truckNumber << endl;
            }
        }
    }
    cout << "----------------------------------------\n";
}

// Function to search for a driver by user ID
void searchDriverById(const unordered_map<string, vector<Vehicle>>& areaVehicles, const string& userId)
{
    for (const auto& area : areaVehicles)
    {
        for (const auto& vehicle : area.second)
        {
            if (vehicle.userId == userId)
            {
                cout << "\nDriver Details:\n";
                cout << "---------------------------------------------\n";
                cout << left << setw(20) << "User ID" << setw(20) << "Driver"
                     << setw(20) << "Waste Collector" << setw(20) << "Truck Number" << endl;
                cout << "----------------------------------------------\n";
                cout << setw(20) << vehicle.userId
                     << setw(20) << vehicle.driver
                     << setw(20) << vehicle.wasteCollector
                     << setw(20) << vehicle.truckNumber << endl;
                cout << "----------------------------------------------\n";
                return;
            }
        }
    }
   cout<<"Driver with User ID \"" + userId + "\" not found.\n";
}

// Brute force string matching algorithm
bool bruteForceStringMatch(const string& text, const string& pattern)
{
    int n = text.length();
    int m = pattern.length();

    // Loop through each character in the text
    for (int i = 0; i <= n - m; ++i)
    {
        int j = 0;

        // Check if pattern matches the substring starting at index i
        while (j < m && text[i + j] == pattern[j])
        {
            ++j;
        }

        // If all characters matched
        if (j == m)
        {
            return true;  // Match found
        }
    }

    return false;  // No match found
}

// Function to search for an area by substring
void searchAreaBySubstring(const vector<string>& areaNames, const string& query)
{
    cout << "\nAreas matching \"" << query << "\":\n";
    bool found = false;
    for (const auto& area : areaNames)
    {
        if (bruteForceStringMatch(area, query))
        {
            cout << area << endl;
            found = true;
        }
    }
    if (!found)
    {
       cout<<"No areas found matching \"" + query + "\".\n";
    }
}

// Function to ask if the user wants to search for another area
void askForAnotherArea(unordered_map<string, vector<Vehicle>>& areaVehicles)
{
    char choice;
    cout << "\nWould you like to search for another Colony? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        string newArea;
        cout << "Enter the Colony name: ";
        cin >> newArea;
        areaSubMenu(newArea, areaVehicles);
    }
}

// Function to add a new vehicle to a specific area
void addVehicle(unordered_map<string, vector<Vehicle>>& areaVehicles, const string& area)
{
    Vehicle newVehicle;
    cout << "Enter User ID: ";
    cin >> newVehicle.userId;
    cin.ignore(); // Clear the input buffer
    cout << "Enter Driver Name: ";
    getline(cin, newVehicle.driver);
    cout << "Enter Waste Collector Name: ";
    getline(cin, newVehicle.wasteCollector);
    cout << "Enter Organic Weight (kg): ";
    cin >> newVehicle.organicWeight;
    cout << "Enter Inorganic Weight (kg): ";
    cin >> newVehicle.inorganicWeight;
    cout << "Enter Electronic Weight (kg): ";
    cin >> newVehicle.electronicWeight;
    cout << "Enter Truck Number: ";
    cin >> newVehicle.truckNumber;

    areaVehicles[area].push_back(newVehicle);
   cout<<"Vehicle added successfully to " + area + ".\n";
}

// Function to remove a vehicle from a specific area
void removeVehicle(unordered_map<string, vector<Vehicle>>& areaVehicles, const string& area)
{
    string userId;
    cout << "Enter User ID of the vehicle to remove: ";
    cin >> userId;

    auto& vehicles = areaVehicles[area];
    for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
    {
        if (it->userId == userId)
        {
            vehicles.erase(it);
            cout<<"Vehicle with User ID \"" + userId + "\" removed successfully from " + area + ".\n";
            return;
        }
    }
    cout<<"Vehicle with User ID \"" + userId + "\" not found in " + area + ".\n";
}

// Function for area sub-menu
void areaSubMenu(const string& area, unordered_map<string, vector<Vehicle>>& areaVehicles)
{
    int choice;
    do
        {
         int width = 60;  // Define the width for centering text
        const string BLUE = "\033[34m";
        const string YELLOW = "\033[33m";
        const string CYAN = "\033[36m";
          const string RESET = "\033[0m";
    // Design the output with colors and center the text
    cout << YELLOW << "******************************" << RESET << endl;
    cout << YELLOW << "* " << BLUE << "Sub-menu for " << CYAN << area << BLUE << ":" << YELLOW << " *" << RESET << endl;
    cout << YELLOW << "******************************" << RESET << endl;
        cout << "1. View userid\n";
        cout << "2. View driver\n";
        cout << "3. View collector\n";
        cout << "4. Calculate billing\n";
        cout << "5. Update waste collector after 3 days\n";
        cout << "6. View all colonies with updated collectors\n";
        cout << "7. Add a new vehicle\n";
        cout << "8. Remove a vehicle\n";
        cout << "0. Go back to main menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                displaySpecificAreaDetails(area, areaVehicles);
                break;
            case 2:
                displayDriverDetails(area, areaVehicles);
                break;
            case 3:
                displayCollectorDetails(area, areaVehicles);
                break;
            case 4:
                calculateBilling(areaVehicles, area);
                break;
            case 5:
                updateWasteCollectors(areaVehicles, vector<string>{area});
                break;
            case 6:
                viewAllAreasWithUpdatedCollectors(areaVehicles);
                break;
            case 7:
                addVehicle(areaVehicles, area);
                break;
            case 8:
                removeVehicle(areaVehicles, area);
                break;
            case 0:
                cout << "Going back to main menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
         if (choice == 6)
        {
            cout << "Would you like to search for another area? (y/n): ";
            char another;
            cin >> another;
            if (another == 'y' || another == 'Y')
            {
                string newArea;
                cout << "Enter Colony name to search: ";
                cin >> newArea;
                areaSubMenu(newArea, areaVehicles);
                break;
            }
        }
    }
    while (choice != 0);
}
// Function for user login
const int d = 256;  // Number of characters in the input alphabet (ASCII values)
const int q = 101;  // A prime number used for hashing

// Rabin-Karp algorithm to find if the 'pattern' exists in 'text'
bool rabinKarpSearch(const string& text, const string& pattern)
{
    int m = pattern.length();
    int n = text.length();
    int i, j;
    int pHash = 0;  // hash value for pattern
    int tHash = 0;  // hash value for text
    int h = 1;

    // Calculate the value of h = (d^(m-1)) % q
    for (i = 0; i < m - 1; i++)
    {
        h = (h * d) % q;
    }

    // Calculate the hash value of the pattern and the first window of text
    for (i = 0; i < m; i++)
    {
        pHash = (d * pHash + pattern[i]) % q;
        tHash = (d * tHash + text[i]) % q;
    }

    // Slide the pattern over the text one by one
    for (i = 0; i <= n - m; i++)
    {
        // Check if the hash values are the same
        if (pHash == tHash)
    {
            // If the hash values match, then check for character by character match
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j])
                {
                    break;
                }
            }
            if (j == m)
            {
                return true;  // Pattern found
            }
        }

        // Calculate the hash value for the next window of text
        if (i < n - m)
        {
            tHash = (d * (tHash - text[i] * h) + text[i + m]) % q;
            if (tHash < 0)
            {
                tHash = (tHash + q);  // Convert negative value of tHash to positive
            }
        }
    }

    return false;  // No match found
}

// Function to perform login using Rabin-Karp algorithm
bool login()
{
    // Predefined login credentials (Login ID, Password pairs)
    struct Credential
    {
        string loginId;
        string password;
    };

    // List of valid credentials
    Credential credentials[] =
    {
        {"admin1", "1234"},
        {"admin2", "4567"},
        {"admin3", "8901"},
        {"admin4", "2345"}
    };

    string loginId;
    string password;

    cout << "Enter Login ID: ";
    cin >> loginId;

    cout << "Enter Password: ";
    char ch;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!password.empty())
            {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else
        {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;

    // Check if loginId and password match any valid credential using Rabin-Karp search
    for (const auto& cred : credentials)
        {
        bool loginIdValid = rabinKarpSearch(loginId, cred.loginId);
        bool passwordValid = rabinKarpSearch(password, cred.password);

        if (loginIdValid && passwordValid)
        {
            return true;  // Match found, login successful
        }
    }

    return false;  // No match found
}
void printWithDelay3(const string& message)
{
    const string magenta = "\033[35m";  // Magenta color code
    const string reset = "\033[0m";     // Reset color to default

    // Print the message with a delay between each character, in magenta
    for (char c : message)
    {
        cout << magenta << c << reset;  // Print each character in magenta
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(100)); // Adjust delay for timing
    }
    cout << endl;
}
void displayMenu(unordered_map<string, vector<Vehicle>>& areaVehicles, vector<string>& areas)
{
    int choice;
    do
        {
       const string green = "\033[32m";   // Green color for dashes
    const string red = "\033[31m";     // Red color for stars
    const string reset = "\033[0m";    // Reset color to default

    // Print the design
    cout << green << "-----------------------------" << red << "********" << green << "-----------------------------" << reset << endl;
        printWithDelay3("\n\t\tWASTE MANAGEMENT DATA COLLECTION\n");
        cout << green << "-----------------------------" << red << "********" << green << "-----------------------------" << reset << endl;
        printWithDelay3("\t\t\t   Main Menu:\n");
        cout<<green << "-------------------------------------------------------------------\n"<<reset;
        cout << "1. Display all Colonies\n";
        cout << "2. Search for an Colony\n";
        cout << "3. Search for a driver by name\n";
        cout << "4. Search for a collector by name\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                displayAreas(areas);
                break;
            case 2:
            {
                string areaName;
                cout << "Enter area name to search: ";
                cin >> areaName;
                areaSubMenu(areaName, areaVehicles); // Directly call the area sub-menu
                break;
            }
 case 3: {
                string driverName;
                char searchAgain;
                do {
                    cout << "Enter Driver name to search: ";
                    cin >> driverName;
                    searchDriverByName(areaVehicles, driverName);
                    cout << "Would you like to search for another driver? (y/n): ";
                    cin >> searchAgain;
                } while (searchAgain == 'y' || searchAgain == 'Y');
                break;
            }
            case 4:
            {
                string collectorName;
                char searchAgain;
                do {
                    cout << "Enter Collector name to search: ";
                    cin >> collectorName;
                    searchCollectorByName(areaVehicles, collectorName);
                    cout << "Would you like to search for another collector? (y/n): ";
                    cin >> searchAgain;
                } while (searchAgain == 'y' || searchAgain == 'Y');
                break;
            }
            case 5:
               printWithDelay3("\nReturning to SMART CITY WASTE MANAGEMENT.......\n");
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    while (choice != 5);
}
void printTitleWithStyle()
{
    string title = "\tSMART CITY WASTE MANAGEMENT\t";

    // Design borders or decorations for the title
    string border = "* * * * * * * * * * * * * * * * * * *";

    // Print the top border
    cout << border << endl;

    // Print title with a delay between each character
    for (char c : title)
    {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(150)); // Adjust delay for timing
    }

    // Print the bottom border
    cout << endl << border << endl;
}
void printWithDelay(const string& text, int delayMs)
{
    for (char ch : text)
    {
        cout << ch;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << endl;
}
struct WasteDetails
{
    string wasteType;
    float weight;  // Weight of the waste in kg
};

// Maximum bin capacity in kg
const float MAX_BIN_CAPACITY = 1000.0;  // Total bin capacity is 1000 kg
float currentWeight = 0.0;  // Keeps track of the current weight in the bin

// Bin capacities for each waste type
const float MAX_ORGANIC_CAPACITY = 250.0;
const float MAX_INORGANIC_CAPACITY = 250.0;
const float MAX_ELECTRONIC_CAPACITY = 250.0;
const float MAX_UNKNOWN_CAPACITY = 250.0;

// Current weight for each waste category
float currentOrganicWeight = 0.0;
float currentInorganicWeight = 0.0;
float currentElectronicWeight = 0.0;
float currentUnknownWeight = 0.0;

// Function to display a reminder for waste collection day
void remindWasteCollection(int dayOfWeek)
{
    // Get the current day and time
    time_t t = time(0);
    tm* now = localtime(&t);

    // Calculate next waste collection day
    int daysUntilNextCollection = (7 - dayOfWeek) % 7;  // days left until next Sunday

    // Get the name of the day (for more personalized output)
    string dayName;
    switch (dayOfWeek) {
        case 1: dayName = "Monday"; break;
        case 2: dayName = "Tuesday"; break;
        case 3: dayName = "Wednesday"; break;
        case 4: dayName = "Thursday"; break;
        case 5: dayName = "Friday"; break;
        case 6: dayName = "Saturday"; break;
        case 7: dayName = "Sunday"; break;
        default: dayName = "Invalid day"; break;
    }

    // Get user input for personalized reminder
    string userName;
    cout << "Enter your name for a personalized reminder: ";
    cin >> userName;

    // Display messages based on the current day
    if (dayOfWeek == 6)
    {
        const string GREEN = "\033[32m";
        const string BLUE = "\033[34m";
        const string RESET = "\033[0m";
         cout<< "Hey " <<userName << RESET << ", " << endl;
         cout << "Today is: " << dayName << RESET << endl;
        cout << "Reminder: Yeah!!! Tomorrow is waste collection day!" << endl;
        cout << "Prepare your waste for collection tomorrow!" << endl;
        cout << "\nReminder Tip: Make sure your recyclables are sorted properly." << endl;
    }
    else
    {
          const string GREEN = "\033[32m";
        const string BLUE = "\033[34m";
        const string RESET = "\033[0m";
        cout<<"Hello " <<userName << ", " << endl;
        cout<< "Today is:"<<dayName;
        cout << "\nNo collection today. Stay tuned for the reminder." << endl;
        cout << "Waste collection will be in " << daysUntilNextCollection << " day(s)" << endl;
        cout << "\nTip: Be sure to separate organic waste from recyclables before the collection day." << endl;
    }

    // Provide additional user interaction options for setting reminders
    char setReminder;
    cout << "\nWould you like to set a reminder for tomorrow? (1 for Yes, 0 for No): ";
    cin >> setReminder;
    if (setReminder == '1')
    {
        // Simulate setting a reminder
        cout << "Reminder set! We'll notify you tomorrow about waste collection." << endl;
    }
    else
    {
        cout << "Reminder not set. You can always check back for updates." << endl;
    }
}
// Function to convert day of the week number to name
string getDayOfWeek2(int dayOfWeek2) {
    switch (dayOfWeek2) {
        case 1: return "Monday";
        case 2: return "Tuesday";
        case 3: return "Wednesday";
        case 4: return "Thursday";
        case 5: return "Friday";
        case 6: return "Saturday";
        case 7: return "Sunday";
        default: return "Invalid Day";
    }
}
struct UserSummary {
    int userID;
    string userName;
    string wasteType;
    float wasteWeight;
    int dayOfWeek2;
    int rewardPoints;
};
// Function to display all user data after the use of all users
void displayAllUsersData(const vector<UserSummary>& userSummaries) {
    cout << "\nUser Data Summary: " << endl;
    cout << "_________________________________________________________________________________________" << endl;
    cout << "| User ID | User Name | Day of the Week | Reward Points | Waste Type | Waste Weight (kg) |" << endl;
    cout << "|_________|___________|_________________|_______________|____________|___________________|" << endl;

    // Iterate through each user summary to display the information.
    for (size_t i = 0; i < userSummaries.size(); ++i) {
        const auto& summary = userSummaries[i];

        // If it's the first occurrence of the user ID, print full information
        if (i == 0 || userSummaries[i].userID != userSummaries[i - 1].userID) {
            cout << "| " << setw(7) << summary.userID
                 << " | " << setw(9) << summary.userName
                 << " | " << setw(15) << getDayOfWeek2(summary.dayOfWeek2)
                 << " | " << setw(13) << summary.rewardPoints << " |";
        } else {
            // Otherwise, just print empty spaces for the repeated fields
            cout << "| " << setw(8) << " "
                 << "|"  << setw(11) << " "
                 << "| " << setw(16) << " "
                 << "| " << setw(14) << " "
                 << "|";
        }

        // Always print the Waste Type and Waste Weight for each line
        cout << setw(11) << summary.wasteType
             << " |" << setw(18) << fixed << setprecision(2) << summary.wasteWeight << " |" << endl;
    }

    cout << "|_________|___________|_________________|_______________|____________|___________________|" << endl;
}



// Function to simulate LED Color Indicator based on waste type and where to place the waste
void displayLEDIndicator(int wasteType)
{
    const int blinkDuration = 500;  // Duration for each blink in milliseconds (500ms)

    // LED blinking effect simulation
    auto blinkLED = [&](const string& color, const string& message) {
        int numBlinks = 5;
        int cursorPosition = 10;  // Line number where the LED message will be displayed

        // Initial message display
        cout << "\033[" << cursorPosition << ";0H" << color << message << "\033[0m" << endl;

        // Simulate LED blinking by moving cursor and toggling on/off effect
        for (int i = 0; i < numBlinks; ++i)  // LED blinks 5 times
        {
            // Move cursor back to the position where LED message is shown
            cout << "\033[" << cursorPosition << ";0H" << "\033[0J";  // Clear the line

            // Display LED color and message
            if (i % 2 == 0)
            {
                // Turn LED on (display message)
                cout << color << message << "\033[0m";
            }
            else
            {
                // Turn LED off (clear message)
                cout << "                                ";  // Clear the message
            }

            // Wait for a short period to simulate blinking
            this_thread::sleep_for(chrono::milliseconds(blinkDuration));

            // Move the cursor back to the start of the line (so the next print happens in the same place)
            cout << "\033[" << cursorPosition << ";0H";
        }

        // Ensure the LED is on after the blinking sequence ends
        cout << color << message << "\033[0m" << endl;
    };

    // Switch case for waste type with different LED indicators
    switch (wasteType)
    {
        case 1:
            blinkLED("\033[32m", "LED Indicator: Green - Organic Waste. Place the waste in Green slot.");  // Green for Organic Waste
            break;
        case 2:
            blinkLED("\033[34m", "LED Indicator: Blue - Inorganic Waste (Plastic, Paper). Place the waste in Blue slot for recycling.");  // Blue for Inorganic Waste
            break;
        case 3:
            blinkLED("\033[31m", "LED Indicator: Red - Electronic Waste. Place the waste in Red slot for proper disposal.");  // Red for Electronic Waste
            break;
        case 4:
            blinkLED("\033[33m", "LED Indicator: Yellow - Unknown Waste. Please sort this waste properly.");  // Yellow for Unknown Waste
            break;
        default:
            cout << "Invalid waste type selection." << endl;
            return;
    }
}

// Function to display the waste summary in a table format
void displayWasteTable(float currentWeight, float remainingCapacity, const string& ledIndicator, const string& wasteType, const string& slotColor) {
    cout << "_________________________________________________________________________________" << endl;
    cout << "|   Current weight  |   Remaining capacity  | LED indicator | Waste Type | Slot  |" << endl;
    cout << "|___________________|_______________________|_______________|____________|_______|" << endl;
     cout << "| " << setw(14) << currentWeight <<" kg "
         <<"| " << setw(18) << remainingCapacity <<" kg"
         << " | " << setw(13) << ledIndicator
         << " | " << setw(10) << wasteType
         << " | " << setw(6) << slotColor << "|" << endl;
    cout << "|___________________|_______________________|_______________|____________|_______|" << endl;
}

// Function to classify waste and track it
void classifyAndTrackWaste(vector<WasteDetails>& wasteHistory, float& totalUserWaste, vector<UserSummary>& userSummaries, int userID, string userName, int dayOfWeek){
    int wasteType;
    float weight;

    // Waste classification options
    cout << "\nSelect the waste type:" << endl;
    cout << "1: Organic" << endl;
    cout << "2: Inorganic (Plastic, Paper)" << endl;
    cout << "3: Electronic" << endl;
    cout << "4: Unknown" << endl;
    cout << "Enter waste type number: ";
    cin >> wasteType;

    cout << "Enter the weight of the waste (in kg): ";
    cin >> weight;

    string ledIndicator, wasteName, slotColor;
    float remainingCapacity = 0.0;

    switch (wasteType) {
        case 1:
            if (currentOrganicWeight + weight > MAX_ORGANIC_CAPACITY) {
                cout << "Organic bin is full! Cannot add more waste." << endl;
                return;
            }
            currentOrganicWeight += weight;
            ledIndicator = "Green";
            wasteName = "Organic";
            slotColor = "Green";
            remainingCapacity = MAX_ORGANIC_CAPACITY - currentOrganicWeight;
            break;
        case 2:
            if (currentInorganicWeight + weight > MAX_INORGANIC_CAPACITY) {
                cout << "Inorganic bin is full! Cannot add more waste." << endl;
                return;
            }
            currentInorganicWeight += weight;
            ledIndicator = "Blue";
            wasteName = "Inorganic";
            slotColor = "Blue";
            remainingCapacity = MAX_INORGANIC_CAPACITY - currentInorganicWeight;
            break;
        case 3:
            if (currentElectronicWeight + weight > MAX_ELECTRONIC_CAPACITY) {
                cout << "Electronic bin is full! Cannot add more waste." << endl;
                return;
            }
            currentElectronicWeight += weight;
            ledIndicator = "Red";
            wasteName = "Electronic";
            slotColor = "Red";
            remainingCapacity = MAX_ELECTRONIC_CAPACITY - currentElectronicWeight;
            break;
        case 4:
            if (currentUnknownWeight + weight > MAX_UNKNOWN_CAPACITY) {
                cout << "Unknown bin is full! Cannot add more waste." << endl;
                return;
            }
            currentUnknownWeight += weight;
            ledIndicator = "Yellow";
            wasteName = "Unknown";
            slotColor = "Yellow";
            remainingCapacity = MAX_UNKNOWN_CAPACITY - currentUnknownWeight;
            break;
        default:
            cout << "Invalid waste type. Please try again." << endl;
            return;
    }

    // Display the LED indicator and instructions
    displayLEDIndicator(wasteType);
    // Add to history and update total
    wasteHistory.push_back({wasteName, weight});
    totalUserWaste += weight;

  userSummaries.push_back({userID, userName, wasteName, weight, dayOfWeek});
    // Display the table
    displayWasteTable(weight, remainingCapacity, ledIndicator, wasteName, slotColor);



}

// Function to calculate rewards based on total weight of waste
int calculateRewards(float totalWaste)
{
    // Base reward points
    int rewardPoints = 0;

    // Check if the total waste is valid (non-negative)
    if (totalWaste < 0)
    {
        cout << "Error: Total waste cannot be negative.\n";
        return 0;  // Return 0 if waste is negative
    }

    // Standard reward calculation (1 kg = 10 reward points)
    if (totalWaste < 50)
    {
        rewardPoints = totalWaste * 10;  // 10 points per kg for < 50 kg
    }
    else if (totalWaste >= 50 && totalWaste <= 100)
    {
        rewardPoints = totalWaste * 12;  // 12 points per kg for 50-100 kg
    }
    else
    {
        rewardPoints = totalWaste * 15;  // 15 points per kg for > 100 kg
    }

    // Adding bonus points if the waste is sorted (just an example logic)
    if (totalWaste > 75)
    {
        rewardPoints += 50;  // Add bonus if waste > 75 kg
    }

    // Discount or extra points for high waste amounts (example)
    if (totalWaste > 200)
    {
        rewardPoints -= 100;  // Give a discount for > 200 kg (deduct 100 points)
    }

    return rewardPoints;
}

// Function to simulate tamper detection with extended functionality
void detectTampering()
{
    bool tampered = false;  // Set to true if tampering is detected
    int tamperingChecks = 3;  // Number of tampering checks to be performed
    int timerDuration = 5;    // Simulated time in seconds for each tampering check

    // Randomize tampering detection (using srand for different results on each run)
    srand(time(0));  // Initialize the random seed

    // Logging start
     string red = "\033[1;31m";
    string green = "\033[1;32m";
    string yellow = "\033[1;33m";
    string reset = "\033[0m"; // Reset to default color

    // Print the tamper detection system message with the specified colors
    cout << yellow << "~" << reset << red << " Tamper Detection System " << reset << yellow << "~" << reset << endl;
    cout << green << "----------------------------" << reset << endl;

    // Loop to simulate multiple tampering checks
    for (int check = 1; check <= tamperingChecks; ++check)
    {
        cout << "Checking for tampering... Check #" << check << " of " << tamperingChecks << "\n";

        // Simulate a countdown timer for each check
        for (int i = timerDuration; i > 0; --i)
        {
            cout << "Time remaining: " << i << " seconds...\r";
            this_thread::sleep_for(chrono::seconds(1));
        }
        cout << endl;

        // Random chance of detecting tampering (10% chance of tampering being detected)
        tampered = (rand() % 10) < 1;  // 10% chance

        // Detect tampering based on the random result
        if (tampered)
        {
            cout << "Warning: Tampering detected! Alerting authorities...\n";
            break;  // If tampering is detected, exit the loop early
        }
        else
        {
            cout << "No tampering detected in this check.\n";
        }
    }

    // Final message after all checks
    if (!tampered)
    {
        cout << "\nAfter " << tamperingChecks << " checks, no tampering was detected.\n";
    }
}
// Function to simulate sleep mode with enhancements
void activateSleepMode()
{
    bool systemActive = false;  // Simulate system activity
    int sleepDuration = 0;      // Simulated sleep duration in seconds
    int timeout = 3;            // Timeout duration for inactivity check (in seconds)

    // Display initial message
    cout << "\n~System Active Detection~\n";
    cout << "Checking system activity...\n";

    // Simulate system activity checks with a timeout
    while (true)
    {
        cout << "\nIs the system active? (1 for Yes, 0 for No): ";
        cin >> systemActive;

        if (!systemActive)
        {
            cout << "System is inactive. Going into sleep mode.\n";

            // Simulate sleep mode for a set duration
            for (int i = 0; i < timeout; ++i)
            {
                cout << "System in sleep mode for " << (i + 1) << " second(s)...\n";
                this_thread::sleep_for(chrono::seconds(1));
            }

            cout << "\nSystem is now in sleep mode.\n";
            cout << "To wake up the system, press 1 and hit Enter.\n";

            int wakeUp;
            cin >> wakeUp;
            if (wakeUp == 1)
            {
                cout << "System is waking up...\n";
                break;  // Break the loop to wake up the system
            }
            else
            {
                cout << "Invalid input. The system remains in sleep mode.\n";
            }
        }
        else
        {
            cout << "System is active. No action needed.\n";
            break;  // Exit the loop if the system is active
        }
    }

    // After exiting the loop, display the final status
    cout << "\nSystem is fully awake and active now.\n";
}

void printWithDelay2(const string& message, int delay_ms = 100)
{
    // ANSI escape codes for colors
    const string colors[] = {
        "\033[38;5;196m", // Bright Red
        "\033[38;5;226m", // Bright Yellow
        "\033[38;5;51m",  // Bright Cyan
        "\033[38;5;213m", // Bright Magenta
        "\033[38;5;46m",  // Bright Green
        "\033[38;5;32m",  // Bright Blue
        "\033[97m",       // Bright White
    };
    const string reset = "\033[0m"; // Reset color to default

    // Check if the message is empty, in which case no printing is needed
    if (message.empty()) {
        return; // Do nothing if the message is empty
    }

    // Split the message into words
    stringstream ss(message);
    string word;
    int colorIndex = 0; // To keep track of the colors

    // Print each word with a different color
    while (ss >> word) {
        // Select color and print the word with delay
        cout << colors[colorIndex % 7] << word << reset << " "; // Cycle through the colors
        colorIndex++; // Move to the next color

        // Print the word with a delay between each character
        this_thread::sleep_for(chrono::milliseconds(delay_ms)); // Adjustable delay
    }

    cout << endl;
}
// Function to print a styled, centered message with a delay
void printWithDelay5(const string& message, int delayMs = 100, int screenWidth = 80)
{
    // ANSI escape codes for colors
    const string brightRed = "\033[1;31m";   // Bright Red color for border
    const string brightGreen = "\033[1;32m"; // Bright Green color for message text
    const string reset = "\033[0m";          // Reset color to default

    // Calculate the number of spaces needed to center the message
    int spacesBeforeText = (screenWidth - message.length()) / 2;

    // Print the top border in bright red
    for (int i = 0; i < screenWidth; ++i)
    {
        cout << brightRed << "#";  // Border symbol in bright red
    }
    cout << reset << endl;

    // Print a small space before the message
    cout << endl;

    // Print spaces to center the message
    for (int i = 0; i < spacesBeforeText; ++i)
    {
        cout << " ";  // Spaces for centering
    }

    // Print the message in bright green with a delay for each character (typing effect)
    cout << brightGreen;
    for (char c : message)
    {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delayMs)); // Adjustable delay
    }
    cout << reset << endl;

    // Print a small space after the message
    cout << endl;

    // Print the bottom border in bright red
    for (int i = 0; i < screenWidth; ++i)
    {
        cout << brightRed << "#";  // Border symbol in bright red
    }
    cout << reset << endl;
}

// Helper function to check if the bin is full and suggest nearby bins if full
void checkBinFull(float remainingCapacity, int wasteType) {
    if (remainingCapacity == 0) {
        srand(time(0));  // Seed the random number generator

        // Array for location names (Colony 1 to Colony 9)
        string locations[] = {"Colony 1", "Colony 2", "Colony 3", "Colony 4", "Colony 5",
                               "Colony 6", "Colony 7", "Colony 8", "Colony 9"};

        // Randomly select a location between 0 and 8 (for Colonies 1 to 9)
        int randomLocationIndex = rand() % 9;
        string randomLocation = locations[randomLocationIndex];

        // Randomly generate a distance between 1 and 10 km
        int randomDistance = rand() % 10 + 1;

        // Display message depending on waste type
        switch (wasteType) {
            case 1:
                cout << "nOrganic bin is full! Cannot add more waste." << endl;
                cout << "Nearest empty bin: " << randomLocation << " at " << randomDistance << " km away." << endl;
                break;
            case 2:
                cout << "\nInorganic bin is full! Cannot add more waste." << endl;
                cout << "Nearest empty bin: " << randomLocation << " at " << randomDistance << " km away." << endl;
                break;
            case 3:
                cout << "\nElectronic bin is full! Cannot add more waste." << endl;
                cout << "Nearest empty bin: " << randomLocation << " at " << randomDistance << " km away." << endl;
                break;
            case 4:
                cout << "\nUnknown bin is full! Cannot add more waste." << endl;
                cout << "Nearest empty bin: " << randomLocation << " at " << randomDistance << " km away." << endl;
                break;
            default:
                cout << "\nInvalid waste type." << endl;
                break;
        }
    }
}
void displayFinalBinWeights(float organicWeight, float inorganicWeight, float electronicWeight, float unknownWeight,
                             float organicRemaining, float inorganicRemaining, float electronicRemaining, float unknownRemaining) {
    cout << "_______________________________________________________________" << endl;
    cout << "| Waste Type   | Current Weight (kg) | Remaining Capacity (kg) |" << endl;
    cout << "|______________|_____________________|_________________________|" << endl;

    cout << "| Organic      | " << setw(18) << left << organicWeight
         << "  | " << setw(23) << left << organicRemaining << " |" << endl;
    cout << "| Inorganic    | " << setw(18) << left << inorganicWeight
         << "  | " << setw(23) << left << inorganicRemaining << " |" << endl;
    cout << "| Electronic   | " << setw(18) << left << electronicWeight
         << "  | " << setw(23) << left << electronicRemaining << " |" << endl;
    cout << "| Unknown      | " << setw(18) << left << unknownWeight
         << "  | " << setw(23) << left << unknownRemaining << " |" << endl;

    cout << "|______________|_____________________|_________________________|" << endl;

    // Call to check if any bin is full and display nearest bin
    checkBinFull(organicRemaining, 1);      // Check Organic bin
    checkBinFull(inorganicRemaining, 2);    // Check Inorganic bin
    checkBinFull(electronicRemaining, 3);   // Check Electronic bin
    checkBinFull(unknownRemaining, 4);      // Check Unknown bin
}

void printCentered(const string& text, int width = 80, int delayMs = 100) {
    const string green = "\033[32m";  // Green color for text
    const string reset = "\033[0m";   // Reset color to default

    // Ensure the text is not wider than the output width
    if (text.length() >= width) {
        // If the text is too long, just print it without centering
        for (char c : text) {
            cout << green << c << reset;
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(delayMs)); // Typing effect delay
        }
        cout << endl;
    } else {
        // Calculate the padding needed to center the text
        int padding = (width - text.length()) / 2;

        // Print the leading spaces (for centering) before the text
        for (int i = 0; i < padding; ++i) {
            cout << " ";  // Spaces for centering
        }

        // Print the centered text with a typing effect (letter by letter)
        for (char c : text) {
            cout << green << c << reset;
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(delayMs)); // Typing effect delay
        }
        cout << endl;
    }
}
// Function to get the current system time as a string
string getCurrentTime() {
    time_t now = time(0);  // Get the current time
    tm* localtm = localtime(&now);  // Convert to local time
    char buffer[80];

    // Format the time into a readable string
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtm);
    return string(buffer);
}

// Function to save user data and system time to a file
void saveUserData(int userID, const string& userName, const string& currentTime) {
    // Open the file (append mode) to save the information
    ofstream outFile;
    outFile.open("user_data.txt", ios::app);  // Open the file in append mode

    if (!outFile) {
        // Check if the file was opened successfully
        cout << "Error opening the file!" << endl;
        return;  // Exit the function if file couldn't be opened
    }

    // Write the user data to the file
    outFile << "User ID: " << to_string(userID) << endl;
    outFile << "User Name: " << userName << endl;
    outFile << "Time of Entry: " << currentTime << endl;
    outFile << "------------------------------------" << endl;

    cout << "User data saved successfully!" << endl;

    // Close the file
    outFile.close();
}
// Function to print text in yellow with a delay, letter by letter
void printInYellowLetterByLetter(const string& text, int delayMs = 100) {
    const string yellow = "\033[33m";  // Yellow color for text
    const string reset = "\033[0m";    // Reset color to default

    // Loop through each character in the text and print it with a delay
    for (char c : text) {
        cout << yellow << c << reset;  // Print character in yellow
        cout.flush();                  // Ensure immediate output
        this_thread::sleep_for(chrono::milliseconds(delayMs)); // Delay for typing effect
    }
    cout << endl; // Move to the next line after finishing the message
}

// Main function
int main()
{
    int ch;

    while (true)
    {
    cout << "************************************************************************************************************************************************************" << endl;
    cout << "*                                                                                                                                                          *" << endl;
    // Center "SMART CITY WASTE MANAGEMENT"
    printWithDelay("*\t\t\t\t\t\t\t\tSMART CITY WASTE MANAGEMENT\t\t\t\t\t\t\t\t\t\t\t\t\t*", 100);
    cout << "*                                                                                                                                                          *" << endl;
    cout << "************************************************************************************************************************************************************" << endl;
    cout << endl;

 // ANSI escape codes for colors
    const string brightRed = "\033[91m";    // Red
    const string brightGreen = "\033[92m";  // Green
    const string brightCyan = "\033[96m"; // cyan
    const string brightBlue = "\033[94m";   // Blue
    const string brightmMagenta = "\033[95m"; // magenta
    const string brightYellow = "\033[93m";
    const string green = "\033[32m";
    const string blue = "\033[34m";
    const string reset = "\033[0m";   // Reset color to default


    // Display options with different colors
    cout <<brightRed<< "1 - BIN MONITORING SYSTEM" << reset << endl;
    cout << brightCyan << "2 - DATA COLLECTION" << reset << endl;
    cout << brightGreen<< "3 - GARBAGE TRUCK MANAGEMENT SYSTEM" << reset << endl;
    cout << brightBlue << "4 - WASTE SORTING MANAGEMENT" << reset << endl;
    cout <<brightmMagenta << "5 - Exit" << reset << endl;

    cout << endl;

    cout <<brightYellow<< "ENTER YOUR CHOICE: "<<reset<<endl;
        cin >> ch;
        if (ch==1)
    {
            vector<WasteDetails> wasteHistory;
            vector<UserSummary> userSummaries;   // To store all waste data for a user
    map<int, string> userDatabase;
    map<int, int> userRewards;  // Store user-specific reward points (userID -> reward points)
    int userID, dayOfWeek;

    while (true)
        {
        activateSleepMode();  // Simulate sleep mode activation
     cout<<blue << "\n******************************************************************************" <<reset<< endl;

    printCentered("WELCOME TO THE WASTE MANAGEMENT SYSTEM!");

        cout <<blue<< "******************************************************************************\n" <<reset<< endl;
        cout << "Enter your User ID: ";
        cin >> userID;
        cin.ignore();
        cout << "Enter your name: ";
        string userName;
        getline(cin, userName);
    string currentTime = getCurrentTime();

    // Call the function to save user data
    saveUserData(userID, userName, currentTime);

        cout << "Hello, " << userName << "!" << endl;
        cout << "Please enter the current day of the week (1-7, where 7th is Sunday): ";
        cin >> dayOfWeek;

        remindWasteCollection(dayOfWeek);

        float totalUserWaste = 0;
        char continueInput;
        do {
            classifyAndTrackWaste(wasteHistory, totalUserWaste, userSummaries, userID, userName, dayOfWeek);
            cout << "\nWould you like to dispose more waste? (1 for Yes, 0 for No): ";
            cin >> continueInput;
        }
        while (continueInput == '1');

        int rewardPoints = calculateRewards(totalUserWaste);
        userRewards[userID] = rewardPoints;
         for (auto& summary : userSummaries) {
            if (summary.userID == userID ) {
                summary.rewardPoints = rewardPoints;
                break;
            }
         }

        cout << "\nTotal Waste Contribution: " << totalUserWaste << " kg" << endl;
        cout << "Reward Points Earned: " << rewardPoints << "\n" << endl;
        displayAllUsersData(userSummaries);
        detectTampering();

        cout << "\nFinal bin weights and remaining capacities:" << endl;
        displayFinalBinWeights(
    currentOrganicWeight, currentInorganicWeight, currentElectronicWeight, currentUnknownWeight,
    MAX_ORGANIC_CAPACITY - currentOrganicWeight,
    MAX_INORGANIC_CAPACITY - currentInorganicWeight,
    MAX_ELECTRONIC_CAPACITY - currentElectronicWeight,
    MAX_UNKNOWN_CAPACITY - currentUnknownWeight
);


       cout<< "\nYipee your waste has been successfully dumped!!" ;
       cout<<"\nWould you like to continue for another user? (1 for Yes, 0 for No): ";
        cin >> continueInput;

        if (continueInput == '0')
        {
             printInYellowLetterByLetter("\nThank you for using the Waste Classification System!", 100);
            break;  // Exit the program if no more users

        }

        wasteHistory.clear();
    }
 }

       else if (ch==2)
    {
          unordered_map<string, vector<Vehicle>> areaVehicles;
    vector<string> areas = {"Colony-1", "Colony-2", "Colony-3", "Colony-4", "Colony-5", "Colony-6", "Colony-7", "Colony-8", "Colony-9"};


    // Sample data with Indian names for demonstration
areaVehicles["Colony-1"].push_back({"U001", "Rajesh", "Kumar", 0, 0, 0, "Truck1"});
areaVehicles["Colony-2"].push_back({"U002", "Suresh", "Rao", 0, 0, 0, "Truck2"});
areaVehicles["Colony-3"].push_back({"U003", "Vikram", "Yadav", 0, 0, 0, "Truck3"});
areaVehicles["Colony-4"].push_back({"U004", "Arjun", "Malshet", 0, 0, 0, "Truck4"});
areaVehicles["Colony-5"].push_back({"U005", "Manoj", "Kumar", 0, 0, 0, "Truck5"});
areaVehicles["Colony-6"].push_back({"U006", "Baman", "Kandur", 0, 0, 0, "Truck6"});
areaVehicles["Colony-7"].push_back({"U007", "Mangesh", "Gavade", 0, 0, 0, "Truck7"});
areaVehicles["Colony-8"].push_back({"U008", "Amit", "Jadhav", 0, 0, 0, "Truck8"});
areaVehicles["Colony-9"].push_back({"U009", "Ramesh", "Chadarmod", 0, 0, 0, "Truck9"});


    ofstream outFile("area_vehicles_data.txt");
    if (!outFile)
    {
        cerr << "Error: Could not create the file." << endl;
        return 1;
    }

outFile << "Colony-1 U001 Rajesh Kumar 0 0 0 Truck1\n";
outFile << "Colony-2 U002 Suresh Rao 0 0 0 Truck2\n";
outFile << "Colony-3 U003 Vikram Yadav 0 0 0 Truck3\n";
outFile << "Colony-4 U004 Arjun Malshet 0 0 0 Truck4\n";
outFile << "Colony-5 U005 Manoj Kumar 0 0 0 Truck5\n";
outFile << "Colony-6 U006 Baman Kandur 0 0 0 Truck6\n";
outFile << "Colony-7 U007 Mangesh Gavade 0 0 0 Truck7\n";
outFile << "Colony-8 U008 Amit Jadhav 0 0 0 Truck8\n";
outFile << "Colony-9 U009 Ramesh Chadarmod 0 0 0 Truck9\n";


    outFile.close();

    if (login())
    {
        displayMenu(areaVehicles, areas);
    }
    else
    {
        cout << "Access denied.\n";
    }
       }
       else if (ch == 3)
        {
            int choice;
              while (true)
        {
        displayMainMenu();
        cin >> choice;

        if (choice == 1)
        {
             if (authenticate2())
        {
               printWithDelay5("\n\t\t*** Garbage Collection Records ***\n", 100, 80);
            while (true)
            {
                delayedPrint("\n1. Generate Monthly Garbage Records\n", 1);
                delayedPrint("2. Display File Contents\n", 1);
                delayedPrint("3. Display Average Garbage Collected per Day\n", 1);
                delayedPrint("4. Generate Colony Reports\n", 1);
                delayedPrint("5. Save Monthly Records\n", 1);
                delayedPrint("6. Load Monthly Records\n", 1);
                delayedPrint("7. Clear Monthly Records\n", 1);
                delayedPrint("8. Exit to Main Menu\n", 1);
                delayedPrint("Enter your choice: ", 1);

                int subChoice;
                cin >> subChoice;

                if (subChoice == 1)
                {
                    generateMonthlyGarbageRecords();
                }
                else if (subChoice == 2)
                {
                    displayFileContents();
                }
                else if (subChoice == 3)
                {
                    displayAverageGarbagePerDay();
                }
                else if (subChoice == 4)
                {
                    generateColonyReports();
                }
                else if (subChoice == 5)
                {
                    saveMonthlyRecords();
                }
                else if (subChoice == 6)
                {
                    loadMonthlyRecords();
                }
                else if (subChoice == 7)
                {
                    clearMonthlyRecords();
                } else if (subChoice == 8)
                {
                    break;
                }
                else
                {
                    delayedPrint("Invalid choice. Please try again.", 1);
                }
            }
             }
        }
        else if (choice == 2)
        {
            garbageTruckManagement();
        }
        else if (choice == 3)
        {
            const string brightYellow = "\033[1;33m";  // Bright Yellow color
    const string reset = "\033[0m";            // Reset color to default

    // Call delayedPrint to print in bright yellow
    delayedPrint(brightYellow + "\nEXITING FROM GARBAGE TRUCK MANAGEMENT.......\n" + reset, 1);
            break;
        }
        else
        {
            delayedPrint("Invalid choice. Please try again.", 1);
        }
    }
}
       else if (ch == 4)
        {
            displayWasteMenu();
        }
    else
    {
       string quote = "\n\n***Waste is a resource in the wrong place. Let s work together to recycle, reduce, and reuse for a cleaner, greener future.***";
    printWithDelay2(quote); // Call the function to print the quote with delay
    return 0;
    }
}
    return 0;
}

