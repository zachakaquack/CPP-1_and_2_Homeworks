#include "globals.h"
#include "logger.h"
#include "sort.h"
#include "time_handler.h"
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

int main() {
    using namespace std::chrono_literals;
    using SA = Sorter::SortAlgorithm;
    Sorter sort = Sorter();
    Logger logger("MAIN");

    std::cout << "README:" << '\n';
    printHelpThingyYupYupYupYupYup();

    auto *promptedGB = promptForGB(); // the question is within the function
    if (promptedGB == nullptr) {      // user wants to quit
        std::cout << "Exiting..." << '\n';
        return 0;
    }
    int maxGB = *promptedGB;
    delete promptedGB;

    logger.log("Chosen GB: " + std::to_string(maxGB));
    logger.warn("Pretend Warning Here");
    logger.error("Pretend Error Here");

    if (maxGB < 4) {
        std::cout << "There is not enough memory to run to a vector of "
                     "size 1 billion."
                  << '\n';
        std::cout << "If you really want to continue (THE PROGRAM WILL EXCEED "
                     "THE GIVEN MEMORY BEFORE 1 BILLION), please make the "
                     "program skip the "
                     "memory check on line 32 of main.cpp (if (maxGB < 4)...)"
                  << '\n';
        return 0;
    }

    std::cout << std::string(53, '-') << '\n';

    for (int i = 0; i < BILLION; i += 1) {
        logger.log("Creating vector of size: " + std::to_string(sizes[i]));
        std::vector<short> vector = createRandomizedVector(sizes[i]);
        logger.log("Finished creating vector of size: " +
                   std::to_string(sizes[i]));
        // logger.warn("Currently used GB: " +
        //             std::to_string(getCurrentlyUsedGB()) + "gb");
        printCenter("[BEGIN SORTING LOOP]");

        // print quantity at the beginning of all loops
        printDottedLine("SIZE OF VECTOR", formatWithCommas(sizes[i]), 55);

        // DETERMINE AMOUNT OF MEMORY WE CAN SUCCESFULLY ALLOCATE
        size_t maxThreads = ((maxGB * std::pow(10, 9)) - (vector.size() * 2)) /
                            (vector.size() * 2);

        // make sure it's always at least one thread
        if (maxThreads < 1) {
            maxThreads = 1;
        }

        logger.log("Calculated vector memory usage: " +
                   std::to_string((vector.size() * 2) / (std::pow(10, 9))) +
                   " GB");
        logger.log("Calculated max thread count: " +
                   std::to_string(maxThreads));

        printDottedLine("MAX THREAD COUNT", formatWithCommas(maxThreads), 55);

        // The thread and future are now stored in the same vector :3
        // It has to be in this order and i hate it but everything crashes and
        // burns if it is {thread, future}
        std::vector<std::tuple<std::future<bool>, std::thread>> threads;
        int algorithmIndex = 0;

        logger.log("Starting algorithms");
        while (algorithmIndex < SA::LAST_ENUM) {
            // We have hit the max threads, do not make any more and check to
            // see if any threads are finished.
            if (threads.size() >= maxThreads) {
                for (size_t t = 0; t < threads.size(); t += 1) {
                    // Grab the thread and future out of the tuple
                    auto &thread = std::get<1>(threads[t]);
                    auto &future = std::get<0>(threads[t]);
                    // If future is not ready, check the next one.
                    if (future.wait_for(0ms) != std::future_status::ready) {
                        // TODO: this adds thousands of lines to the logger and
                        // isnt needed lol
                        // TODO: logger.log("Future not ready, skipping thread
                        // #" + std::to_string(t));
                        continue;
                    }
                    // We know the thread is finished, so join and delete it
                    // from the array
                    logger.log("Future is ready, joining thread #" +
                               std::to_string(t));
                    thread.join();
                    threads.erase(threads.begin() + t);
                    logger.log("Joined Thread #" + std::to_string(t));
                }
                continue;
            }
            logger.log("Creating thread SA #" + std::to_string(algorithmIndex));
            // Create a promise, which we will pass to the thread (while giving
            // it ownership) This is so we can know when the thread is finished.
            std::promise<bool> promise;
            // Push the promise's future (there will be a value but not yet)
            // and the thread it signals to the vector
            threads.push_back({promise.get_future(),
                               std::thread(&Sorter::startSort, sort, vector,
                                           static_cast<SA>(algorithmIndex),
                                           sizes[i], std::move(promise), i)});

            logger.log("Created thread SA::" +
                       algorithmToName[static_cast<SA>(algorithmIndex)]);
            logger.log("Thread vector size: " + std::to_string(threads.size()));
            // logger.warn("Currently used GB: " +
            //             std::to_string(getCurrentlyUsedGB()) + "gb");

            // Yippee! Next algorithm!
            algorithmIndex += 1;
        }

        // join all threads at the end
        logger.log("Joining all threads");
        for (auto &thread : threads) {
            std::get<1>(thread).join();
        }

        // logger.warn("Currently used GB: " +
        //             std::to_string(getCurrentlyUsedGB()) + "gb");
        logger.log("Deleting threads vector");
        threads.erase(threads.begin(), threads.end());
        // logger.warn("Currently used GB: " +
        //             std::to_string(getCurrentlyUsedGB()) + "gb");

        // we are now at the end. we can run radix now to avoid the memory
        // duplicates
        // WARN: I HATE THIS - Cat
        // NOTE: love you too - zach
        std::cout << "RUNNING SPECIAL RADIX SORT" << '\n';
        std::promise<bool> promise;
        sort.startSort(ref(vector), SA::RADIX, sizes[i], std::move(promise), i);

        std::cout << "FINISHED SPECIAL RADIX SORT" << '\n';
        printCenter("[END SORTING LOOP]");
        std::cout << '\n' << std::string(53, '-') << "\n\n";
    }

    TimeHandler handler = sort.timeHandler();
    printAllTimes(handler);
    writeCSVDataToFileFromHandler(handler);

    return 0;
}
