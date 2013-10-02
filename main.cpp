/* 
 * Author: Rémi PIOTAIX <remi.piotaix@gmail.com>
 *
 */

#include <cstdlib>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options.hpp>

#include "pisinger.h"
#include "instanceType.h"

typedef unsigned long long ulong_t;

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

void filterInstanceTypes(vector<int>& instanceTypes) {
    for (std::vector<int>::iterator it = instanceTypes.begin(); it != instanceTypes.end(); it++) {
        if (*it < 1 && 4 < *it) {
            cout << "Instance type " << *it << " invalid, ignoring" << endl;
            it = instanceTypes.erase(it);
        }
    }
}

ostream& operator<<(ostream& os, const vector<ulong_t>& data) {
    for (std::vector<ulong_t>::const_iterator it = data.begin(); it != data.end(); it++) {
        if (it != data.begin())
            os << " ";
        os << *it;
    }

    return os;
}

void generateInstance(ostream& out, int numItems, int coeffRange, instanceType type, int instanceNumber, int serie, string& generatorPath, float ratioSumWj = 0.5) {
    char path[generatorPath.length() + 1];
    strcpy(path, generatorPath.c_str());

    callPisinger(numItems, coeffRange, type, instanceNumber, serie, path);
    ifstream in("test.in", ifstream::in);
    int trash;
    ulong_t utility;
    ulong_t weight;
    int countObjects;
    ulong_t knapsackSize;
    int sumWeight = 0;

    //lecture
    vector<ulong_t> objectWeights;
    vector<ulong_t> objectUtilities;
    in >> countObjects;
    for (int i = 0; i < countObjects; i++) {
        in >> trash >> utility >> weight;
        objectWeights.push_back(weight);
        objectUtilities.push_back(utility);
        sumWeight += weight;
    }

    knapsackSize = sumWeight * ratioSumWj;

    //dump in the file
    out
            << countObjects << endl
            << endl
            << objectWeights << endl
            << endl
            << objectUtilities << endl
            << endl
            << knapsackSize << flush;

}

void generateInstance(string& filename, int numItems, int coeffRange, instanceType type, int instanceNumber, int serie, string& generatorPath, float ratioSumWj = 0.5) {
    ofstream out(filename.c_str(), ofstream::out);
    generateInstance(out, numItems, coeffRange, type, instanceNumber, serie, generatorPath, ratioSumWj);
    out.close();
}

void generateInstanceStdOut(string& filename, int numItems, int coeffRange, instanceType type, int instanceNumber, int serie, string& generatorPath, float ratioSumWj = 0.5) {
    cout << endl << filename << endl << endl;
    generateInstance(cout, numItems, coeffRange, type, instanceNumber, serie, generatorPath, ratioSumWj);
}

void generateInstance(int numItems, int coeffRange, instanceType type, int instanceNumber, int serie, string& generatorPath, float ratioSumWj = 0.5) {
    generateInstance(cout, numItems, coeffRange, type, instanceNumber, serie, generatorPath, ratioSumWj);
}

string getInstancesDescription(){
    stringstream ss;
    ss << "List the types of instances you want to generate:" << endl
            << "1=Uncorrelated" << endl
            << "2=Weakly correlated" << endl
            << "3=Strongly correlated" << endl
            << "4=Inverse strongly correlated" << endl
            << "5=Almost strongly correlated" << endl
            << "6=Subset-sum" << endl
            << "7=Even-odd-subset-sum" << endl
            << "8=Even-odd knapsack" << endl
            << "9=Uncorrelated, similar weights" << endl
            << "11=Avis subset-sum" << endl
            << "12=Avis knapsack" << endl
            << "13=Collapsing KP" << endl
            << "14=Bounded strongly correlated" << endl
            << "15=no small weights" << endl;
            
    
    return ss.str();
}

/*
 * 
 */
