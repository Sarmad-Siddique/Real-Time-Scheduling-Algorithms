//21L-5225

#include <iostream>
#include <fstream>
using namespace std;

class process
{
    int p_id;
    int time_period;
    int exec_time;
    int priority;
    int exec_time_left;

public:

    process()
    {
        p_id = -1;
        time_period = 0;
        exec_time = 0;
        priority = 0;
        exec_time_left = 0;
    }

    void init(int id, int timeP, int execT)
    {
        p_id = id;
        time_period = timeP;
        exec_time = execT;
        priority = 0;
        exec_time_left = 0;

        return;
    }

    void print()
    {
        cout << "Process ID: " << p_id << endl;
        cout << "Time Period: " << time_period << endl;
        cout << "Execution Time: " << exec_time << endl;
        cout << "Priority: " << priority << endl;
        cout << "Execution Time Left: " << exec_time_left << endl;

        return;
    }

    void set_prio_ratemono()
    {
        priority = time_period;

        return;
    }

    void set_prio_earliestdeadline(int count)
    {
        priority = time_period - (count % time_period);

        return;
    }

    void set_prio(int prio)
    {
        priority = prio;

        return;
    }

    int get_pid()
    {
        return p_id;
    }

    int get_prio()
    {
        return priority;
    }

    int get_execution_time_left()
    {
        return exec_time_left;
    }

    int get_execution_time()
    {
        return exec_time;
    }

    void set_execution_time_left()
    {
        exec_time_left = exec_time;

        return;
    }

    void set_execution_time_left(int time)
    {
        exec_time_left = time;

        return;
    }

    void dec_execution_time_left()
    {
        exec_time_left--;

        return;
    }

    int get_time_period()
    {
        return time_period;
    }

};

bool Init(process* &process_array, int& array_size);
void Print_array(process* process_array, int array_size);
void Schedulable(process* process_array, int array_size);

void RateMonotonic(process* process_array, int array_size);
bool CheckNewProcessRateMono(process* &process_array, int array_size, int count);

void EarliestDeadlineFirst(process* process_array, int array_size);
bool CheckNewProcessEDF(process*& process_array, int array_size, int count);


void ResetValues(process* process_array, int array_size);
bool continuePrompt(int count);
void PrintExecution(process* process_array, int exec_index, int last_index, int count);
int getLowestPrioIndex(process* process_array, int array_size);




int main()
{
    process* process_array = nullptr; 
    int array_size = 0;

    if (Init(process_array, array_size) != true)
    {
        return 0;
    }

    Schedulable(process_array, array_size);

    RateMonotonic(process_array, array_size);

    ResetValues(process_array, array_size);

    EarliestDeadlineFirst(process_array, array_size);

    //Print_array(process_array, array_size);

    return 0;
}




bool Init(process* &process_array, int& array_size)
{
    string filename;
    cout << "Enter file name where process info is stored (e.g processInfo.txt): ";
    cin >> filename;

    ifstream fin;
    fin.open(filename);
    
    if (!fin)
    {
        cout << "Error opening file" << endl;
        return false;
    }

    fin >> array_size;
    process_array = new process[array_size];

    int id;
    int timeP;
    int execT;

    for (int i = 0; i < array_size; i++)
    {
        fin >> id;
        fin >> timeP;
        fin >> execT;
        process_array[i].init(id, timeP, execT);
    }

    fin.close();

    return true;
}

void RateMonotonic(process* process_array, int array_size)
{
    bool status = true;
    int count = 0;
    int last_index = -1;

    cout << "\n\n\nRATE MONOTONIC\n\n";

    for (int i = 0; i < array_size; i++)
    {
        process_array[i].set_prio_ratemono();
    }

    while (status == true)
    {
        if ((status = CheckNewProcessRateMono(process_array, array_size, count)) == false)
        {
            break;
        }

        int exec_index = getLowestPrioIndex(process_array, array_size);

        if (exec_index == -1)
        {
            if (last_index != -1)
            {
                cout << " ------      P_ID: " << process_array[last_index].get_pid() << " ends executing" << endl;
            }

            cout << "No process available to execute" << endl;
            count++;

            last_index = -1;
            continue;
        }

        process_array[exec_index].dec_execution_time_left();

        if (process_array[exec_index].get_execution_time_left() == 0)
        {
            process_array[exec_index].set_prio(INT_MAX);
        }

        PrintExecution(process_array, exec_index, last_index, count);


        status = continuePrompt(count);

        count++;
        last_index = exec_index;
    }
}


