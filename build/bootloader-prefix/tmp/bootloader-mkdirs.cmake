# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Applications/esp/esp-idf/components/bootloader/subproject"
  "/Users/macbookpro/josef/chatzigiannakis-vitaletti/projects/individual_ass/build/bootloader"
  "/Users/macbookpro/josef/chatzigiannakis-vitaletti/projects/individual_ass/build/bootloader-prefix"
  "/Users/macbookpro/josef/chatzigiannakis-vitaletti/projects/individual_ass/build/bootloader-prefix/tmp"
  "/Users/macbookpro/josef/chatzigiannakis-vitaletti/projects/individual_ass/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/macbookpro/josef/chatzigiannakis-vitaletti/projects/individual_ass/build/bootloader-prefix/src"
  "/Users/macbookpro/josef/chatzigiannakis-vitaletti/projects/individual_ass/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/macbookpro/josef/chatzigiannakis-vitaletti/projects/individual_ass/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/macbookpro/josef/chatzigiannakis-vitaletti/projects/individual_ass/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