int main(int argc, char** argv) {
    vector<int> instanceSizes;
    vector<int> instanceTypes;
    vector<int> rangeCoeffs;
    string generatorPath;
    string baseOutputFolder;
    bool groupFolder = false;
    bool toStdout = false;
    int instanceNumberStart;
    int numTestSerie;
    int numberGenerated;
    float ratioSumWeights;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "Display help message")
            ("step-instance-size,S", "Generate n1 instance sizes from n2 using a step of n3 (use the -s option)")
            ("instance-sizes,s", po::value<vector<int> >(&instanceSizes)->multitoken(), "List of the instances' sizes that will be generated")
            ("instance-types,t", po::value<vector<int> >(&instanceTypes)->multitoken(), getInstancesDescription().c_str())
            ("instance-number,n", po::value<int>(&instanceNumberStart)->default_value(1), "The number of the first instance that will be generated, for the others, n=n+1")
            ("coeff-range,c", po::value<vector<int> >(&rangeCoeffs)->multitoken(), "Range of the coefficients")
            ("number-test-series,N", po::value<int>(&numTestSerie)->default_value(1000), "Number of tests in series")
            ("generator-path,g", po::value<string>(&generatorPath)->default_value("pisinger_generator"), "Path to the pisinger generator")
            ("output-folder,o", po::value<string>(&baseOutputFolder)->default_value(""), "Change the destination folder for the generated files")
            ("group-by-instance-type,G", "Allow to group the instances in folders based on instance type")
            ("repeat,R", po::value<int>(&numberGenerated)->default_value(1), "Nomber of instance to be generated for each combinaison of type/range/size")
            ("ratio,r", po::value<float>(&ratioSumWeights)->default_value(0.5), "Ratio applicated tu the sum of weights to calculate the knapsack size")
            ("stdout", "Print all the instance to stdout")
            ;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);

    //    cout << get_current_dir_name() << endl;

    if (vm.count("help")) {
        cout << desc << endl;
        return EXIT_SUCCESS;
    }

    if (rangeCoeffs.size() == 0) {
        cerr << "No range selected." << endl;
        return EXIT_FAILURE;
    }

    if (vm.count("stdout")) {
        toStdout = true;
    }

    if (vm.count("step-instance-size")) {
        if (instanceSizes.size() < 3) {
            cerr << "Error: you must pass 3 integers to the options --instance-sizes(-i)" << endl;
            exit(EXIT_FAILURE);
        } else {
            int countInstances = instanceSizes[0];
            int from = instanceSizes[1];
            int step = instanceSizes[2];

            instanceSizes.clear();
            for (int i = 0; i < countInstances; i++) {
                instanceSizes.push_back(from + i * step);
            }
        }
    }

    if (vm.count("group-by-instance-type")) {
        groupFolder = true;
    }

    filterInstanceTypes(instanceTypes);
    //Pour chaque type d'instance
    for (vector<int>::iterator itTypes = instanceTypes.begin(); itTypes != instanceTypes.end(); itTypes++) {
        int instanceNumber = instanceNumberStart;
        instanceType type = instanceType(*itTypes);
        string outputFolder = baseOutputFolder;
        if (groupFolder) {
            stringstream ss;
            ss << outputFolder << '/' << type;

            fs::path folderPath(ss.str());
            if (!fs::exists(folderPath)) {
                if (!fs::create_directory(folderPath)) {
                    cerr << "Fail to create folder: " << ss.str() << endl;
                    return EXIT_FAILURE;
                }
            } else if (!fs::is_directory(folderPath)) {
                cerr << "the filename " << type << " already exists in the folder " << outputFolder << endl;
                return EXIT_FAILURE;
            }

            // si on arrive la c'est que l'on a soit créé le dossier avec success, soit qu'il existe deja
            outputFolder = ss.str();
        }

        if (outputFolder.length() != 0) {
            outputFolder += "/";
        }

        cerr << "type: " << type << endl;
        //Pour chaque taille d'instance
        for (vector<int>::iterator itSizes = instanceSizes.begin(); itSizes != instanceSizes.end(); itSizes++) {
            int size = *itSizes;
            //Pour chaque ecart de coeffs
            for (vector<int>::iterator itRange = rangeCoeffs.begin(); itRange != rangeCoeffs.end(); itRange++) {
                int rangeCoeff = *itRange;
                //générer 'repeat' instances de chaque type/range/taille
                for (int repeat = 1; repeat <= numberGenerated; repeat++) {

                    stringstream ss;
                    ss << outputFolder << "KP_" << type << "_" << size << "_R" << rangeCoeff << "_" << instanceNumber;
                    cerr << "Generating " << ss.str() << "Ratio: " << repeat / ((float) numberGenerated + 1) << endl;
                    string filename = ss.str();

                    float ratio = 0.5;
                    if (numberGenerated > 1) {
                        ratio = repeat / ((float) numberGenerated + 1);
                    }
                    if (toStdout)
                        generateInstanceStdOut(filename, size, rangeCoeff, type, instanceNumber, numTestSerie, generatorPath, ratio);
                    else
                        generateInstance(filename, size, rangeCoeff, type, instanceNumber, numTestSerie, generatorPath, ratio);


                    instanceNumber++;
                }
            }
        }
    }


    return 0;
}

