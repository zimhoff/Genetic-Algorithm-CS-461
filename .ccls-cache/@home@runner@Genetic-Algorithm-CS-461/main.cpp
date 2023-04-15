
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <string>

#define generations 100
#define population 500
#define num_courses 11
#define num_facilitator 10
#define num_times 6
#define num_rooms 9

using namespace std;

// int getLenghtOfArray(string arr[100]) {
//     for (int i : arr) {
//     num ++;
//   }
//   cout << num << "\n";
//   return num;
// }



int sizeOfIntArray(int arr[10]) {
    int arr3[10];
    int size = 0;
    for (int i = 0; i < 10; i++) {
        arr3[i] = arr[i];
        // cout<< arr3[i] <<endl;    
    }

    for (int i : arr3)
        size++;
    return size;
    // cout << "size: " << size;
}


// int sizeOfStrArray(string arr[11]){
//     int arr3[11];
//     int size = 0;
//     for(int i=0; i<11; i++){
//         strcpy(arr3[i], arr[i]);
//         // cout<< arr3[i] <<endl;    
//     }

//     for(int i : arr3)
//         size++;
//   return size;     
//     // cout << "size: " << size;
// } NOT WORKING AT THE MOMENT


bool IsNotEmpty(const string& str)
{
    return !str.empty();
}

void heapify(int arr[], int n, int i)
{
    int smallest = i; // Initialize smallest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is smaller than root
    if (l < n && arr[l] < arr[smallest])
        smallest = l;

    // If right child is smaller than smallest so far
    if (r < n && arr[r] < arr[smallest])
        smallest = r;

    // If smallest is not root
    if (smallest != i) {
        swap(arr[i], arr[smallest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, smallest);
    }
}

// main function to do heap sort
void heapSort(int arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);

        // call min heapify on the reduced heap
        heapify(arr, i, 0);
    }
}


