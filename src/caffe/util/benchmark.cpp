#include <boost/date_time/posix_time/posix_time.hpp>

#include "caffe/common.hpp"
#include "caffe/util/benchmark.hpp"

namespace caffe {

#ifndef CPU_ONLY
GPUTimer::GPUTimer() {
  if (!initted()) {
    CUDA_CHECK(cudaEventCreate(&start_gpu_));
    CUDA_CHECK(cudaEventCreate(&stop_gpu_));
    initted_ = true;
  } 
}

GPUTimer::~GPUTimer() {
  CUDA_CHECK(cudaEventDestroy(start_gpu_));
  CUDA_CHECK(cudaEventDestroy(stop_gpu_));
}

void GPUTimer::Start() {
  if (!running()) {
    CUDA_CHECK(cudaEventRecord(start_gpu_, 0));
    running_ = true;
    has_run_at_least_once_ = true;
  }
}

void GPUTimer::Stop() {
  if (running()) {
    CUDA_CHECK(cudaEventRecord(stop_gpu_, 0));
    CUDA_CHECK(cudaEventSynchronize(stop_gpu_));
    running_ = false;
  }
}

float GPUTimer::MicroSeconds() {
  if (!has_run_at_least_once()) {
    LOG(WARNING) << "Timer has never been run before reading time.";
    return 0;
  }
  if (running()) {
    Stop();
  }
  CUDA_CHECK(cudaEventElapsedTime(&elapsed_milliseconds_, start_gpu_,
                                  stop_gpu_));
  // Cuda only measure milliseconds
  elapsed_microseconds_ = elapsed_milliseconds_ * 1000;
  return elapsed_microseconds_;
}

float GPUTimer::MilliSeconds() {
  if (!has_run_at_least_once()) {
    LOG(WARNING) << "Timer has never been run before reading time.";
    return 0;
  }
  if (running()) {
    Stop();
  }
  CUDA_CHECK(cudaEventElapsedTime(&elapsed_milliseconds_, start_gpu_,
                                    stop_gpu_));
  return elapsed_milliseconds_;
}
#endif

CPUTimer::CPUTimer() {
  this->initted_ = true;
  this->running_ = false;
  this->has_run_at_least_once_ = false;
}

void CPUTimer::Start() {
  if (!running()) {
    this->start_cpu_ = boost::posix_time::microsec_clock::local_time();
    this->running_ = true;
    this->has_run_at_least_once_ = true;
  }
}

void CPUTimer::Stop() {
  if (running()) {
    this->stop_cpu_ = boost::posix_time::microsec_clock::local_time();
    this->running_ = false;
  }
}

float CPUTimer::MilliSeconds() {
  if (!has_run_at_least_once()) {
    LOG(WARNING) << "Timer has never been run before reading time.";
    return 0;
  }
  if (running()) {
    Stop();
  }
  this->elapsed_milliseconds_ = (this->stop_cpu_ -
                                this->start_cpu_).total_milliseconds();
  return this->elapsed_milliseconds_;
}

float CPUTimer::MicroSeconds() {
  if (!has_run_at_least_once()) {
    LOG(WARNING) << "Timer has never been run before reading time.";
    return 0;
  }
  if (running()) {
    Stop();
  }
  this->elapsed_microseconds_ = (this->stop_cpu_ -
                                this->start_cpu_).total_microseconds();
  return this->elapsed_microseconds_;
}

}  // namespace caffe
