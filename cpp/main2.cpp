// #include <X11/X.h>
#include <boost/program_options.hpp>
#include <chrono>
#include <format>
#include <iostream>

#include "CImg.h"

using namespace cimg_library;
using namespace std;
using namespace std::chrono;

namespace po = boost::program_options;
bool quiet = false;

template <typename T> void cutoff(CImg<T> &image, double cutoff) {
  for (int i = 0; i < image.width(); i++) {
    for (int j = 0; j < image.height(); j++) {
      T *tmp = image.data(i, j);
      *tmp = *tmp > cutoff ? *tmp : 0;
    }
  }
  return;
}

template <typename T> void cutoff_par(CImg<T> &image, double cutoff) {
#pragma omp parallel for num_threads(16)
  for (int i = 0; i < image.width(); i++) {
    for (int j = 0; j < image.height(); j++) {
      T *tmp = image.data(i, j);
      *tmp = *tmp > cutoff ? *tmp : 0;
    }
  }
  return;
}

int main(int argc, char *argv[]) {
  int a;

  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "print this message and leave")(
      "input,i", po::value<string>()->required(), "path to the input file")(
      "output,o", po::value<string>(), "path to the output file")(
      "cutoff,c", po::value<double>()->required(),
      "cutoff value")("parallel,p", "run with maximum parallelism")(
      "quiet,q", "print only the number of pixels and elapsed time");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help") || argc < 2) {
    cerr << desc << "\n";
    exit(1);
  }

  string inFile = vm["input"].as<string>();
  string outFile = "";
  if (vm.count("output")) {
    outFile = vm["output"].as<string>();
  }
  double cutoffValue = vm["cutoff"].as<double>();
  quiet = vm.count("quiet");
  if (!quiet)
    cout << format("input: {}, output: {}, cutoff: {}\n", inFile, outFile,
                   cutoffValue);

  CImg<float> inputImage(inFile.c_str());
  if (!quiet)
    cout << format("Processing image of {}x{} pixels\n", inputImage.width(),
                   inputImage.height());

  auto start = high_resolution_clock::now();

  if (vm.count("parallel")) {
    cutoff_par(inputImage, cutoffValue);
  } else {
    cutoff(inputImage, cutoffValue);
  }

  auto end = high_resolution_clock::now();
  std::chrono::duration<double> elapsedTime = end - start;
  // time_t timeDiff = high_resolution_clock::to_time_t(end);
  if (!quiet)
    cout << format("Elapsed time: {}\n", elapsedTime);
  else
    cout << format("{} {}\n", inputImage.width() * inputImage.height(),
                   elapsedTime);

  if (vm.count("output")) {
    inputImage.save(outFile.c_str());
  }

  return 0;
}