void GeneticAlgorithm(string time_table[population][num_courses][3], string courses_names[num_courses], string room_names[num_rooms], int room_capacity[num_rooms], int courses_expected_enrollment[num_courses], string courses_preferred_facilitator[num_courses][10], string courses_other_facilitator[num_courses][10], string facilitator[num_facilitator], int time_slots[num_times]) {


    // string time_table2[population][num_courses][3];

    float fitness = 0.0;
    float scores[population];

    string class_time_pairs[num_courses * 2];
    int class_time_index = 0;

    string ins_time_pairs[num_facilitator * 2];
    int ins_time_index = 0;

    int classes_per_facilitator[num_facilitator];
    for (int i = 0; i < num_facilitator; i++)
        classes_per_facilitator[i] = 0;

    string room_per_course[num_courses];
    int time_slot_per_course[num_courses];

    for (int gen = 0; gen < generations; gen++) {
        fitness = 0.0;
        class_time_index = 0;
        ins_time_index = 0;
        for (int i = 0; i < num_facilitator; i++)
            classes_per_facilitator[i] = 0;


        cout << "////////////////////////////////" << endl;
        cout << "Generation = " << gen + 1 << endl;


        for (int pop = 0; pop < population; pop++) {
            fitness = 0;
            // cout << "\n\n\nPopulation = " << pop <<endl;
            class_time_index = 0;
            ins_time_index = 0;

            for (int ele = 0; ele < num_courses; ele++) {
                // cout << "\nCourse = " << courses_names[ele] <<endl;

                // getting class data
                string room = time_table[pop][ele][0];
                string time_slot = time_table[pop][ele][1];
                string instructor = time_table[pop][ele][2];

                // Checking 2 classes in 1 room at 1 time
                class_time_pairs[class_time_index] = room;
                class_time_index++;
                class_time_pairs[class_time_index] = time_slot;
                class_time_index++;

                // Checking 2 classes 1 teacher at 1 time
                ins_time_pairs[ins_time_index] = instructor;
                ins_time_index++;
                ins_time_pairs[ins_time_index] = time_slot;
                ins_time_index++;

                // Room capacity and enrollment size
                int room_index = 0;
                for (int i = 0; i < num_rooms; i++) {
                    if (room_names[i].compare(room) == 0)
                        break;
                    room_index++;
                }
                int expected_enrollment = courses_expected_enrollment[ele];
                int capacity = room_capacity[room_index];
                if (capacity < expected_enrollment) {
                    fitness = fitness - 0.5;
                    // cout << "capacity < expexted_enrollment" << endl;
                }
                else if (capacity > 6 * expected_enrollment) {
                    fitness = fitness - 0.4;
                    // cout << "capacity > 6*expexted_enrollment" << endl;
                }
                else if (capacity > 3 * expected_enrollment) {
                    fitness = fitness - 0.2;
                    // cout << "capacity > 3*expexted_enrollment" << endl;
                }
                else {
                    fitness = fitness + 0.3;
                    // cout << "Capacity Good" << endl;
                }


                // Check if course is taught by prefered
                bool prefered = false;
                int num_prefered = 0;

                for (int i = 0; i < 6; i++)
                {
                    if (courses_preferred_facilitator[ele][i].empty())
                        break;
                    num_prefered++;
                }


                for (int i = 0; i < num_prefered; i++) {
                    if (courses_preferred_facilitator[ele][i].compare(instructor) == 0) {
                        fitness += 0.5;
                        prefered = true;
                        // cout << "Taught by Prefered Teacher" << endl;
                        break;
                    }
                }

                // Check if course is taught by other
                int num_other = 0;
                bool other = false;

                for (int i = 0; i < 6; i++)
                {
                    if (courses_other_facilitator[ele][i].empty())
                        break;
                    num_prefered++;
                }


                for (int i = 0; i < num_other; i++) {
                    if (courses_other_facilitator[ele][i].compare(instructor) == 0) {
                        fitness += 0.2;
                        other = true;
                        // cout << "Taught by Other Teacher" << endl;
                        break;
                    }
                }


                // check if not by pref and other

                if (!prefered && !other) {
                    fitness -= 0.1;
                    // cout<<"Not Prefered Not other" << endl;
                }

                // Classes per teacher
                int facilitator_index = 0;
                for (int i = 0; i < num_facilitator; i++) {
                    if (facilitator[i].compare(instructor) == 0)
                        break;
                    facilitator_index++;
                }
                classes_per_facilitator[facilitator_index] = classes_per_facilitator[facilitator_index] + 1;

                // Courses are apart or together
                time_slot_per_course[ele] = stoi(time_slot);


                // Getting list of room of each course 
                room_per_course[ele] = room;

                // printing
                // cout << "room = " << room << endl;
                // cout << "time_slot = " << time_slot << endl;
                // cout << "facilitator = " << facilitator << endl;
                // cout << "capacity = " << capacity << endl;
                // cout << "Enrollment = " << expected_enrollment << endl;
                // cout << "Fitness : " << fitness <<endl;
            }

            //  This is to check if 2 classes have same room at same time
            // cout << "\n\nClass Time Pairs: " << endl;

            int class_time_douplicates_found = 0;


            for (int i = 0; i < num_courses * 2 - 2; i += 2) {
                if (class_time_pairs[i].empty()) {
                    break;
                    cout << "Breaks Applied" << endl;
                }
                // cout << class_time_pairs[i] << endl;
                // cout << class_time_pairs[i+1] << endl;
                for (int j = i + 2; j < num_courses * 2 - 1; j += 2) {
                    if (class_time_pairs[i].compare(class_time_pairs[j]) == 0 && class_time_pairs[i + 1].compare(class_time_pairs[j + 1]) == 0)
                        class_time_douplicates_found++;
                }
            }

            // Facilitator load checking
            int num_fours = 0;
            for (int i = 0; i < num_facilitator; i++) {
                if (i == 7)
                    continue;
                if (classes_per_facilitator[i] >= 4)
                    fitness -= 0.5;
                else if (classes_per_facilitator[i] == 1 || classes_per_facilitator[i] == 2)
                    fitness -= 0.4;
            }

            // Course Specific
            if (abs(time_slot_per_course[0] - time_slot_per_course[1]) > 4)
                fitness += 0.5;

            if (time_slot_per_course[0] == time_slot_per_course[1])
                fitness -= 0.5;

            if (abs(time_slot_per_course[2] - time_slot_per_course[3]) > 4)
                fitness += 0.5;

            if (time_slot_per_course[2] == time_slot_per_course[3])
                fitness -= 0.5;

            bool op1 = abs(time_slot_per_course[0] - time_slot_per_course[2]) == 1;
            bool op2 = abs(time_slot_per_course[1] - time_slot_per_course[2]) == 1;
            bool op3 = abs(time_slot_per_course[0] - time_slot_per_course[3]) == 1;
            bool op4 = abs(time_slot_per_course[1] - time_slot_per_course[3]) == 1;

            // if SLA 101 and SLA 191 sections are Consective and are in same department  

            if (op1 || op2 || op3 || op4)
            {
                // cout << "Consective classes";
                fitness += 0.5;
                if (!(op1 && (room_per_course[0][0] == room_per_course[2][0] == 'B' || room_per_course[0][0] == room_per_course[2][0] == 'K')))
                    fitness -= 0.4;

                if (!(op2 && (room_per_course[1][0] == room_per_course[2][0] == 'B' || room_per_course[1][0] == room_per_course[2][0] == 'K')))
                    fitness -= 0.4;

                if (!(op3 && (room_per_course[0][0] == room_per_course[3][0] == 'B' || room_per_course[0][0] == room_per_course[3][0] == 'K')))
                    fitness -= 0.4;

                if (!(op4 && (room_per_course[1][0] == room_per_course[3][0] == 'B' || room_per_course[1][0] == room_per_course[3][0] == 'K')))
                    fitness -= 0.4;

            }

            if (abs(time_slot_per_course[0] - time_slot_per_course[2]) == 1 || abs(time_slot_per_course[0] - time_slot_per_course[3]) == 1 || abs(time_slot_per_course[1] - time_slot_per_course[2]) == 2 || abs(time_slot_per_course[1] - time_slot_per_course[3]) == 2)
            {
                // cout << "Consective classes";
                fitness += 0.25;
            }

            if (abs(time_slot_per_course[0] - time_slot_per_course[2]) == 0 || abs(time_slot_per_course[0] - time_slot_per_course[3]) == 0 || abs(time_slot_per_course[1] - time_slot_per_course[2]) == 0 || abs(time_slot_per_course[1] - time_slot_per_course[3]) == 0)
            {
                // cout << "Consective classes";
                fitness -= 0.25;
            }

            // Facilitator same time 2 classes check
            int ins_time_douplicates_found = 0;

            for (int i = 0; i < num_facilitator * 2 - 2; i += 2) {
                // if(ins_time_pairs[i].empty()){
                //     break;
                //     cout << "Breaks Applied" << endl;
                // }
                // cout << ins_time_pairs[i] << endl;
                // cout << ins_time_pairs[i+1] << endl;
                for (int j = i + 2; j < num_facilitator * 2 - 1; j += 2) {
                    if (ins_time_pairs[i].compare(ins_time_pairs[j]) == 0 && ins_time_pairs[i + 1].compare(ins_time_pairs[j + 1]) == 0)
                        ins_time_douplicates_found++;
                }
            }

            if (ins_time_douplicates_found == 0)
            {
                fitness += 0.2;
                // cout<<"No Teacher has 2 classes at 1 time";
            }
            else
            {
                fitness -= 0.2;
                // cout<<"1 or more teachers has 2 classes at 1 time";
            }

            // cout << "\nFitness without classtime dups " << fitness << endl;
            fitness = fitness - 0.5 * class_time_douplicates_found;
            // cout << "douplicates_found " << class_time_douplicates_found << endl;

            // cout << "\nFitness Score for this chromosome " << fitness << endl;
            // cout<< "Fitness " << fitness << endl;
            scores[pop] = fitness;
        }

        // return scores;
        // cout << "Scores " << endl;
        // for(int i=0; i<population; i++)
        // {
        //     cout << scores[i] << endl;
        // }

        int largest_index = 0;
        float largest = scores[0];
        for (int i = 0; i < population; i++) {
            if (scores[i] > largest) {
                largest_index = i;
                largest = scores[i];
            }
        }

        int sec_largest_index = 0;
        float sec_largest = scores[0];
        for (int i = 0; i < population; i++) {
            if (scores[i] > sec_largest && scores[i] != largest) {
                sec_largest_index = i;
                sec_largest = scores[i];
            }
        }

        // cout << "\n\n\n\n Largest = " << largest << endl;
        // cout << "\n Sec Largest = " << sec_largest << "\n\n\n\n";

        string parent1[num_courses][3];
        for (int i = 0; i < num_courses; i++)
            for (int j = 0; j < 3; j++)
                parent1[i][j] = time_table[largest_index][i][j];


        string parent2[num_courses][3];
        for (int i = 0; i < num_courses; i++)
            for (int j = 0; j < 3; j++)
                parent2[i][j] = time_table[sec_largest_index][i][j];



        // cout << "\n\nbest time_table\n" <<endl;

        // string pararent1[num_courses][3];
        for (int i = 0; i < num_courses; i++)
            for (int j = 0; j < 3; j++)
                // cout << parent1[i][j] << endl;
        // cout << "Best Score is " << largest << endl;

        // cout << "\n\nSecond Best Time Table is \n" << endl;

                for (int i = 0; i < num_courses; i++)
                    for (int j = 0; j < 3; j++)
                        // cout << parent2[i][j] << endl;
                // cout << "2nd Best Score is " << sec_largest << endl;


                // Mutate to get next generation
                        srand(time(0));
        // set time_table[0] to best and time_table[1] to 2nd best

        string time_table_random[num_courses][3];



        for (int sla = 0; sla < num_courses; sla++) {
            time_table_random[sla][0] = room_names[rand() % num_rooms];
            time_table_random[sla][1] = to_string(time_slots[rand() % num_times]);
            time_table_random[sla][2] = facilitator[rand() % num_facilitator];
        }



        for (int i = 0; i < num_courses; i++)
            for (int j = 0; j < 3; j++) {
                time_table[0][i][j] = parent1[i][j];
                time_table[1][i][j] = parent2[i][j];
            }

        string child1[num_courses][3];
        string child2[num_courses][3];
        for (int pop = 2; pop < population - 1; pop += 2) {
            int index_1 = rand() % num_courses;
            int index_2 = rand() % 3;
            for (int k = 0; k < num_courses; k++) {
                for (int t = 0; t < 3; t++) {
                    child1[k][t] = parent1[k][t];
                    child2[k][t] = parent2[k][t];
                }
            }

            for (int sla = 0; sla < num_courses; sla++) {
                time_table_random[sla][0] = room_names[rand() % num_rooms];
                time_table_random[sla][1] = to_string(time_slots[rand() % num_times]);
                time_table_random[sla][2] = facilitator[rand() % num_facilitator];
            }

            child1[index_1][index_2] = time_table_random[index_1][index_2];


            for (int sla = 0; sla < num_courses; sla++) {
                time_table_random[sla][0] = room_names[rand() % num_rooms];
                time_table_random[sla][1] = to_string(time_slots[rand() % num_times]);
                time_table_random[sla][2] = facilitator[rand() % num_facilitator];
            }

            index_1 = rand() % num_courses;
            index_2 = rand() % 3;

            child2[index_1][index_2] = parent1[index_1][index_2];

            for (int i = 0; i < num_courses; i++)
                for (int j = 0; j < 3; j++) {
                    time_table[pop][i][j] = child1[i][j];
                    time_table[pop + 1][i][j] = child2[i][j];
                }
        }



        // cout << "\n\nNew Time Table \n\n";

        // for(int pop=0; pop<population; pop++)
        //     for(int i=0; i<num_courses; i++)
        //         for(int j=0; j<3; j++)
        //             cout << time_table[pop][i][j] << endl;

        cout << "Best Score = " << largest << endl;
        cout << "Best Time Table : \n" << endl;

        for (int i = 0; i < num_courses; i++) {
            cout << "Course = " << courses_names[i] << endl;
            int time_to_print = stoi(parent1[i][1]);
            cout << "Room Name = " << parent1[i][0] << endl;
            if (time_to_print >= 12)
                if (time_to_print == 12)
                    cout << "Time Slot = " << time_to_print << " PM" << endl;
                else
                    cout << "Time Slot = " << time_to_print - 12 << " PM" << endl;
            else
                cout << "Time Slot = " << time_to_print << " AM" << endl;
            cout << "Facilitator Name = " << parent1[i][2] << endl;

            cout << "\n";
        }

        cout << "////////////////////////////////" << endl;

    }



}



