
************
Introduction
************

``lib_xcore_math`` is a library of optimised math functions for taking advantage of the vector
processing unit (VPU) of the `XMOS` XS3 architecture (i.e `xcore.ai`).
Included in the library are functions for block floating-point arithmetic, fast Fourier transforms,
linear algebra, discrete cosine transforms, linear filtering and more.

Repository structure
====================

* */lib_xcore_math/*

  * *api/* - Headers containing the public API.
  * *script/* - Scripts used for source generation.
  * *src/*- Library source code.

* */doc/* - documentation source.
* */examples/* - Example applications.
* */tests/*  - Unit test projects.

API structure
=============

This library is organised around several sub-APIs.  These APIs collect the provided operations into
coherent groups based on the kind of operation or the types of object being acted upon.

The current APIs are:

  * Block Floating-Point Vector API
  * Vector/Array API
  * Scalar API
  * Linear Filtering API
  * Fast Fourier Transform API
  * Discrete Cosine Transform API

Using ``lib_xcore_math``
========================

``lib_xcore_math`` is intended to be used with the `XCommon CMake <https://www.xmos.com/file/xcommon-cmake-documentation/?version=latest>`_
, the `XMOS` application build and dependency management system.

``lib_xcore_math`` can be compiled for both x86 platforms and XS3 based processors.

On x86 platforms you can develop DSP algorithms and test them for functional correctness;
this is an optional step before porting the library to an `xcore` device.

To use this module, include ``lib_xcore_math`` in the application's ``APP_DEPENDENT_MODULES`` list and
include the ``xcore_math.h`` header file.

