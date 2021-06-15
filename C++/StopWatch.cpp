#include "BasicRayMarch.hpp"

/*
class StopWatch {        // The class
  private:
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
  public:
  void Start() {  
      t1 = std::chrono::high_resolution_clock::now();
    }
  void Stop() {
      t2 = std::chrono::high_resolution_clock::now();
  }
  float Time(){
      std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      return time_span.count();
  }
};*/

void StopWatch::Start()
{
    t1 = std::chrono::high_resolution_clock::now();    
}

void StopWatch::Stop()
{
    t2 = std::chrono::high_resolution_clock::now();
}

float StopWatch::Time()
{
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    return time_span.count();        
}