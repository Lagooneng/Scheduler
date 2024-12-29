/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32204236
*	    Student name : 정현우
*/

#include <string>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include "sched.h"


class SPN : public Scheduler{
    private:
        /*
        * 구현 (멤버 변수/함수 추가 및 삭제 가능)
        */
	    struct cmp {
	    	bool operator()(Job x, Job y) {
			return x.service_time > y.service_time;
		}
	    };
	    std::priority_queue< Job, std::vector<Job>, cmp > job_heap_;
    public:
        SPN(std::queue<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead) {
            name = "SPN";
            /*
            * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
            * 나머지는 자유롭게 수정 및 작성 가능
            */
        }

        int run() override {
            /*
            * 구현 
            */
		while( !job_queue_.empty() ) {
			Job testJobArrived = job_queue_.front();

			if( testJobArrived.arrival_time <= current_time_ ) {
				job_heap_.push( testJobArrived );
				job_queue_.pop();	
			}
			else {
				break;
			}
		}

		if( current_job_.name == 0 && !job_heap_.empty() ) {
			current_job_ = job_heap_.top();
			job_heap_.pop();
		}

		if( current_job_.remain_time == 0 ) {
			current_job_.completion_time = current_time_;
			end_jobs_.push_back( current_job_ );

			if( job_heap_.empty() ) return -1;

			current_job_ = job_heap_.top();
			job_heap_.pop();
			current_time_ += switch_time_;
		}
			
		if( current_job_.service_time == current_job_.remain_time ) {
			current_job_.first_run_time = current_time_;
		}

		current_time_++;
		current_job_.remain_time--;
		
            return current_job_.name;
        }
};

class RR : public Scheduler{
    private:
        int time_slice_;
        int left_slice_;
        std::queue<Job> waiting_queue;
        /*
        * 구현 (멤버 변수/함수 추가 및 삭제 가능)
        */
    public:
        RR(std::queue<Job> jobs, double switch_overhead, int time_slice) : Scheduler(jobs, switch_overhead) {
            name = "RR_"+std::to_string(time_slice);
            /*
            * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
            * 나머지는 자유롭게 수정 및 작성 가능 (아래 코드 수정 및 삭제 가능)
            */
            time_slice_ = time_slice; 
            left_slice_ = time_slice;
        }

        int run() override {
            	while( !job_queue_.empty() ) {
                        Job testJobArrived = job_queue_.front();

                        if( testJobArrived.arrival_time <= current_time_ ) {
                                waiting_queue.push( testJobArrived );
                                job_queue_.pop();
                        }
                        else {
                                break;
                        }
                }

                if( current_job_.name == 0 && !waiting_queue.empty() ) {
                        current_job_ = waiting_queue.front();
                        waiting_queue.pop();
                }

		if( current_job_.remain_time == 0 ) {
                        current_job_.completion_time = current_time_;
                        end_jobs_.push_back( current_job_ );

                        if( waiting_queue.empty() ) return -1;

                        current_job_ = waiting_queue.front();
                        waiting_queue.pop();

                        current_time_ += switch_time_;
			left_slice_ = time_slice_;
                }

		if( left_slice_ == 0 ) {
			left_slice_ = time_slice_;

			waiting_queue.push( current_job_ );
			current_job_ = waiting_queue.front();
			
			waiting_queue.pop();

			current_time_ += switch_time_;
		}

                if( current_job_.service_time == current_job_.remain_time ) {
                        current_job_.first_run_time = current_time_;
                }

                current_time_++;
                current_job_.remain_time--;
		left_slice_--;

            return current_job_.name;
        }
                
};

class SRT : public Scheduler{
    private:
        /*
        * 구현 (멤버 변수/함수 추가 및 삭제 가능)
        */
	struct cmp {
                bool operator()(Job x, Job y) {
			if( x.remain_time == y.remain_time ) return x.name > y.name;
			else return x.remain_time > y.remain_time;
                }
        };

	std::priority_queue< Job, std::vector< Job >, cmp > job_heap_;
    public:
        SRT(std::queue<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead) {
            name = "SRT";
            /*
            * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
            * 나머지는 자유롭게 수정 및 작성 가능
            */
        }

        int run() override {
            /*
            * 구현 
            */
	    	while( !job_queue_.empty() ) {
                        Job testJobArrived = job_queue_.front();

                        if( testJobArrived.arrival_time <= current_time_ ) {
                                job_heap_.push( testJobArrived );
                                job_queue_.pop();
                        }
                        else {
                                break;
                        }
                }

                if( current_job_.name == 0 && !job_heap_.empty() ) {
                        current_job_ = job_heap_.top();
                        job_heap_.pop();
                }

		if( current_job_.remain_time == 0 ) {
                        current_job_.completion_time = current_time_;
                        end_jobs_.push_back( current_job_ );

                        if( job_heap_.empty() ) return -1;

                        current_job_ = job_heap_.top();
                        job_heap_.pop();
                        current_time_ += switch_time_;
                }

		if( !job_heap_.empty() && current_job_.remain_time > job_heap_.top().remain_time ) {
                        job_heap_.push( current_job_ );
			current_job_ = job_heap_.top();
                        job_heap_.pop();
			
                        current_time_ += switch_time_;
                }

                if( current_job_.service_time == current_job_.remain_time ) {
                        current_job_.first_run_time = current_time_;
		}
		
                current_time_++;
                current_job_.remain_time--;
		 
            return current_job_.name;
        }
};

