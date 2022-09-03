# Magic Lantern Digital Workprint Library API

## Overview

The Magic Lantern Digital Workprint Library is a component of Magic Lantern,
an interactive, title development platform for authoring highly interactive
titles, such as interactive TV applications, games, educational programs, and
point-of-information/sales kiosks. The DWP library provides utilities for
constructing and maintaining a Digital Workprint.

The Magic Lantern DWP Library is distributed as two static libraries and two
dynamic link libraries. The build environment distributed with the source for
the DWP library can be used to build any or all of these libraries.

The libraries are
<ul>
  <li>tools/DWP.lib       - Release Static Library</li>
  <li>tools/DWPd.lib      - Debug Static Library</li>
  <li>rehearsal/DWP.dll   - Release Dynamic Link Library</li>
  <li>rehearsal/DWPd.dll  - Debug Dynamic Link Library</li>
</ul>

The tools version of the libraries are used with Magic Lantern tools and
can be found under <b>MLE_HOME/lib/tools</b>. The rehearsal version
of the libraries are distributed as a DLL/DSO and are used with the Magic
Lantern Rehearsal Player. They can be found under <b>MLE_HOME/bin/rehearsal</b>.

## Build Instructions

### Building for Microsoft Windows

Instructions for building the Digital Workprint library for the Microsoft
Windows platform can be found on the Magic Lantern [github mle-documentation
wiki](https://github.com/magic-lantern-studio/mle-documentation/wiki/SDK-Build-Environment-using-Microsoft-Visual-Studio#build-the-digital-workprint-libraries).

### Building for Linux

Instructions for building the Digital Workprint library for the Linux
platform can be found on the Magic Lantern [github mle-documentation wiki]
(https://github.com/magic-lantern-studio/mle-documentation/wiki/SDK-Build-Environment-for-Linux-Platform#build-the-digital-workprint-libraries).