int main() {

    string facilitator[10] = { "Lock", "Glen", "Banks", "Richards", "Shaw", "Singer", "Uther", "Tyler", "Numen", "Zeldin" };

    string courses_names[11] = { "SLA101A", "SLA101B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451" };

    int courses_expected_enrollment[11] = { 50, 50, 50, 50, 50, 50, 60, 25, 20, 60, 100 };

    string courses_preferred_facilitator[11][10] = { {"Glen", "Lock", "Banks", "Zeldin"}, { "Glen", "Lock", "Banks", "Zeldin"}, { "Glen", "Lock", "Banks", "Zeldin"}, { "Glen", "Lock", "Banks", "Zeldin"}, { "Glen", "Banks", "Zeldin", "Shaw"}, { "Lock", "Banks", "Zeldin", "Singer"}, { "Glen", "Zeldin", "Banks"}, { "Glen", "Banks", "Tyler"}, {"Tyler", "Singer"}, {"Tyler", "Singer", "Shaw"}, {"Tyler", "Singer", "Shaw"} };

    string courses_other_facilitator[11][10] = { { "Numen", "Richards"}, { "Numen", "Richards"}, { "Numen", "Richards"}, { "Numen", "Richards"}, { "Numen", "Richards", "Singer"}, { "Numen", "Richards", "Shaw", "Tyler"}, { "Numen", "Singer", "Shaw"}, {"Numen", "Singer", "Shaw", "Richards", "Uther", "Zeldin"}, {"Richards", "Zeldin"}, { "Zeldin", "Uther"}, { "Zeldin", "Uther", "Richards", "Banks"} };

    string room_names[9] = { "Slater 003", "Roman 216", "Loft 206", "Roman 201", "Loft 310", "Beach 201", "Beach 301", "Logos 325", "Frank 119" };

    int room_capacity[9] = { 45, 30, 75, 50, 108, 60, 75, 450, 60 };

    int time_slots[6] = { 10, 11, 12, 13, 14, 15 };



    srand(time(0));
    // Write C++ code here
    int i = rand() % 10;

    // THis is the code to get length of string array
    int size = 0;
    for (string element : courses_preferred_facilitator[i]) {
        if (element.empty())
            break;
        size++;
        // cout<<"elements: "<< element << endl;
    }

    // int j = rand() % size;
    // cout << "Hello Mr. "<<i << " "<< j<< endl;
    // // cout << sizeOfStrArray(courses_preferred_facilitator[i]) << endl;
    // cout << courses_preferred_facilitator[i][j] << endl;

    // // sizeOfStrArray(courses_preferred_facilitator[i]);

    // int generations = 10;
    // int population = 2;
    // int num_courses = 11;
    // int num_facilitator = 10;
    // int num_times = 6;
    // int num_rooms = 9;

    string time_table[population][num_courses][3];


    for (int pop = 0; pop < population; pop++) {
        for (int sla = 0; sla < num_courses; sla++) {
            time_table[pop][sla][0] = room_names[rand() % num_rooms];
            time_table[pop][sla][1] = to_string(time_slots[rand() % num_times]);
            time_table[pop][sla][2] = facilitator[rand() % num_facilitator];
        }
    }

    // Printing Time Table
    // for(int pop=0; pop<population; pop++){
    //     cout << "\n\n\nPopulation = " << pop <<endl;
    //     for(int ele=0; ele<num_courses; ele++){
    //         cout << "\nCourse = " << courses_names[ele] <<endl;
    //         for(int j=0; j<3; j++)
    //             // cout<<"ele = " << ele << endl;
    //             cout<< time_table[pop][ele][j] << endl;
    //     }
    // }

    GeneticAlgorithm(time_table, courses_names, room_names, room_capacity, courses_expected_enrollment, courses_preferred_facilitator, courses_other_facilitator, facilitator, time_slots);


    cout << "\n\n\n" << generations << " Generations Completed with the population size of " << population << endl;

    return 0;
}