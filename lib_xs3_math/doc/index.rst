.. lib_xs3_math documentation master file, created by
   sphinx-quickstart on Mon Jul 12 11:09:43 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Introduction
============

.. toctree::
   :maxdepth: 2
   :caption: Contents:
   :glob:

   src/getting_started
   src/fft_lengths

   src/reference/index


``lib_xs3_math`` is a library of optimized math functions for taking advantage of the vector processing unit (VPU) on the XMOS XS3 architecture.

See :ref:`getting_started` to get going.

On GitHub
---------

Get the latest version of ``lib_xs3_math`` at https://github.com/xmos/lib_xs3_math.

API
---

To use the functions in this library in an application, include :ref:`bfp_math_h` for the high-level block floating-point 
functions, or :ref:`xs3_math_h` for only the low-level functions.


Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