class HRRN : public Scheduler{
    private:
        /*
        * 구현 (멤버 변수/함수 추가 및 삭제 가능)
        */    
	    std::vector<Job> waiting_vector;
    public:
        HRRN(std::queue<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead) {
            name = "HRRN";
            /*
            * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
            * 나머지는 자유롭게 수정 및 작성 가능
            */	    
        }

	int getNextIdx() {
		int length = waiting_vector.size();
                double maxResponseRate = -1.0;
             	int idx = 0;
		
            	for( int i = 0; i < length; ++i ) {
              		Job temp_job = waiting_vector[i];
          		double responseRate = ( current_time_ - temp_job.arrival_time + temp_job.service_time ) / temp_job.service_time;

           		if( maxResponseRate < responseRate ) {
               			idx = i;
                 		maxResponseRate = responseRate;
         		}
    		}
		
		return idx;
	}

        int run() override {
            /*
            구현 
            */
		while( !job_queue_.empty() ) {
                        Job testJobArrived = job_queue_.front();

                        if( testJobArrived.arrival_time <= current_time_ ) {
                                waiting_vector.push_back( testJobArrived );
                                job_queue_.pop();
                        }
                        else {
                                break;
                        }
                }

		if( current_job_.name == 0 && !waiting_vector.empty() ) {
			int nextIdx = getNextIdx();
                        current_job_ = waiting_vector[nextIdx];
                        waiting_vector.erase( waiting_vector.begin() + nextIdx );
                }

                if( current_job_.remain_time == 0 ) {
                        current_job_.completion_time = current_time_;
                        end_jobs_.push_back( current_job_ );

                        if( waiting_vector.empty() ) return -1;
			
			int nextIdx = getNextIdx();
                        current_job_ = waiting_vector[nextIdx];
                        waiting_vector.erase( waiting_vector.begin() + nextIdx );
                        current_time_ += switch_time_;
                }

		if( current_job_.service_time == current_job_.remain_time ) {
                        current_job_.first_run_time = current_time_;
                }

                current_time_++;
                current_job_.remain_time--;

            return current_job_.name;
        }
};

// FeedBack 스케줄러 (queue 개수 : 4 / boosting 없음)
class FeedBack : public Scheduler{
    private:
        /*
        * 구현 (멤버 변수/함수 추가 및 삭제 가능)
        */    
	std::queue<Job> waiting_queue[4];
	int time_slice_[4];
	int now_time_slice_, left_slice_;
	int now_priority = 0;
	int feedbackStartFlag = -1;
    public:
        FeedBack(std::queue<Job> jobs, double switch_overhead, bool is_2i) : Scheduler(jobs, switch_overhead) {
            if(is_2i){
                name = "FeedBack_2i";
            } else {
                name = "FeedBack_1";
            }

            /*
            * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
            * 나머지는 자유롭게 수정 및 작성 가능
            */

	    if(is_2i) {
	    	time_slice_[0] = 1;
		time_slice_[1] = 2;
		time_slice_[2] = 4;
		time_slice_[3] = 8;
	    }
	    else {
	    	time_slice_[0] = 1;
                time_slice_[1] = 1;
                time_slice_[2] = 1;
                time_slice_[3] = 1;
	    }
        }

	bool multiQueuing() {
                if( !waiting_queue[0].empty() ) {
                        current_job_ = waiting_queue[0].front();
                        waiting_queue[0].pop();
                        left_slice_ = time_slice_[0];
                        now_priority = 0;
                }
                else if( !waiting_queue[1].empty() ) {
                        current_job_ = waiting_queue[1].front();
                        waiting_queue[1].pop();
                        left_slice_ = time_slice_[1];
                        now_priority = 1;
                }
                else if( !waiting_queue[2].empty() ) {
                        current_job_ = waiting_queue[2].front();
                        waiting_queue[2].pop();
                        left_slice_ = time_slice_[2];
                        now_priority = 2;
                }
                else if( !waiting_queue[3].empty() ) {
                        current_job_ = waiting_queue[3].front();
                        waiting_queue[3].pop();
                        left_slice_ = time_slice_[3];
                        now_priority = 3;
                }
                else {
                        return false;
                }

                return true;
        }


        int run() override {
            /*
            * 구현 
            */
	     	while( !job_queue_.empty() ) {
                        Job testJobArrived = job_queue_.front();

                        if( testJobArrived.arrival_time <= current_time_ ) {
                                waiting_queue[0].push( testJobArrived );
                                job_queue_.pop();
				if( feedbackStartFlag < 1 ) {
					feedbackStartFlag++;
				}
                        }
                        else {
                                break;
                        }
                }
		
		if( feedbackStartFlag > 0 ) {
                        left_slice_--;
                }
		
		if( current_job_.name == 0 && !waiting_queue[0].empty() ) {
			current_job_ = waiting_queue[0].front();
			waiting_queue[0].pop();
			now_time_slice_ = time_slice_[0];
			left_slice_ = time_slice_[0];
			now_priority = 0;
                }

                if( current_job_.remain_time == 0 ) {
                        current_job_.completion_time = current_time_;
                        end_jobs_.push_back( current_job_ );
			
			if( !multiQueuing() ) return -1;

                        current_time_ += switch_time_;
                }
		
		if( left_slice_ == 0 ) {
			if( now_priority < 3 ) {
				waiting_queue[ now_priority + 1 ].push( current_job_ );
			}
			else {	// min priority
				waiting_queue[ now_priority ].push( current_job_ );
			}
			
			multiQueuing();
			//std::cout << now_priority << '\n';
                        current_time_ += switch_time_;
                }

		if( current_job_.service_time == current_job_.remain_time ) {
                        current_job_.first_run_time = current_time_;
                }

                current_time_++;
                current_job_.remain_time--;
			
            return current_job_.name;
        }
};