//Basically checks whether a new process starts on this cycle, if it does it checks if it was finished, if yes then it sets the prio, if no then it aborts the program
bool CheckNewProcessRateMono(process* &process_array, int array_size, int count)
{
    for (int i = 0; i < array_size; i++)
    {
        if (count % process_array[i].get_time_period()  == 0)
        {
            if (process_array[i].get_execution_time_left() == 0)
            {
                process_array[i].set_execution_time_left();
                process_array[i].set_prio_ratemono();
            }
            else
            {
                cout << "\n!!!!\nProcess ID: " << process_array[i].get_pid() << " was not completed within time!!!\n!!!!\n" << endl;
                return false;
            }
        }
    }

    return true;
}


void EarliestDeadlineFirst(process* process_array, int array_size)
{
    bool status = true;
    int count = 0;
    int last_index = -1;

    cout << "\n\n\nEARLIEST DEADLINE FIRST\n\n";

    while (status == true)
    {

        for (int i = 0; i < array_size; i++)
        {
            if (process_array[i].get_execution_time_left() != 0)
            {
                process_array[i].set_prio_earliestdeadline(count);
            }
            
        }

        if ((status = CheckNewProcessEDF(process_array, array_size, count)) == false)
        {
            break;
        }

        int exec_index = getLowestPrioIndex(process_array, array_size);

        if (exec_index == -1)
        {
            if (last_index != -1)
            {
                cout << " ------      P_ID: " << process_array[last_index].get_pid() << " ends executing" << endl;
            }

            cout << "No process available to execute" << endl;
            count++;

            last_index = -1;
            continue;
        }

        process_array[exec_index].dec_execution_time_left();

        if (process_array[exec_index].get_execution_time_left() == 0)
        {
            process_array[exec_index].set_prio(INT_MAX);
        }

        PrintExecution(process_array, exec_index, last_index, count);


        status = continuePrompt(count);

        count++;
        last_index = exec_index;
    }
}

bool CheckNewProcessEDF(process*& process_array, int array_size, int count)
{
    for (int i = 0; i < array_size; i++)
    {
        if (count % process_array[i].get_time_period() == 0)
        {
            if (process_array[i].get_execution_time_left() == 0)
            {
                process_array[i].set_execution_time_left();
                process_array[i].set_prio_earliestdeadline(count);
            }
            else
            {
                cout << "\n!!!!\nProcess ID: " << process_array[i].get_pid() << " was not completed within time!!!\n!!!!\n" << endl;
                return false;
            }
        }
    }

    return true;
}

int getLowestPrioIndex(process* process_array, int array_size)
{
    int lowest_prio = INT_MAX;
    int index = -1;

    for (int i = 0; i < array_size; i++)
    {
        if (process_array[i].get_prio() < lowest_prio)
        {
            lowest_prio = process_array[i].get_prio();
            index = i;
        }
    }

    return index;
}

void ResetValues(process* process_array, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        process_array[i].set_execution_time_left(0);
        process_array[i].set_prio(0);
    }

    return;
}

void PrintExecution(process* process_array, int exec_index, int last_index, int count)
{
    if (exec_index == last_index)
    {
        cout << "|      |" << count << endl;
    }
    else if (last_index == -1)
    {
        cout << " ------ " << endl;
        cout << "|      |" << count << "    P_ID: " << process_array[exec_index].get_pid() << " starts executing" << endl;
    }
    else
    {
        cout << " ------      P_ID: " << process_array[last_index].get_pid() << " ends executing" << endl;
        cout << " ------ " << endl;
        cout << "|      |" << count << "    P_ID: " << process_array[exec_index].get_pid() << " starts executing" << endl;
    }

    return;
}

bool continuePrompt(int count)
{
    if (count % 100 == 0 && count != 0)
    {
        char choice;
        cout << "\n\n 100 cycles have passed, press Y to continue or press any other key to stop: ";
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

void Schedulable(process* process_array, int array_size)
{
    double cpu_utilization = 0;
    for (int i = 0; i < array_size; i++)
    {
        cpu_utilization = cpu_utilization + ((float)process_array[i].get_execution_time() / (float)process_array[i].get_time_period());
    }

    cout << "\nIdeal CPU Utilization would be " << cpu_utilization << endl;
    if (cpu_utilization > 1)
    {
        cout << "Hence, It is not schedulable under any circumstances, lets prove it." << endl;
    }
    else
    {
        cout << "Hence, It is schedulable" << endl;
    }

    return;
}


void Print_array(process* process_array, int array_size)
{
    cout << "Total Processes: " << array_size << endl << endl;
    for (int i = 0; i < array_size; i++)
    {
        process_array[i].print();
        cout << endl;
    }

    return;
}