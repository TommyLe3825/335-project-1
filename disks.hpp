///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// TODO
#include <functional>
#include <iostream>

enum disk_color { DISK_LIGHT, DISK_DARK};

class disk_state {
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

    assert(light_count > 0);

    for (size_t i = 1; i < _colors.size(); i += 2) {
      _colors[i] = DISK_DARK;
    }
  }

  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_initialized() const {
    for(size_t i = 0; i < total_count(); i+=2) {   //Iterate every other element
      if(get(i) != DISK_LIGHT) {                   //If the first (0) and every even element is not DISK_LIGHT, return false
        return false;
      }
    }

    for(size_t i = 1; i < total_count(); i+=2) {   //Iterate every other element
      if(get(i) != DISK_DARK) {                    //If the second (1) and every odd element is not DISK_LIGHT, return false
        return false;
      }
    }

      return true;        //If the vector have alternating disks (ie. DISK_LIGHT, DISK_DARK, DISK_LIGHT, DISK_DARK), return true
  }

  // Return true when this disk_state is fully sorted, with all light disks on
  // the left (low indices) and all dark disks on the right (high indices).
  bool is_sorted() const {
    for(size_t i = 0; i < light_count(); i++) {   //Iterate through half of the vector
      if(get(i) != DISK_LIGHT) {                  //If one of the element is not DISK_LIGHT, vector is not sorted, return false
        return false;
      }
    }

    for(size_t i = dark_count(); i < total_count(); i++) {  //Iterate through second half of the vector
      if(get(i) != DISK_DARK) {                             //If one of the element is not DISK_DARK, vector is not sorted, return false
        return false;
      }
    }
    return true;          //If the first half of the vector is all DISK_LIGHT and the second half of the vector is all DISK_DARK,
                          //the vector is sorted, return true
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the alternate algorithm.
sorted_disks sort_alternate(const disk_state& before) {

  disk_state disk = before;
  int swap = 0;                      //Swap counter
  size_t n = disk.light_count();     //Number of light disks

  for(size_t i = 0; i < n + 1; i++) {                                    //Total of n + 1 runs
    for(size_t j = i % 2; j < 2*n - 1; j+=2) {                           //Either starts on the first element (0) or the second element(1)
      if(disk.get(j) == DISK_DARK && disk.get(j+1) == DISK_LIGHT) {      //If the left disk is DISK_Dark and the right disk is DISK_LIGHT,
        disk.swap(j);               //swap disks
        swap++;                     //Increment swap counter
      }
    }
  }

  return sorted_disks(disk, swap);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {

  disk_state disk = before;
  int swap = 0;                             //Swap counter
  size_t n = disk.light_count();            //Number of light disks

  for(size_t i = 0; i < (n+1)/2; i++) {                                //Total of (n+1)/2 runs
    for(size_t j = 0; j < 2*n - 1; j++) {                              //Iterate through the vector from left to right
      if(disk.get(j) == DISK_DARK && disk.get(j+1) == DISK_LIGHT) {    //If the left disk is DISK_DARK and the right disk is DISK_LIGHT,
        disk.swap(j);                                                  //swap disks
        swap++;                                                        //Increment swap counter
      }
    }
    for(size_t j = 2*n - 1; j > 0; j--) {                              //Iterate through the vector from right to left
      if(disk.get(j) == DISK_LIGHT && disk.get(j-1) == DISK_DARK) {    //If the right disk is DISK_LIGHT and the left disk is DISK_DARK,
        disk.swap(j-1);            //swap disk
        swap++;                    //Increment swap counter
      }
    }
  }

  return sorted_disks(disk, swap);
}
