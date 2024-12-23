//============================================================================
// Name        : ThreadTest.cpp
// Author      : Copyright (c) 2024 Kent Thompson
// Version     :
// Description : shows Truly asynchronous threads working as they would be if running/monitoring on machines
//				 or separate processes that are not synchronous
// Build		:  g++ -std=c++17 -O0 -g3 -Wall -fmessage-length=0 -fsanitize=thread  -o Threads  Threads.cpp
//============================================================================
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <mutex>
#include <chrono>
#include <sstream>
#include <string>
#include<atomic>
#include <memory>
#include <vector>

using namespace std::chrono_literals;
using std::cout; using std::endl; //using std::string; // does NOT pollute global namespace and stops the useless non-informationial "sea of "stds" prefixes

enum struct State { Error = -1, None, Ready, Paused, Started, Running, Milestone, Done, Complete };
std::mutex gLock;
int gTCounter = 0;			// thread counter

struct ThreadData {
    std::atomic<std::thread::id> Id;
	std::atomic<State> state;
	std::atomic<double> elapsed;
	std::stringstream msg;	// cannot be atomic
};

void coutMsg( ThreadData& data ) {	// because std::stringstream cannot be atomic
	gLock.lock();
		cout << data.msg.str();
	gLock.unlock();
}

//-- worker threads / tasks --
void TaskOne( ThreadData& data ) {
	data.state = State::Started;
	const auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(2000ms);

	const auto mid = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> middiff = mid - start;
	data.msg << "TaskOne 50 Percent at " << middiff.count() << "ms\n";
	data.state = State::Milestone;

	std::this_thread::sleep_for(2000ms);
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> diff = end - start;
	data.elapsed = diff.count();
	data.state = State::Done;
 }


void TaskTwo( ThreadData& data ) {
	data.state = State::Started;
	const auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(1000ms);

	const auto mid = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> middiff = mid - start;
	data.msg << "TaskTwo 50 Percent at " << middiff.count() << "ms\n";
	data.state = State::Milestone;

	std::this_thread::sleep_for(1000ms);
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> diff = end - start;
	data.elapsed = diff.count();
	data.state = State::Done;
}
	

int main() {
    //-- instantiate threads with tasks and data
	ThreadData oneData;
    	std::thread t1( TaskOne, std::ref(oneData) );
    oneData.Id = t1.get_id();   // could also be inside task
  	t1.detach();	            // TRULY ASYNCRONOUS and independent


	ThreadData twoData;
	std::thread t2( TaskTwo, std::ref(twoData) );
        twoData.Id = t2.get_id();
	t2.detach();

	// control / event Loop - each loop is a "tick"
	cout << "loop running" << endl;
	bool running = true;    
	while ( running ) {
		std::this_thread::yield();

		try {
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wswitch"

			switch( oneData.state ) {

			case State::Started:
				++gTCounter;
				oneData.state = State::Running;
				cout << "TaskOne Running" << endl;
				break;

			case State::Milestone:
				oneData.state = State::Running;
				coutMsg( oneData );
				break;

			case State::Done:
				// could now do some additional process; clean up, et cetera
				--gTCounter;
				oneData.state = State::Complete;
				cout << "TaskOne Took " << oneData.elapsed << "ms\n";
				cout << "TaskOne Complete" << endl;
				break;
			}

			switch( twoData.state ) {
			case State::Started:
				++gTCounter;
				twoData.state = State::Running;
				cout << "TaskTwo Running" << endl;
				break;

			case State::Milestone:
				twoData.state = State::Running;
				coutMsg( twoData );
				break;

			case State::Done:
				// now do some additional process; clean up, et cetera
				--gTCounter;
				twoData.state = State::Complete;
				cout << "TaskTwo Took " << twoData.elapsed << "ms\n";
				cout << "TaskTwo Complete" << endl;
				break;
			}

		} catch( std::exception& e ) {
			cout << e.what();
			// break; or re-throw exception...
		}

		if( gTCounter < 1 ) {	// all threads done
			running = false;
		}
	}
    cout << "Data from outside Task ONE, Thread ID: " << oneData.Id << " Took " << oneData.elapsed << " Milliseconds to run" << endl;
    cout << "Data from outside Task TWO, Thread ID: " << twoData.Id << " Took " << twoData.elapsed << " Milliseconds to run" << endl;
	cout << "App Finished" << endl;
	return 0;
}
