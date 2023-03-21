#include<bits/stdc++.h>

using namespace std;
vector<vector<long long int>> runs; // vector to store sorted runs
int seeks = 0;  // total no of seeks
int transfer = 0;  // total no of transfer
int sorted_seeks = 0;  // total no of sorted seeks
int sorted_transfer = 0;  // total no of sorted transfer 
vector<int> merge_seeks;  // vector of seeks in different merge pass such that merge_seeks[0] = no of seeksin merge pass 1
vector<int> merge_transfer;  // same as above except transfer in place of seeks


// all the implementation details and executation is done using proper comments



/*        *************************************************************************************         
                                      algorithm
source: Database system concepts, 7th edition


1 In the ﬁrst stage, a number of sorted runs are created; each run is sorted but
contains only some of the records of the relation.
            i = 0;
            repeat
                    read M blocks of the relation, or the rest of the relation,
                    whichever is smaller;
                    sort the in-memory part of the relation;
                    write the sorted data to run ﬁle R i ;
                    i = i + 1;
            until the end of the relation
2. In the second stage, the runs are merged. Suppose, for now, that the total number
of runs, N, is less than M, so that we can allocate one block to each run and have
space left to hold one block of output. The merge stage operates as follows:

read one block of each of the N ﬁles R i into a buﬀer block in memory;
repeat
        choose the ﬁrst tuple (in sort order) among all buﬀer blocks;
        write the tuple to the output, and delete it from the buﬀer block;
        if the buﬀer block of any run R i is empty and not end-of-ﬁle(R i )
        then read the next block of R i into the buﬀer block;
until all input buﬀer blocks are empty

                                         
                                         
                                         
                                         file names
sorted.txt: final sorted output
RUNS_No.txt: initial sorted runs 
Merge_No.txt: merge pass
and other outputs is put in the terminal


          *************************************************************************************         */




/*        *************************************************************************************         
                                      execution

 make clean
 make run
 ./executable file m k n b

          *************************************************************************************         */




// merge function takes x(maximum m*b/k -1) vectors such that one element from each vector can be hold in memory and 1 for output
// in that way reading a element from vectors is equaivalent to reading first element from each file in disk(i have implemented disk as vector)
// merge the sorted vector in sorted order pass the result in new vector(disk) 
void merge(vector<vector<long long int>>& runs, vector<long long int>& temp){
    vector<long long int> pointer;
    long long int num = 0, min, k, i, j;
    for (i = 0; i < runs.size(); i++){
        pointer.push_back(0);
        num += runs[i].size();
    }
    for (i = 0; i < num; i++,seeks++,transfer++){
        min = LLONG_MAX;
        k = 0;
        for (j = 0; j < pointer.size(); j++){
            if (pointer[j] < runs[j].size() && runs[j][pointer[j]] < min){
                min = runs[j][pointer[j]];
                k = j;
            }
        }
        temp.push_back(min);
        seeks++;transfer++;
        pointer[k]++;
    }
}


int main(int argc, char* argv[]){

    // numbers of arguement should be equal to 5
    if(argc<6){
        cout<<"ERROR!!! Numbers of inputs must be 5";
    }
    ifstream file;
    file.open(argv[1]);
    long long int m = stoi(argv[2]), k = stoi(argv[3]), n = stoi(argv[4]), b = stoi(argv[5]);



    // sort



    long long int sorted_runs= ceil((n*k)/(m*b)), block_size = m*b;
    string key;
    vector<long long int> vec; // vector to hold a single run
    ofstream temp;

    // upper loop is for no of runs
    // inner loop to take maximum element in memory that can be fitted and sort it using sort stl
    // push back to file(RUNS_number.txt) and take next set

    for(long long int i = 0; i<= sorted_runs; i++, seeks++){
        for(long long int j = 0 ; j<block_size ; j+=k, transfer++){
            if(file.eof()){
                break;
            }
            getline(file,key); // get a key from file and push it into vector
            vec.push_back(stoi(key));          
        }
        sort(vec.begin(),vec.end()); // sort a single run 
        temp.open("RUNS_"+ to_string(i) + ".txt",ios::out);
        seeks++;
        for(long long int j = 0; j<vec.size() ; j++,transfer++){
            temp<<to_string(vec[j])<<endl;
        }
        runs.push_back(vec); // vector of all sorted runs
        vec.clear();
        temp.close();
    }
    file.close();

    sorted_seeks = seeks;
    sorted_transfer= transfer;

    // merge

    int seektemp = seeks, transfertemp = transfer;  // variables to store seeks and tranfer of last merge pass

    int cnt=1;
    while (runs.size()>1) 
    {
        vector<vector<long long int>> vec; // vector to store each merge pass status 
        // loop to pass over all vectors
        for(long long int j = 0 ; j < runs.size(); j+=((m*b)/k - 1)){
                vector<vector<long long int>> mer ;
                // loop for taking n(maximum -1 that can be fit in memory or remaining vectors) vectors
                for(long long int i = j ; i < j + (m*b)/k - 1 && i < runs.size() ; i++) mer.push_back(runs[i]); 
                vector<long long int> temp; // to hold the result of merge of above vectors
                merge(mer,temp);
                vec.push_back(temp);
                mer.clear();
        }
        runs.clear();
        runs = vec;
        vec.clear();
        ofstream fout("Merge_"+ to_string(cnt) + ".txt");
        for (auto i : runs)
        {
            for (auto j:i)
            {
                fout<<j<<" ";
            }
            fout<<endl<<endl<<endl;
        }
        cnt++;
        merge_seeks.push_back(seeks-seektemp);          // to calculate no of seeks in each marge pass
        merge_transfer.push_back(transfer-transfertemp); // same as above
        seektemp=seeks;
        transfertemp=transfer;
    }
    ofstream fout("Sorted.txt");
    for (auto i : runs)
    {
        for (auto j:i)
        {
            fout<<j<<endl;
        }
        
    }
    cout<<"No of Seeks: "<<seeks<<endl;
    cout<<"No of Transfers: "<<transfer<<endl;
    cout<<"No of Merge Passes: "<<cnt-1<<endl;
    cout<<"No of sorted Seeks: "<<sorted_seeks<<endl;
    cout<<"No of sorted Transfers: "<<sorted_transfer<<endl;
    for(int i = 0 ;i <merge_seeks.size() ; i++){
        cout<<"No of Seeks in merge pass "<<i+1<<": "<<merge_seeks[i]<<endl;
    }
    for(int i = 0 ;i <merge_transfer.size() ; i++){
        cout<<"No of transfer in merge pass "<<i+1<<": "<<merge_transfer[i]<<endl;
    }
}